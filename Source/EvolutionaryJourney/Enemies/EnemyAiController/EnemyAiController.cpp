// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Enemies/EnemyAiController/EnemyAiController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"
#include "EvolutionaryJourney/Weapons/LongRangeSystem/LongRangeSystem.h"
#include "Navigation/PathFollowingComponent.h"



AEnemyAiController::AEnemyAiController()
{
	PrimaryActorTick.bCanEverTick = true;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (IsValid(SightConfig))
	{
		SightConfig->SightRadius = 1000.0f;
		SightConfig->LoseSightRadius = 1800.0f;
		SightConfig->PeripheralVisionAngleDegrees = 60.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		PerceptionComponent->ConfigureSense(*SightConfig);
		PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
		bIsPlayerDetected = false;
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAiController::OnTargetDetected);
	}
	
}

void AEnemyAiController::BeginPlay()
{
	Super::BeginPlay();

	AIOwner = Cast<ABaseEnemy>(GetPawn());
}

void AEnemyAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AIOwner->ActiveWeapon == AIOwner->CloseRangeSystem->GetChildActor())
	{
		if (bIsPlayerDetected && !AIOwner->GetIsAttacking())
		{
			MoveToActor(DetectedPlayer);
		}
	}
	else if (AIOwner->ActiveWeapon == AIOwner->LongRangeSystem->GetChildActor())
	{
		if (bIsPlayerDetected && !AIOwner->GetIsAttacking())
		{
			AimAtTarget(DetectedPlayer);
		}
	}
}

void AEnemyAiController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor && Actor->IsA(APlayerCharacter::StaticClass()))
    {
		if (Stimulus.WasSuccessfullySensed())
		{
			bIsPlayerDetected = true;
			DetectedPlayer = Cast<APlayerCharacter>(Actor);

			if (AIOwner->ActiveWeapon == AIOwner->CloseRangeSystem->GetChildActor())
			{
				MoveToActor(Actor);
				UPathFollowingComponent* PathFollowing = GetPathFollowingComponent();
				if (IsValid(PathFollowing))
				{
					PathFollowing->OnRequestFinished.AddUObject(this, &AEnemyAiController::OnMoveCompleted);
				}
			}
			else if (AIOwner->ActiveWeapon == AIOwner->LongRangeSystem->GetChildActor())
			{
				AimAtTarget(Actor);
			}
		}
		else 
		{
			bIsPlayerDetected = false;
			DetectedPlayer = nullptr;
			StopMovement();
		}
    }
}

void AEnemyAiController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DetectedPlayer);
		if (IsValid(PlayerCharacter)) {
			float DistanceToPlayer = FVector::Dist(PlayerCharacter->GetActorLocation(), AIOwner->GetActorLocation());

			if (AIOwner->ActiveWeapon == AIOwner->CloseRangeSystem->GetChildActor())
			{
				if (DistanceToPlayer <= AIOwner->AttackRange && bIsPlayerDetected && !AIOwner->GetIsAttacking())
				{
					AIOwner->StartAttack();
					StopMovement();
				}
			}
		}
	}
}

void AEnemyAiController::AimAtTarget(AActor* Target)
{
	if (!Target) return;

	FVector EnemyLocation = AIOwner->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FRotator LookAtRotation = (TargetLocation - EnemyLocation).Rotation();
	AIOwner->SetActorRotation(FRotator(AIOwner->GetActorRotation().Pitch, LookAtRotation.Yaw, AIOwner->GetActorRotation().Roll));

	AIOwner->StartAttack();
}


