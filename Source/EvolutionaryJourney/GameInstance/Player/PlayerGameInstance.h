// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EvolutionaryJourney/Components/InventorySystem/Inventory/InventoryComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/SlotStruct/FSlotStruct.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "PlayerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UPlayerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPlayerGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FSlotStruct> Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expirience")
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expirience")
	float CurrentEXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expirience")
	float EXPToLevelUp;
};
