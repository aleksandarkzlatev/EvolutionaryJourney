// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/InventorySystem/Items/Banana/BananaItem.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"

ABananaItem::ABananaItem()
{
	RootComponent = ItemMesh;
}

bool ABananaItem::ActivateItemEffect(AActor* ItemOwner)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ItemOwner))
	{
		if (UHealthComponent* HealthComponent = PlayerCharacter->FindComponentByClass<UHealthComponent>())
		{
			return HealthComponent->RecoverHealth(3);
		}
	}
	return false;
}
