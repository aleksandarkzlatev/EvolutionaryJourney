// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/InventorySystem/Inventory/InventoryComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/SlotStruct/FSlotStruct.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EvolutionaryJourney/Components/InventorySystem/InteractInterface/InteractInterface.h"
#include "EvolutionaryJourney/Components/InventorySystem/BaseItem/BaseItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractionRange = 500;
	TraceRadius = 25;
	LookAtActor = nullptr;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	Content.Init(FSlotStruct(), InventorySize);

	if (IsValid(DisplayMessageClass))
	{
		DisplayMessage = CreateWidget<UDisplayMessage>(GetWorld(), DisplayMessageClass);
		if (IsValid(DisplayMessage))
		{
			DisplayMessage->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DisplayMessageClass is not valid"));
	}

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractionTrace();
}

int32 UInventoryComponent::AddToInventory(UItemDataComponent* ItemData)
{
	FString ItemID = ItemData->ItemID.RowName.ToString();
	int32 QuantityRemaining = ItemData->Quantity;
	bool HasFailed = false;
	int32 index = 0;
	while (QuantityRemaining > 0 && !HasFailed)
	{
		index = FindSlot(ItemID);
		if (index == -1)
		{
			if (FindEmptySlot() != -1)
			{
				if (CreateNewStack(ItemID, 1, ItemData->ItemTable, ItemData->ItemID.RowName)) 
				{
					QuantityRemaining--;
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
			QuantityRemaining--;
		}
	}

	return QuantityRemaining;
}

int32 UInventoryComponent::FindSlot(FString ItemID)
{
	int32 index = 0;
	for (FSlotStruct Slot : Content)
	{
		if (Slot.ItemID == ItemID)
		{
			if (Slot.Quantity < GetMaxStackSizes(ItemID))
			{
				return index;
			}
		}
		index++;
	}
	return -1;
}

int32 UInventoryComponent::GetMaxStackSizes(FString ItemID)
{
	if (FItemStruct* ItemData = GetItemData(ItemID))
	{
		return ItemData->ItemStackSize;
	}
	return -1;
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

bool UInventoryComponent::CreateNewStack(FString ItemID, int32 Quantity, UDataTable* ItemTable, FName ItemRowName)
{
	int32 EmptySlot = FindEmptySlot();
	if (EmptySlot != -1)
	{
		Content[EmptySlot].ItemID = ItemID;
		Content[EmptySlot].Quantity = Quantity;
		Content[EmptySlot].ItemTable = ItemTable;
		Content[EmptySlot].ItemRowName = ItemRowName;
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

void UInventoryComponent::RemoveFromInventory(int32 index, bool RemoveWholeStack, bool IsConsumed)
{
	FString ItemID = Content[index].ItemID;
	int32 Quantity = Content[index].Quantity;

	if (RemoveWholeStack || Quantity == 1) 
	{
		if (IsConsumed) 
		{
			if(ConsumeItem(index))
			{
				Content[index].ItemID = "";
				Content[index].Quantity = 0;
			}
		}
		else
		{
			if (Quantity > 0) DropItem(ItemID, Quantity);
			Content[index].ItemID = "";
			Content[index].Quantity = 0;
		}
	}
	else
	{
		if (IsConsumed) 
		{
			if (ConsumeItem(index))
			{
				Content[index].Quantity--;
			}
		}
		else
		{
			if (Quantity > 0)
			{
				DropItem(ItemID, 1);
				Content[index].Quantity--;
			}
		}
		if (Quantity == 0) Content[index].ItemID = "";
	}
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
	{
		PlayerCharacter->UpdateInventory();
	}
}

void UInventoryComponent::DropItem(FString ItemID, int32 Quantity)
{
    for (; Quantity > 0; Quantity--)
    {
        FItemStruct* ItemData = GetItemData(ItemID);
        if (ItemData) 
        {
            TSubclassOf<AActor> ItemClass = ItemData->ItemClass;
            GetWorld()->SpawnActor<AActor>(ItemClass, GetDropLocation(), FRotator::ZeroRotator);
        }
    }
}

bool UInventoryComponent::ConsumeItem(int32 index)
{
	if(FItemStruct* ItemData = GetItemData(Content[index].ItemID))
	{
		if(TSubclassOf<AActor> ItemClass = ItemData->ItemClass)
		{
			if (AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClass))
			{
				if (ABaseItem* UsableItem = Cast<ABaseItem>(SpawnedItem))
				{
					SpawnedItem->Destroy();
					return UsableItem->ActivateItemEffect(Owner);
				}
				SpawnedItem->Destroy();
			}
		}
	}
	return false;
}

FItemStruct* UInventoryComponent::GetItemData(FString ItemID)
{
	int32 index = 0;
	for (FSlotStruct Slot : Content)
	{
		if (Slot.ItemID == ItemID)
		{
			if (const UDataTable* ItemTable = Cast<UDataTable>(Content[index].ItemTable))
			{
				FItemStruct* Item = ItemTable->FindRow<FItemStruct>(Content[index].ItemRowName, TEXT("Item"));
				if (Item)
				{
					return Item;
				}
			}
		}
		index++;
	}
	return nullptr;
}

FVector UInventoryComponent::GetDropLocation()
{
	FVector ActorLocation = Owner->GetActorLocation();
	FVector ForwardVector = Owner->GetActorForwardVector();

	float ConeHalfAngleInDegrees = 60.0f;
	FVector RandomDirection = FMath::VRandCone(ForwardVector, FMath::DegreesToRadians(ConeHalfAngleInDegrees));

	FVector Start = ActorLocation + ForwardVector * 100.0f; 
	FVector Offset(0.0f, 0.0f, 80.0f);               
	FVector End = Start + RandomDirection * 300.0f + Offset;

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	TraceParams.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End - FVector(0.0f, 0.0f, 500.0f),
		ECC_Visibility, 
		TraceParams
	);

	if (bHit)
	{
		return HitResult.Location;
	}

	return FVector::ZeroVector;
}

void UInventoryComponent::TransferSlots(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex)
{
	FSlotStruct TempSlot = SourceInventory->Content[SourceIndex];
	if (DestinationIndex >= 0 && SourceIndex != DestinationIndex)
	{
		if (SourceInventory->Content[SourceIndex].ItemID == SourceInventory->Content[DestinationIndex].ItemID)
		{
			FString ItemID = SourceInventory->Content[SourceIndex].ItemID;
			while (SourceInventory->Content[SourceIndex].Quantity > 0 && SourceInventory->Content[DestinationIndex].Quantity < GetMaxStackSizes(ItemID))
			{
				SourceInventory->Content[SourceIndex].Quantity--;
				SourceInventory->Content[DestinationIndex].Quantity++;
			}
			if (SourceInventory->Content[SourceIndex].Quantity == 0)
			{
				SourceInventory->Content[SourceIndex].ItemID = "";
			}
		}
		else 
		{
			SourceInventory->Content[SourceIndex] = SourceInventory->Content[DestinationIndex];
			SourceInventory->Content[DestinationIndex] = TempSlot;
		}
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->UpdateInventory();
		}
	}
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
		if (ABaseItem* Item = Cast<ABaseItem>(HitResult.GetActor()))
		{
			FString LookAtString = Item->LookAt();
			FString Message = FString::Printf(TEXT("Pick up: %s"), *LookAtString);
			DisplayMessage->DisplayMessage(Message);
		}
		else
		{
			DisplayMessage->DisplayMessage("");
		}
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
		if (ABaseItem* Item = Cast<ABaseItem>(LookAtActor))
		{
			if (UItemDataComponent* ItemDataComponent = Cast<UItemDataComponent>(Item->ItemDataComponent))
			{
				ItemDataComponent->InteractWith(Owner);
			}
		}
	}
}

