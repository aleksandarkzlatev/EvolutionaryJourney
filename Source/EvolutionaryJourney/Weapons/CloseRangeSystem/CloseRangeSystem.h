// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Weapons/BaseWeaponSystem/BaseWeaponSystem.h"
#include "CloseRangeSystem.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API ACloseRangeSystem : public ABaseWeaponSystem
{
	GENERATED_BODY()

	ACloseRangeSystem();

public:

	UPROPERTY(EditAnywhere)
	int Damage;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CollisionComponent;

	void BeginPlay() override;

	void InitializeWeapon(AActor* InitOwner);

	void StartAttack() override;

	void EndAttack() override;

	void EndAttackDelay();

	UFUNCTION(BlueprintCallable)
	void TurnCollisionOn();

	UFUNCTION(BlueprintCallable)
	void TurnCollisionOff();

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
