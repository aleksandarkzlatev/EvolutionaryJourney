// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/InventorySystem/Inventory/InventoryComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/SlotStruct/FSlotStruct.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EvolutionaryJourney/Components/InventorySystem/InteractInterface/InteractInterface.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemData/ItemDataComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/ExampleItem/ExampleItem.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemStruct/FItemStruct.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractionRange = 200;
	TraceRadius = 15;
	LookAtActor = nullptr;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	Content.Init(FSlotStruct(), InventorySize);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractionTrace();
}

int32 UInventoryComponent::AddToInventory(FString ItemID, int32 Quantity)
{
	int32 QunatityRemainig = Quantity;
	bool HasFailed = false;
	int32 index = 0;
	while (QunatityRemainig > 0 && !HasFailed)
	{
		index = FindSlot(ItemID);
		if (index == -1)
		{
			if (FindEmptySlot() != -1)
			{
				if (CreateNewStack(ItemID, 1)) 
				{
					QunatityRemainig--;
				}
				else
				{
					HasFailed = true;
				}
			}
			else
			{
				HasFailed = true;
			}
		}
		else
		{
			AddToStack(index, 1);
			QunatityRemainig--;
		}
	}

	return QunatityRemainig;
}

int32 UInventoryComponent::FindSlot(FString ItemID)
{
	int32 index = 0;
	for (FSlotStruct Slot : Content)
	{
		if (Slot.ItemID == ItemID)
		{
			if (AExampleItem* Item = Cast<AExampleItem>(LookAtActor))
			{
				if (UItemDataComponent* ItemDataComponent = Cast<UItemDataComponent>(Item->ItemDataComponent))
				{
					if (Slot.Quantity < GetMaxStackSizes(ItemDataComponent->ItemID))
					{
						return index;
					}
				}
			}
		}
		index++;
	}
	return -1;
}

int32 UInventoryComponent::GetMaxStackSizes(FDataTableRowHandle ItemID)
{
	if (const UDataTable* ItemTable = Cast<UDataTable>(ItemID.DataTable))
	{
		FItemStruct* Item = ItemTable->FindRow<FItemStruct>(ItemID.RowName, TEXT("Item"));
		if (Item)
		{
			return Item->ItemStackSize;
		}
		else return -1;
	}
	else return -1;
}

void UInventoryComponent::AddToStack(int32 index, int32 Quantity)
{
	Content[index].Quantity += Quantity;
}

int32 UInventoryComponent::FindEmptySlot()
{
	int32 index = 0;
	for (FSlotStruct Slot : Content)
	{
		if (Slot.Quantity == 0)
		{
			return index;
		}
		index++;
	}
	return -1;
}

bool UInventoryComponent::CreateNewStack(FString ItemID, int32 Quantity)
{
	int32 EmptySlot = FindEmptySlot();
	if (EmptySlot != -1)
	{
		Content[EmptySlot].ItemID = ItemID;
		Content[EmptySlot].Quantity = Quantity;
		return true;
	}
	return false;
}

void UInventoryComponent::Debug_PrintInventory()
{
	int32 index = 0;
	for (FSlotStruct Slot : Content)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Slot %d: %s, %d"), index, *Slot.ItemID, Slot.Quantity));
		index++;
	}
}

void UInventoryComponent::RemoveFromInventory(AActor* ItemToRemove)
{
}

void UInventoryComponent::InteractionTrace()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner);
	APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());

	FVector CameraLocation;
	FRotator CameraRotation;

	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector Start = Owner->GetActorLocation();
	const FVector End = Start + CameraRotation.Vector() * InteractionRange;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(Owner);

	FHitResult HitResult;

	const bool Hit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_EngineTraceChannel1),
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (Hit) 
	{
		if (HitResult.GetActor() != LookAtActor) 
		{
			LookAtActor = HitResult.GetActor();
		}
	}
	else 
	{
		LookAtActor = nullptr;
	}
}

void UInventoryComponent::InteractionWith()
{
	if (IsValid(LookAtActor))
	{
		if (AExampleItem* Item = Cast<AExampleItem>(LookAtActor))
		{
			if (UItemDataComponent* ItemDataComponent = Cast<UItemDataComponent>(Item->ItemDataComponent))
			{
				ItemDataComponent->InteractWith(Owner);
			}
		}
	}
}

