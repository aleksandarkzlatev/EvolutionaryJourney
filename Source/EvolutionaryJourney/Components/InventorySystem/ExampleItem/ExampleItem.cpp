// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/InventorySystem/ExampleItem/ExampleItem.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemData/ItemDataComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemStruct/FItemStruct.h"

// Sets default values
AExampleItem::AExampleItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(TEXT("ItemDataComponent"));
}

// Called when the game starts or when spawned
void AExampleItem::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AExampleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AExampleItem::LookAt()
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

void AExampleItem::InteractWith(AActor* InventoryOwner)
{
}

