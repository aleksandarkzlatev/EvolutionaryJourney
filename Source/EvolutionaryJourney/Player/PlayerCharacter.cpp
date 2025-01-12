#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Weapons/BaseWeaponSystem/BaseWeaponSystem.h"
#include "EvolutionaryJourney/Weapons/CloseRangeSystem/CloseRangeSystem.h"
#include "EvolutionaryJourney/Weapons/LongRangeSystem/LongRangeSystem.h"
#include "EvolutionaryJourney/Components/InventorySystem/Inventory/InventoryComponent.h"
#include "EvolutionaryJourney/Animations/PlayerCharacter/PlayerCharacterAnimations.h"
#include "Components/ChildActorComponent.h"
#include "Perception//AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	ThirdPersonCamera->SetupAttachment(SpringArm);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FirstPersonCamera = CreateDefaultSubobject< UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), TEXT("HeadSocket"));
	FirstPersonCamera->bUsePawnControlRotation = true;

	bIsFirstPerson = false;
	FirstPersonCamera->SetActive(false);
	ThirdPersonCamera->SetActive(true);
	bCanSwitchCamera = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	Quiver = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Quiver"));
	Quiver->SetupAttachment(GetMesh(), TEXT("QuiverSocket"));

	CloseRangeSystem = CreateDefaultSubobject<UChildActorComponent>(TEXT("Close Range Weapon Mesh"));
	if (IsValid(CloseRangeSystem))
	{
		CloseRangeSystem->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter: CloseRangeWeaponComponent->WeaponMesh is not valid"));
	}
	
	LongRangeSystem = CreateDefaultSubobject<UChildActorComponent>(TEXT("Long Range Weapon Mesh"));
	if (IsValid(LongRangeSystem))
	{
		LongRangeSystem->SetupAttachment(GetMesh(), TEXT("BowSocket"));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter: LongRangeWeaponComponent->WeaponMesh is not valid"));
	}	

	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimuli Source"));

	MaxWalkSpeed = 500;
	MaxSprintSpeed = 800;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	MaxStamina = 100;
	CurrStamina = MaxStamina;
	Level = 0;
	CurrentEXP = 0;
	EXPToLevelUp = 10.0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ACloseRangeSystem* CloseRangeWeapon = Cast<ACloseRangeSystem>(CloseRangeSystem->GetChildActor());
	if (IsValid(CloseRangeWeapon))
	{
		CloseRangeWeapon->WeaponIsActive = true;
		CloseRangeWeapon->InitializeWeapon(this);
	}
	

	ALongRangeSystem* LongRangeWeapon = Cast<ALongRangeSystem>(LongRangeSystem->GetChildActor());
	if (IsValid(LongRangeWeapon))
	{
		LongRangeWeapon->WeaponIsActive = false;
		LongRangeWeapon->InitializeWeapon(this);
	}


	ActiveWeapon = CloseRangeWeapon;
	CloseRangeSystem->SetVisibility(true);
	LongRangeSystem->SetVisibility(false);

	if (IsValid(PerceptionStimuliSource))
	{
		PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
		PerceptionStimuliSource->bAutoRegister = true;
	}

	AnimInstance = Cast<UPlayerCharacterAnimations>(GetCustomAnimInstance());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateStamina();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast < APlayerController>(Controller)) 
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) 
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		Input->BindAction(JumpActiom, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		Input->BindAction(SwitchCameraAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchCamera);
		Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndSprint);
		Input->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::StartAttack);
		Input->BindAction(SwitchToCloseRangeWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchToCloseRangeWeapon);
		Input->BindAction(SwitchToLongRangeWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchToLongRangeWeapon);
		Input->BindAction(QuitGameAction, ETriggerEvent::Triggered, this, &APlayerCharacter::QuitGame);
	}


}

