// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvolutionaryJourney/Components/InventorySystem/SlotStruct/FSlotStruct.h"
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

	int32 AddToInventory(FString ItemID, int32 Quantity);
	int32 FindSlot(FString ItemID);
	int32 GetMaxStackSizes(FDataTableRowHandle ItemID);
	void AddToStack(int32 index, int32 Quantity);
	int32 FindEmptySlot();
	bool CreateNewStack(FString ItemID, int32 Quantity);
	void Debug_PrintInventory();
	void RemoveFromInventory(class AActor* ItemToRemove);
	void InteractionTrace();
	void InteractionWith();
};
