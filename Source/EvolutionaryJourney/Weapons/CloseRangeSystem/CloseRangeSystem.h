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

	void BeginPlay() override;

	void InitializeWeapon(AActor* InitOwner);

	void StartAttack() override;

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
