// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimations.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UPlayerCharacterAnimations : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsJumping;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
	bool bIsAttacking;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetIsAttacking(bool bAttacking);

protected:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
