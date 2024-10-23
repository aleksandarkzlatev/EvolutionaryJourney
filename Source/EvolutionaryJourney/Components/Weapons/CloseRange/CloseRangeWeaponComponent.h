// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Components/Weapons/BaseWeaponComponent/BaseWeaponComponent.h"
#include "CloseRangeWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UCloseRangeWeaponComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()

	UCloseRangeWeaponComponent();


protected:
	void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Close Range Weapon")
	class ABaseCloseRangeWeapon* CloseRangeWeapon;

	void StartAttack() override;

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

};

