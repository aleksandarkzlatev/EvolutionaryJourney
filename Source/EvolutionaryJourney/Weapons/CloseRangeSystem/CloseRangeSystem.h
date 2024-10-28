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

	UPROPERTY(EditAnywhere, Category = "CloseRangeSystem")
	class ABaseCloseRangeWeapon* Weapon;

	void BeginPlay() override;

	void InitializeWeapon(AActor* InitOwner);

	void StartAttack() override;

	void AttackAnimDelay();

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
