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
#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "EvolutionaryJourney/UI/Player/PlayerInventoryMenu/PlayerInventoryMenu.h"
#include "EvolutionaryJourney/UI/Player/PlayerStatBars/PlayerStatBars.h"
#include "EvolutionaryJourney/UI/Player/PauseMenu/PauseMenu.h"
#include "EvolutionaryJourney/UI/Player/EnemiesToDefeat/EnemiesToDefeat.h"
#include <Kismet/GameplayStatics.h>
#include "EvolutionaryJourney/GameInstance/Player/PlayerGameInstance.h"
#include "../../../../../EpicGames/UnrealEngine/UE_5.5/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"



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

	EnemiesKilled = 0;
	MaxWalkSpeed = 500;
	MaxSprintSpeed = 800;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	MaxStamina = 100;
	CurrStamina = MaxStamina;
	StaminaDelayBeforeDrain = 0.015;
	Level = 0;
	CurrentEXP = 0;
	EXPToLevelUp = 10.0;
	bCanPickupItem = true;
	bCanToggleInventory = true;
	bCanUsePauseMenu = true;
	bInventoryIsOpen = false;
	bCanDash = true;
	ForwardDashSpeed = 1000;
	UpwardDashSpeed = 500;
	bCanLaunchFireball = true;
	FireballDamage = 10;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UPlayerGameInstance* GameInstance = Cast<UPlayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GameInstance))
	{
		if (GameInstance->Health != 0) HealthComponent->SetHealth(GameInstance->Health);
		if (GameInstance->MaxHealth != 0) HealthComponent->SetMaxHealth(GameInstance->MaxHealth);
		if (!GameInstance->Content.IsEmpty()) InventoryComponent->Content = GameInstance->Content;
		if (GameInstance->Level != 0) Level = GameInstance->Level;
		if (GameInstance->CurrentEXP != 0) CurrentEXP = GameInstance->CurrentEXP;
		if (GameInstance->EXPToLevelUp != 0) EXPToLevelUp = GameInstance->EXPToLevelUp;
	}
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

	if (IsValid(PlayerHUDWidgetClass))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
			PlayerStatBars = Cast<UPlayerStatBars>(PlayerHUDWidget->GetWidgetFromName("WB_PlayerStatBars"));
			if (IsValid(PlayerStatBars))
			{
				PlayerStatBars->SetVisibility(ESlateVisibility::Visible);
			}
			PlayerInventoryMenu = Cast<UPlayerInventoryMenu>(PlayerHUDWidget->GetWidgetFromName("WB_PlayerInventoryMenu"));
			if (IsValid(PlayerInventoryMenu))
			{
				PlayerInventoryMenu->SetVisibility(ESlateVisibility::Hidden);
			}
			PauseMenu = Cast<UPauseMenu>(PlayerHUDWidget->GetWidgetFromName("WB_PauseMenu"));
			if (IsValid(PauseMenu))
			{
				PauseMenu->SetVisibility(ESlateVisibility::Hidden);
				PauseMenu->bInventoryIsOpen = false;
				PauseMenu->Owner = this;
			}
			EnemiesToDefeat = Cast<UEnemiesToDefeat>(PlayerHUDWidget->GetWidgetFromName("WB_EnemiesToDefeat"));
			if (IsValid(EnemiesToDefeat))
			{
				EnemiesToDefeat->SetVisibility(ESlateVisibility::Hidden);
				EnemiesToDefeat->Owner = this;
			}
		}
	}

	AnimInstance = Cast<UPlayerCharacterAnimations>(GetCustomAnimInstance());

	FTimerHandle StaminaTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &APlayerCharacter::UpdateStamina, StaminaDelayBeforeDrain, true);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		Input->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PauseGame);
		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InteractWithItem);
		Input->BindAction(ToggleInventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleInventory);
		Input->BindAction(DashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dash);
		Input->BindAction(LaunchFireballAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LaunchFireball);
	}


}

