// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Components/Weapons/BaseWeapon/BaseWeaponClass.h"
#include "LongRangeWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API ULongRangeWeaponComponent : public UBaseWeaponClass
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts
	void BeginPlay() override;


public:
	ULongRangeWeaponComponent();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileActor> ProjectileActor;

	void StartAttack() override;
};
