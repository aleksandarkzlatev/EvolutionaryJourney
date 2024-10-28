// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Weapons/BaseWeaponSystem/BaseWeaponSystem.h"
#include "LongRangeSystem.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API ALongRangeSystem : public ABaseWeaponSystem
{
	GENERATED_BODY()

	ALongRangeSystem();
	

	UPROPERTY(VisibleAnywhere)
	class ABaseProjectile* SpawnedProjectile;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseProjectile> ProjectileActor;

	void BeginPlay() override;

	void InitializeWeapon(AActor* InitOwner);

	void StartAttack() override;

	void SpawnProjectile();

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
