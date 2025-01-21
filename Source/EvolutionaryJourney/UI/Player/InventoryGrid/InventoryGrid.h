// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	AActor* Owner;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void DisplayInventory();
};
