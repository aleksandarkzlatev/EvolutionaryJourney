// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EVOLUTIONARYJOURNEY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth;

	bool bCanTakeDamage;

	void Die();

	void AllowedToTakeDamage();

public:

	void TakeDamage(AActor* DealtBy, int Damage);

	float GetHealth();

	bool RecoverHealth(float addedHealth);

	float GetMaxHealth();
};

