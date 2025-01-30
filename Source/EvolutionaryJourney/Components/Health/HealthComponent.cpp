// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health = 10;
	MaxHealth = 10;
	bCanTakeDamage = true;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UHealthComponent::TakeDamage(AActor* DealtBy, int Damage)	
{
	if (bCanTakeDamage) 
	{
		Health -= Damage;
		bCanTakeDamage = false;
	}

	FTimerHandle InvincibilityDelay;
	GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UHealthComponent::AllowedToTakeDamage, 0.25f, false);

	if (Health <= 0) 
	{
		if (IsValid(DealtBy) && DealtBy->IsA(APlayerCharacter::StaticClass())) 
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DealtBy);
			ABaseEnemy* Enemy = Cast<ABaseEnemy>(GetOwner());
			if (IsValid(PlayerCharacter) && IsValid(Enemy))
			{
				PlayerCharacter->IncreaseEXP(Enemy->DroppedEXP);
				PlayerCharacter->EnemiesKilled++;
			}
		}
		Die();
	}
}

void UHealthComponent::Die()
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GetOwner()))
	{
		Enemy->Death();
	}
	else if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->Death();
	}
}

void UHealthComponent::AllowedToTakeDamage()
{
	bCanTakeDamage = true;
}

float UHealthComponent::GetHealth()
{
	return Health;
}

bool UHealthComponent::RecoverHealth(float addedHealth)
{
	if (Health < MaxHealth)
	{
		if (Health + addedHealth <= MaxHealth)
		{
			Health += addedHealth;
			return true;
		}
		else if (Health + addedHealth > MaxHealth)
		{
			Health = MaxHealth;
			return true;
		}
	}
	return false;
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}
