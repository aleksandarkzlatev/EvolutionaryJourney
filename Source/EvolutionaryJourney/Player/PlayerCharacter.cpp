#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Components/Weapons/BaseWeapon/BaseWeaponClass.h"
#include "EvolutionaryJourney/Components/Weapons/CloseRange/CloseRangeWeaponComponent.h"
#include "EvolutionaryJourney/Components/Weapons/LongRange/LongRangeWeaponComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/Inventory/InventoryComponent.h"
#include "EvolutionaryJourney/Animations/PlayerCharacter/PlayerCharacterAnimations.h"

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

	CloseRangeWeaponComponent = CreateDefaultSubobject<UCloseRangeWeaponComponent>(TEXT("Close Range Weapon System"));
	CloseRangeWeaponComponent->WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	CloseRangeWeaponComponent->WeaponMesh->SetupAttachment(GetMesh(), TEXT("SwordSocket"));
	CloseRangeWeaponComponent->CustomAnimInstance = GetMesh()->GetAnimInstance();
	ActiveWeapon = CloseRangeWeaponComponent;


	LongRangeWeaponComponent = CreateDefaultSubobject<ULongRangeWeaponComponent>(TEXT("Long Range Weapon System"));

	InventoryComponent->CloseRangeWeapon = CloseRangeWeaponComponent;

	MaxWalkSpeed = 500;
	MaxSprintSpeed = 800;

	static ConstructorHelpers::FObjectFinder<UClass> AnimBPClass(TEXT("Class'/Game/PathToYourAnimBP.YourAnimBP_C'"));
	if (AnimBPClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBPClass.Object);
	}

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	if (APlayerController* PlayerController = Cast < APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		Input->BindAction(JumpActiom, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		Input->BindAction(SwitchCameraAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchCamera);
		Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndSprint);
		Input->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::StartAttack);
		Input->BindAction(SwitchToCloseRangeWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchToCloseRangeWeapon);
		Input->BindAction(SwitchToLongRangeWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchToLongRangeWeapon);
	}


}

void APlayerCharacter::Move(const FInputActionValue& InputNumber)
{
	FVector2D InputVector = InputNumber.Get<FVector2D>();

	if (IsValid(Controller)) {

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

	if (IsValid(Controller)) {
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void APlayerCharacter::Jump()
{
	ACharacter::Jump();
}

void APlayerCharacter::SwitchCamera()
{
	if (!bCanSwitchCamera) {
		return;
	}

	if (bIsFirstPerson) {
		FirstPersonCamera->SetActive(true);
		ThirdPersonCamera->SetActive(false);
	}
	else {
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
	if (bHasStamina) {
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;

		// In the case of the sprint button being held while the player is not moving
		if (GetVelocity().Size() >= 0.5) {
			bIsSprinting = true;
		}
		else {
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
	if (bIsSprinting) {
		CurrStamina -= StaminaDrainTime;
		CurrentRefillDelayTime = StaminaDelayBeforeRefill;
	}
	else if (!bIsSprinting && CurrStamina < MaxStamina) {
		CurrentRefillDelayTime--;
		if (CurrentRefillDelayTime <= 0) {
			CurrStamina += StaminaRefillTime;
		}
	}

	if (CurrStamina <= 0) {
		bHasStamina = false;
		EndSprint();
	}
	else {
		bHasStamina = true;
	}
}

void APlayerCharacter::StartAttack()
{
	if (IsValid(ActiveWeapon)) {
		ActiveWeapon->StartAttack();
	}

	else {
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter: ActiveWeapon is not valid"));
	}
}

void APlayerCharacter::SwitchToCloseRangeWeapon()
{
	if (IsValid(CloseRangeWeaponComponent) && ActiveWeapon != CloseRangeWeaponComponent)
	{
		ActiveWeapon = CloseRangeWeaponComponent;
		CloseRangeWeaponComponent->WeaponMesh->SetVisibility(true);
	}
}

void APlayerCharacter::SwitchToLongRangeWeapon()
{
	if (IsValid(LongRangeWeaponComponent) && ActiveWeapon != LongRangeWeaponComponent)
	{
		ActiveWeapon = LongRangeWeaponComponent;
		CloseRangeWeaponComponent->WeaponMesh->SetVisibility(false);
	}
}

UAnimInstance* APlayerCharacter::GetCustomAnimInstance() const
{
	return GetMesh()->GetAnimInstance();
}

void APlayerCharacter::SetIsAttacking(bool bIsAttacking)
{
	UPlayerCharacterAnimations* AnimInstance = Cast<UPlayerCharacterAnimations>(GetCustomAnimInstance());
	if (IsValid(AnimInstance))
	{
		AnimInstance->bIsAttacking = bIsAttacking;
	}
}
