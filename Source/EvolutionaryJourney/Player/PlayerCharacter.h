#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "InputActionValue.h"
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



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInventoryComponent* InventoryComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class ACloseRangeSystem* CloseRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class ALongRangeSystem* LongRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class ABaseWeaponSystem* ActiveWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UStaticMeshComponent* Quiver;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UChildActorComponent* CloseRangeWeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UChildActorComponent* LongRangeWeaponMesh;


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
	UAnimInstance* GetCustomAnimInstance() const;
	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bIsAttacking);
	bool GetIsAttacking() const;
	void SetAttackIsCloseRange(bool bIsCloseRange);
	bool GetAttackIsCloseRange() const;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
