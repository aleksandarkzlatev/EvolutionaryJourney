// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Components/InventorySystem/BaseItem/BaseItem.h"
#include "SpeedPotion.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API ASpeedPotion : public ABaseItem
{
	GENERATED_BODY()

public:
	ASpeedPotion();

	bool ActivateItemEffect(AActor* ItemOwner) override;
};
