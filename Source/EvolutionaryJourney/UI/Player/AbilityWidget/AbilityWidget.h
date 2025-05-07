// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite)
	float CooldownDuratation;

	UFUNCTION(BlueprintImplementableEvent)
	void BeginCooldown();
};
