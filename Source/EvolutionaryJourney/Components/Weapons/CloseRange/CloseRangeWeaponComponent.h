// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Components/Weapons/BaseWeapon/BaseWeaponClass.h"
#include "CloseRangeWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UCloseRangeWeaponComponent : public UBaseWeaponClass
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	
public:
	UCloseRangeWeaponComponent();

	void StartAttack() override;

	void LineTrace() override;
};
