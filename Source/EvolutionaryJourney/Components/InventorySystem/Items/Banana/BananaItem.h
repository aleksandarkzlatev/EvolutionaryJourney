// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Components/InventorySystem/BaseItem/BaseItem.h"
#include "BananaItem.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API ABananaItem : public ABaseItem
{
	GENERATED_BODY()
	
public:

	ABananaItem();

	bool ActivateItemEffect(AActor* ItemOwner) override;
};
