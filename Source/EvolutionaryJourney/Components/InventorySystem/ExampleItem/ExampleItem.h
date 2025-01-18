// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EvolutionaryJourney/Components/InventorySystem/InteractInterface/InteractInterface.h"
#include "ExampleItem.generated.h"

UCLASS()
class EVOLUTIONARYJOURNEY_API AExampleItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExampleItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	class UItemDataComponent* ItemDataComponent;

	UFUNCTION()
	FString LookAt();

	UFUNCTION()
	void InteractWith(AActor* InventoryOwner);
};