void APlayerCharacter::Move(const FInputActionValue& InputNumber)
{
	FVector2D InputVector = InputNumber.Get<FVector2D>();

	if (IsValid(Controller) && !GetIsAttacking()) 
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FRotationMatrix RotationMatrix(YawRotation);


		const FVector ForwardDirection = RotationMatrix.GetUnitAxis(EAxis::X);
		const FVector RightDirection = RotationMatrix.GetUnitAxis(EAxis::Y);
		

		AddMovementInput(ForwardDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& InputNumber)
{
	FVector2D InputVector = InputNumber.Get<FVector2D>();

	if (IsValid(Controller)) 
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void APlayerCharacter::Jump()
{
	if (!GetIsAttacking()) ACharacter::Jump();
}

void APlayerCharacter::SwitchCamera()
{
	if (!bCanSwitchCamera) return;

	if (bIsFirstPerson) 
	{
		FirstPersonCamera->SetActive(true);
		ThirdPersonCamera->SetActive(false);
	}
	else 
	{
		FirstPersonCamera->SetActive(false);
		ThirdPersonCamera->SetActive(true);
	}

	bIsFirstPerson = !bIsFirstPerson;

	bCanSwitchCamera = false;

	FTimerHandle SwitchCameraTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SwitchCameraTimerHandle, this, &APlayerCharacter::ResetCameraSwitch, 0.5f, false);
}

void APlayerCharacter::ResetCameraSwitch()
{
	bCanSwitchCamera = true;
}

void APlayerCharacter::StartSprint()
{
	if (bHasStamina && !GetIsAttacking()) 
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;

		// In the case of the sprint button being held while the player is not moving
		if (GetVelocity().Size() >= 0.5)
		{
			bIsSprinting = true;
		}
		else 
		{
			bIsSprinting = false;
		}
	}
	
}

void APlayerCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	bIsSprinting = false;
}

void APlayerCharacter::UpdateStamina()
{
	if (GetIsAttacking()) bIsSprinting = false;

	if (bIsSprinting) 
	{
		CurrStamina -= StaminaDrainTime;
		CurrentRefillDelayTime = StaminaDelayBeforeRefill;
	}
	else if (!bIsSprinting && CurrStamina < MaxStamina) 
	{
		CurrentRefillDelayTime--;
		if (CurrentRefillDelayTime <= 0) 
		{
			CurrStamina += StaminaRefillTime;
		}
	}


	if (CurrStamina <= 0) 
	{
		bHasStamina = false;
		EndSprint();
	}
	else 
	{
		bHasStamina = true;
	}
}

void APlayerCharacter::StartAttack()
{
	if (IsValid(ActiveWeapon) && !GetIsAttacking()) 
	{
		ActiveWeapon->StartAttack();
	}
}

void APlayerCharacter::SwitchToCloseRangeWeapon()
{
	ACloseRangeSystem* CloseRangeWeapon = Cast<ACloseRangeSystem>(CloseRangeSystem->GetChildActor());
	ALongRangeSystem* LongRangeWeapon = Cast<ALongRangeSystem>(LongRangeSystem->GetChildActor());
	if (IsValid(CloseRangeWeapon) && ActiveWeapon != CloseRangeWeapon && !GetIsAttacking())
	{
		ActiveWeapon = CloseRangeWeapon;
		CloseRangeWeapon->WeaponIsActive = true;
		LongRangeWeapon->WeaponIsActive = false;
		CloseRangeSystem->SetVisibility(true);
		LongRangeSystem->SetVisibility(false);
	}
}

void APlayerCharacter::SwitchToLongRangeWeapon()
{
	ACloseRangeSystem* CloseRangeWeapon = Cast<ACloseRangeSystem>(CloseRangeSystem->GetChildActor());
	ALongRangeSystem* LongRangeWeapon = Cast<ALongRangeSystem>(LongRangeSystem->GetChildActor());
	if (IsValid(LongRangeWeapon) && ActiveWeapon != LongRangeWeapon && !GetIsAttacking())
	{
		ActiveWeapon = LongRangeWeapon;
		CloseRangeWeapon->WeaponIsActive = false;
		LongRangeWeapon->WeaponIsActive = true;
		CloseRangeSystem->SetVisibility(false);
		LongRangeSystem->SetVisibility(true);
	}
}

void APlayerCharacter::QuitGame()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->ConsoleCommand("quit");
	}
}

UAnimInstance* APlayerCharacter::GetCustomAnimInstance() const
{
	return GetMesh()->GetAnimInstance();
}

void APlayerCharacter::SetIsAttacking(bool bIsAttacking)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->SetIsAttacking(bIsAttacking);
	}
}

bool APlayerCharacter::GetIsAttacking() const
{
	if (IsValid(AnimInstance))
	{
		return AnimInstance->GetIsAttacking();
	}
	return false;
}

void APlayerCharacter::SetAttackIsCloseRange(bool bIsCloseRange)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->SetAttackIsCloseRange(bIsCloseRange);
	}
}

bool APlayerCharacter::GetAttackIsCloseRange() const
{
	if (IsValid(AnimInstance))
	{
		return AnimInstance->GetAttackIsCloseRange();
	}
	return false;
}

void APlayerCharacter::IncreaseEXP(float IncreaseBy)
{
	CurrentEXP += IncreaseBy;
	if (CurrentEXP >= EXPToLevelUp)
	{
		IncreaseLevel();
		CurrentEXP -= EXPToLevelUp;
		EXPToLevelUp *= 1.25;
	}
}

void APlayerCharacter::IncreaseLevel()
{
	Level++;
}
