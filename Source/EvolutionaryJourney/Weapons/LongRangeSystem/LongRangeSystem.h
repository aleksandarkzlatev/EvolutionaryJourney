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
	

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseProjectile> ProjectileActor;

	UPROPERTY(VisibleAnywhere)
	class ABaseProjectile* SpawnedProjectile;

	void BeginPlay() override;

	void InitializeWeapon(AActor* InitOwner);

	void StartAttack() override;


	virtual void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
