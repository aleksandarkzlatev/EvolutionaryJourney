// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/InventorySystem/BaseItem/BaseItem.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemData/ItemDataComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemStruct/FItemStruct.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(TEXT("ItemDataComponent"));
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ABaseItem::LookAt()
{
	const UDataTable* ItemTable = Cast<UDataTable>(ItemDataComponent->ItemID.DataTable);
	if (IsValid(ItemTable))
	{
		FItemStruct* Item = ItemTable->FindRow<FItemStruct>(ItemDataComponent->ItemID.RowName, TEXT("LookAt"));
		if (Item)
		{
			return *Item->ItemName;
		}
	}
	return "NoName";
}

void ABaseItem::InteractWith(AActor* InventoryOwner)
{
}

bool ABaseItem::ActivateItemEffect(AActor* ItemOwner)
{
	return false;
}

