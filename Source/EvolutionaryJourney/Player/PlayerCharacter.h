#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/UI/Player/PlayerHUD/PlayerHUD.h"
#include "EvolutionaryJourney/UI/Player/InventoryGrid/InventoryGrid.h"
#include "InputActionValue.h"
#include "EvolutionaryJourney/Weapons/BaseProjectile/BaseProjectile.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class EVOLUTIONARYJOURNEY_API APlayerCharacter : public ACharacter, public IAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsFirstPerson;
	bool bCanSwitchCamera;
	bool bIsSprinting;
	bool bHasStamina;
	bool bHasInfiniteStamina;
	bool bCanPickupItem;
	bool bCanToggleInventory;
	bool bCanUsePauseMenu;
	bool bInventoryIsOpen;
	bool bCanDash;
	bool bCanLaunchFireball;


	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Movement")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrStamina;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float StaminaDrainTime;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float StaminaRefillTime;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float StaminaDelayBeforeRefill;

	float CurrentRefillDelayTime;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float StaminaDelayBeforeDrain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Expirience")
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Expirience")
	float CurrentEXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Expirience")
	float EXPToLevelUp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float ForwardDashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float UpwardDashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float FireballDamage;

	// Basic component needed for the player vision
	UPROPERTY(EditAnywhere)
	class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;



	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* JumpActiom;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* SwitchCameraAction;
	
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* SwitchToCloseRangeWeaponAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* SwitchToLongRangeWeaponAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* PauseGameAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* ToggleInventoryAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LaunchFireballAction;


	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHUD> PlayerHUDWidgetClass;


	class UPlayerStatBars* PlayerStatBars;
	class UPlayerInventoryMenu* PlayerInventoryMenu;
	class UPauseMenu* PauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInventoryComponent* InventoryComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UChildActorComponent* CloseRangeSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UChildActorComponent* LongRangeSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class ABaseWeaponSystem* ActiveWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UStaticMeshComponent* Quiver;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UPlayerCharacterAnimations* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UNiagaraSystem* DashEffect;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<ABaseProjectile> FireballAbility;

	UPROPERTY(VisibleAnywhere)
	class ABaseProjectile* SpawnedFireball;

	void Move(const FInputActionValue& ActionValue);

	void Look(const FInputActionValue& ActionValue);

	void Jump();

	void SwitchCamera();

	void ResetCameraSwitch();

	void StartSprint();

	void EndSprint();

	void UpdateStamina();

	void StartAttack();

	void SwitchToCloseRangeWeapon();

	void SwitchToLongRangeWeapon();

	void PauseGame();

	void PauseGameDelay();

	void InteractWithItem();

	void InteractWithItemDelay();

	void ToggleInventory();

	void ToggleInventoryDelay();

	void OpenInventory(UInventoryGrid* InventoryGrid);

	void CloseInventory();

	void Dash();

	void DashDelay();

	void LaunchFireball();

	UFUNCTION(BlueprintCallable)
	void CreateFirevall();

	UFUNCTION(BlueprintCallable)
	void EndFireballLaunch();

	void LaunchFireballDelay();

	UFUNCTION()
	void FireballBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnFireballHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyFailedFireball();

	UAnimInstance* GetCustomAnimInstance() const;

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bIsAttacking);

	bool GetIsAttacking() const;

	void SetAttackIsCloseRange(bool bIsCloseRange);

	bool GetAttackIsCloseRange() const;

	void SetIsDead(bool bIsDead);

	void SetIsUsingMagic(bool IsUsingMagic);

	bool GetIsUsingMagic() const;

	void IncreaseLevel();

	void DeathDelay();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	int32 EnemiesKilled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 EnemiesToKillForFirstLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UPlayerHUD* PlayerHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UEnemiesToDefeat* EnemiesToDefeat;


	bool GetIsFirstPerson();

	void SetHasInfiniteStaminaTrue();

	void SetHasInfiniteStaminaTrueDelay();

	UFUNCTION(BlueprintCallable)
	bool GetIsInventoryOpen() const;

	UInventoryComponent* GetInventoryComponent() const;

	UHealthComponent* GetHealthComponent() const;

	void UpdateInventory();

	void IncreaseEXP(float IncreaseBy);

	bool GetIsDead() const;

	void Death();
};
