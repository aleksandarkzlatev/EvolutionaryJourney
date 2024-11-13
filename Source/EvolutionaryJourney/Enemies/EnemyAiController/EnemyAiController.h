// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAiController.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API AEnemyAiController : public AAIController
{
	GENERATED_BODY()
	
	AEnemyAiController();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere)
	class AActor* AIOwner;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	bool bIsPlayerDetected;

	UFUNCTION()
	void OntargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