void APlayerCharacter::Move(const FInputActionValue& InputNumber)
{
	FVector2D InputVector = InputNumber.Get<FVector2D>();

	if (IsValid(Controller) && !GetIsAttacking() && !GetIsDead() && !GetIsUsingMagic()) 
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

	if (IsValid(Controller) && !GetIsDead()) 
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void APlayerCharacter::Jump()
{
	if (!GetIsAttacking() && !GetIsDead() && !GetIsUsingMagic()) ACharacter::Jump();
}

bool APlayerCharacter::GetIsFirstPerson()
{
	return bIsFirstPerson;
}

void APlayerCharacter::SetHasInfiniteStaminaTrue()
{
	bHasInfiniteStamina = true;
	FTimerHandle InfiniteStaminaTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(InfiniteStaminaTimerHandle, this, &APlayerCharacter::SetHasInfiniteStaminaTrueDelay, 5.0f, false);
}

void APlayerCharacter::SetHasInfiniteStaminaTrueDelay()
{
	bHasInfiniteStamina = false;
}

bool APlayerCharacter::GetIsInventoryOpen() const
{
	return bInventoryIsOpen;
}

UInventoryComponent* APlayerCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UHealthComponent* APlayerCharacter::GetHealthComponent() const
{
	return HealthComponent;
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
	GetWorld()->GetTimerManager().SetTimer(SwitchCameraTimerHandle, this, &APlayerCharacter::ResetCameraSwitch, 0.25f, false);
}

void APlayerCharacter::ResetCameraSwitch()
{
	bCanSwitchCamera = true;
}

void APlayerCharacter::StartSprint()
{
	if (bHasStamina && !GetIsAttacking() && !GetIsDead() && !GetIsUsingMagic())
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;

		// In the case of the sprint button being held while the player is not moving
		if (GetVelocity().Size() >= 0.5 && !bHasInfiniteStamina)
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
	if (IsValid(ActiveWeapon) && !GetIsAttacking() && !GetIsDead() && !GetIsUsingMagic())
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

void APlayerCharacter::PauseGame()
{
	if (bCanUsePauseMenu)
	{
		if (IsValid(PlayerHUDWidget))
		{
			PauseMenu->bInventoryIsOpen = bInventoryIsOpen;
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			if (PauseMenu->GetVisibility() == ESlateVisibility::Hidden)
			{
				PauseMenu->SetVisibility(ESlateVisibility::Visible);
				if (PlayerController)
				{
					FInputModeGameAndUI InputMode;
					PauseMenu->SetIsFocusable(true);
					InputMode.SetWidgetToFocus(PauseMenu->TakeWidget());
					InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
					PlayerController->SetInputMode(InputMode);
					PlayerController->bShowMouseCursor = true;
					PlayerController->SetPause(true);
				}
			}
		}
	}
	bCanUsePauseMenu = false;
	FTimerHandle PauseGameTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(PauseGameTimerHandle, this, &APlayerCharacter::PauseGameDelay, 0.5f, false);
}

void APlayerCharacter::PauseGameDelay()
{
	bCanUsePauseMenu = true;
}

void APlayerCharacter::InteractWithItem()
{
	if (!GetIsAttacking() && bCanPickupItem) 
	{
		InventoryComponent->InteractionWith();
		UpdateInventory();
		bCanPickupItem = false;
		FTimerHandle InteractDelayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(InteractDelayTimerHandle, this, &APlayerCharacter::InteractWithItemDelay, 0.2f, false);
	}
}

void APlayerCharacter::InteractWithItemDelay()
{
	bCanPickupItem = true;
}

void APlayerCharacter::ToggleInventory()
{
	if (IsValid(PlayerHUDWidget) && bCanToggleInventory)
	{
		if (IsValid(PlayerInventoryMenu))
		{
			UInventoryGrid* InventoryGrid = Cast<UInventoryGrid>(PlayerInventoryMenu->GetWidgetFromName(TEXT("WB_InventoryGrid")));
			if (!InventoryGrid->Owner)
			{
				InventoryGrid->Owner = this;
			}
			if (PlayerInventoryMenu->GetVisibility() == ESlateVisibility::Hidden)
			{
				bInventoryIsOpen = true;
				PlayerInventoryMenu->SetVisibility(ESlateVisibility::Visible);
				UpdateInventory();
				OpenInventory(InventoryGrid);
			}
			else
			{
				bInventoryIsOpen = false;
				PlayerInventoryMenu->SetVisibility(ESlateVisibility::Hidden);
				CloseInventory();
			}
		}
		bCanToggleInventory = false;
		FTimerHandle ToggleInventoryTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ToggleInventoryTimerHandle, this, &APlayerCharacter::ToggleInventoryDelay, 0.5f, false);
	}
}

void APlayerCharacter::ToggleInventoryDelay()
{
	bCanToggleInventory = true;
}

void APlayerCharacter::UpdateInventory()
{
	if (IsValid(PlayerHUDWidget) && bCanToggleInventory)
	{
		if (IsValid(PlayerInventoryMenu))
		{
			UInventoryGrid* InventoryGrid = Cast<UInventoryGrid>(PlayerInventoryMenu->GetWidgetFromName(TEXT("WB_InventoryGrid")));
			if (IsValid(InventoryGrid))
			{
				InventoryGrid->DisplayInventory();
			}
		}
	}
}

void APlayerCharacter::OpenInventory(UInventoryGrid* InventoryGrid)
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryGrid->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;

		InventoryGrid->SetKeyboardFocus();
	}
}

