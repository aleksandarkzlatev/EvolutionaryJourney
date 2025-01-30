// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/InventorySystem/ItemData/ItemDataComponent.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "EvolutionaryJourney/Components/InventorySystem/Inventory/InventoryComponent.h"

// Sets default values for this component's properties
UItemDataComponent::UItemDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Quantity = 0;
}


// Called when the game starts
void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UItemDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FString UItemDataComponent::LookAt()
{
	return FString();
}

void UItemDataComponent::InteractWith(AActor* InventoryOwner)
{
	if(APlayerCharacter* Player = Cast<APlayerCharacter>(InventoryOwner))
	{
		if (UInventoryComponent* InventoryComponent = Player->GetInventoryComponent())
		{
			if (!InventoryComponent->AddToInventory(this))
			{
				GetOwner()->Destroy();
			}
		}
	}
}

