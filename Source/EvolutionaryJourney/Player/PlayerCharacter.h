#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Components/Weapons/WeaponInterface/WeaponInterface.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class EVOLUTIONARYJOURNEY_API APlayerCharacter : public ACharacter, public IWeaponInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCloseRangeWeaponComponent* CloseRangeWeaponComponent;


	void Move(const FInputActionValue& ActionValue);
	void Look(const FInputActionValue& ActionValue);
	void Jump();

	void SwitchCamera();
	void ResetCameraSwitch();

	void StartSprint();
	void EndSprint();
	void UpdateStamina();

	void StartAttack();
	UAnimInstance* GetCustomAnimInstance() const override;
	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bIsAttacking) override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
