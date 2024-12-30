// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
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
	UPROPERTY(VisibleAnywhere)
	bool bIsPlayerDetected;

	FTimerHandle AimCheckTimerHandle;

	FTimerHandle MoveCheckTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class APlayerCharacter* DetectedPlayer = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere)
	class ABaseEnemy* AIOwner;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void StartMoveCheck();

	void StopMoveCheck();

	void CheckAndMoveToTarget();

	void StartAimCheck();

	void StopAimCheck();

	void CheckAndAimAtTarget();

	void AimAtTarget(AActor* Target);
};