void APlayerCharacter::CloseInventory()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

	void APlayerCharacter::Dash()
	{
		if (bCanDash && !GetIsAttacking() && !GetIsDead())
		{
			FVector MovementDirection = GetCharacterMovement()->GetLastInputVector();

			if (MovementDirection.IsNearlyZero())
			{
				MovementDirection = GetActorForwardVector();
			}
			MovementDirection.Normalize();

			FVector DashVelocity = MovementDirection * ForwardDashSpeed + GetActorUpVector() * UpwardDashSpeed;

			LaunchCharacter(DashVelocity, true, true);
			
			if (DashEffect)
			{
				UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
					DashEffect,
					GetMesh(),
					TEXT("DashSocket"),
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::SnapToTarget,
					true
				);
			}
			bCanDash = false;
			FTimerHandle DashTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &APlayerCharacter::DashDelay,10.0f, false);
		}
	}

void APlayerCharacter::DashDelay()
{
	bCanDash = true;
}

void APlayerCharacter::LaunchFireball()
{
	if (bCanLaunchFireball && !GetIsAttacking() && !GetIsDead() && !GetIsUsingMagic())
	{
		if (IsValid(FireballAbility) && IsValid(AnimInstance))
		{
			AnimInstance->SetIsUsingMagic(true);
		}
	}
}

void APlayerCharacter::CreateFirevall()
{
	FVector OwnerLocation = GetActorLocation();
	FRotator OwnerRotation = GetActorRotation();
	FVector ForwardVector = GetActorForwardVector();
	float SpawnDistance = 25.0f;
	FVector SpawnLocation = OwnerLocation + (ForwardVector * SpawnDistance);
	SpawnedFireball = GetWorld()->SpawnActor<ABaseProjectile>(FireballAbility, SpawnLocation, OwnerRotation);
	SpawnedFireball->OnActorBeginOverlap.AddDynamic(this, &APlayerCharacter::FireballBeginOverlap);
	SpawnedFireball->OnActorHit.AddDynamic(this, &APlayerCharacter::OnFireballHit);
	bCanLaunchFireball = false;
	FTimerHandle FireballTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FireballTimerHandle, this, &APlayerCharacter::LaunchFireballDelay, 15.0f, false);
}

void APlayerCharacter::EndFireballLaunch()
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->SetIsUsingMagic(false);
	}
}

void APlayerCharacter::LaunchFireballDelay()
{
	bCanLaunchFireball = true;
	if (IsValid(SpawnedFireball))
	{
		SpawnedFireball->Destroy();
		SpawnedFireball = nullptr;
	}
}

void APlayerCharacter::FireballBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
	if (IsValid(Enemy))
	{
		UHealthComponent* EnemyHealthComponent = Enemy->FindComponentByClass<UHealthComponent>();
		if (IsValid(EnemyHealthComponent))
		{
			EnemyHealthComponent->TakeDamage(this, FireballDamage);
			if (IsValid(SpawnedFireball))
			{
				SpawnedFireball->Destroy();
				SpawnedFireball = nullptr;
			}
		}
	}
}

void APlayerCharacter::OnFireballHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FTimerHandle FireballTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FireballTimerHandle, this, &APlayerCharacter::DestroyFailedFireball, 3.0f, false);
}

void APlayerCharacter::DestroyFailedFireball()
{
	if (IsValid(SpawnedFireball))
	{
		SpawnedFireball->Destroy();
		SpawnedFireball = nullptr;
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

void APlayerCharacter::SetIsDead(bool bIsDead)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->SetIsDead(bIsDead);
	}
}

void APlayerCharacter::SetIsUsingMagic(bool IsUsingMagic)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->SetIsUsingMagic(IsUsingMagic);
	}
}

bool APlayerCharacter::GetIsUsingMagic() const
{
	if (IsValid(AnimInstance))
	{
		return AnimInstance->GetIsUsingMagic();
	}
	return false;
}

bool APlayerCharacter::GetIsDead() const
{
	if (IsValid(AnimInstance))
	{
		return AnimInstance->GetIsDead();
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

void APlayerCharacter::Death()
{
	AnimInstance->SetIsDead(true);

	FTimerHandle DeathTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &APlayerCharacter::DeathDelay, 3.7f, false);
}

void APlayerCharacter::DeathDelay()
{
	ACloseRangeSystem* CloseRangeWeapon = Cast<ACloseRangeSystem>(CloseRangeSystem->GetChildActor());
	ALongRangeSystem* LongRangeWeapon = Cast<ALongRangeSystem>(LongRangeSystem->GetChildActor());

	if (IsValid(CloseRangeWeapon)) CloseRangeWeapon->Destroy();
	if (IsValid(LongRangeWeapon)) LongRangeWeapon->Destroy();
}

void APlayerCharacter::IncreaseLevel()
{
	Level++;
}
