// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemStruct.generated.h"


USTRUCT(BlueprintType)
struct EVOLUTIONARYJOURNEY_API FItemStruct : public FTableRowBase
{
public:
	GENERATED_BODY()

	FItemStruct();
	~FItemStruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 ItemStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	AActor* ItemClass;

};
