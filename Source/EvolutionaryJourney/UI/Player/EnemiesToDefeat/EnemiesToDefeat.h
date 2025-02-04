// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemiesToDefeat.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UEnemiesToDefeat : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	class APlayerCharacter* Owner;
};
