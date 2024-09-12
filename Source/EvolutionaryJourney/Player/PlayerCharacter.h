#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	class UCameraComponent* Camera;

	void MoveForward(float InputNumber);
	void MoveRight(float InputNumber);

	void Turn(float InputNumber);
	void LookUp(float InputNumber);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
