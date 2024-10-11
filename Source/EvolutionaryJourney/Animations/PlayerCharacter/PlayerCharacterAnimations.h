// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "PlayerCharacterAnimations.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API UPlayerCharacterAnimations : public UAnimInstance, public IAttackInterface
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsAttacking;

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetIsAttacking(bool bIsAttacking) override;
	bool GetIsAttacking() const override;
};
