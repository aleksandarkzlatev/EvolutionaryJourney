// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/InventorySystem/Items/SpeedPotion/SpeedPotion.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"

ASpeedPotion::ASpeedPotion()
{
	RootComponent = ItemMesh;
}

bool ASpeedPotion::ActivateItemEffect(AActor* ItemOwner)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(ItemOwner))
	{
		Player->SetHasInfiniteStaminaTrue();
		return true;
	}
	return false;
}
