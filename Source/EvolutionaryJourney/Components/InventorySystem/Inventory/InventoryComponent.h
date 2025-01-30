// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/SlotStruct/FSlotStruct.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemData/ItemDataComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/ItemStruct/FItemStruct.h"
#include "EvolutionaryJourney/UI/Player/DisplayMessage/DisplayMessage.h"
#include "InventoryComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVOLUTIONARYJOURNEY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AActor* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FSlotStruct> Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float InteractionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	class AActor* LookAtActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UDisplayMessage> DisplayMessageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDisplayMessage* DisplayMessage;

	int32 AddToInventory(UItemDataComponent* ItemData);

	int32 FindSlot(FString ItemID);

	int32 GetMaxStackSizes(FString ItemID);

	void AddToStack(int32 index, int32 Quantity);

	int32 FindEmptySlot();

	bool CreateNewStack(FString ItemID, int32 Quantity, UDataTable* ItemTable, FName ItemRowName, int32 index);

	void Debug_PrintInventory();

	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int32 index, bool RemoveWholeStack, bool IsConsumed);

	void DropItem(FString ItemID, int32 Quantity);

	bool ConsumeItem(int32 index);

	FItemStruct* GetItemData(FString ItemID);

	FVector GetDropLocation();

	UFUNCTION(BlueprintCallable)
	void TransferSlots(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex);

	void InteractionTrace();

	void InteractionWith();
};
