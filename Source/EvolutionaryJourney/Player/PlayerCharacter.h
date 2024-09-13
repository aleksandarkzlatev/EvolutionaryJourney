#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class EVOLUTIONARYJOURNEY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Basic component needed for the player vision
	UPROPERTY(EditAnywhere)
	class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* FirstPersonCamera;

	bool bIsFirstPerson;
	bool bCanSwitchCamera;

	FTimerHandle SwitchCameraTimerHandle;

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

	void Move(const FInputActionValue& ActionValue);
	void Look(const FInputActionValue& ActionValue);
	void Jump();
	void SwitchCamera();
	void ResetCameraSwitch();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
