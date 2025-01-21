// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DisplayMessage.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UDisplayMessage : public UUserWidget
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayMessage(const FString& Message);
	
};
