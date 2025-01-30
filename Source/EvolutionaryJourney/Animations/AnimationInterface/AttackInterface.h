// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EVOLUTIONARYJOURNEY_API IAttackInterface
{
	GENERATED_BODY()

	
public:
	virtual UAnimInstance* GetCustomAnimInstance() const = 0;
	virtual void SetIsAttacking(bool bIsAttacking) = 0;
	virtual bool GetIsAttacking() const = 0;
	virtual void SetAttackIsCloseRange(bool bIsAttacking) = 0;
	virtual bool GetAttackIsCloseRange() const = 0;
	virtual void SetIsDead(bool IsDead) = 0;
	virtual bool GetIsDead() const = 0;
};
