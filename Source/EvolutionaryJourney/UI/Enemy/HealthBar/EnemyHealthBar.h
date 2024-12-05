// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class EVOLUTIONARYJOURNEY_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseWidget")
    class ABaseEnemy* Owner;

    UFUNCTION(BlueprintCallable, Category = "BaseWidget")
    void SetOwnerEnemy(ABaseEnemy* NewOwner);

protected:
    virtual void NativeConstruct() override;

};
