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
		SightConfig->SightRadius = 3500.0f;
		SightConfig->LoseSightRadius = 3500.0f;
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
				StartMoveCheck();
				UPathFollowingComponent* PathFollowing = GetPathFollowingComponent();
				if (IsValid(PathFollowing))
				{
					PathFollowing->OnRequestFinished.RemoveAll(this);
					PathFollowing->OnRequestFinished.AddUObject(this, &AEnemyAiController::OnMoveCompleted);
				}
			}
			else if (AIOwner->ActiveWeapon == AIOwner->LongRangeSystem->GetChildActor())
			{
				StartAimCheck();
			}
		}
		else
		{
			bIsPlayerDetected = false;
		}
    }
}

void AEnemyAiController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DetectedPlayer);
		if (IsValid(PlayerCharacter)) 
		{
			float DistanceToPlayer = FVector::Dist(PlayerCharacter->GetActorLocation(), AIOwner->GetActorLocation());

			if (AIOwner->ActiveWeapon == AIOwner->CloseRangeSystem->GetChildActor())
			{
				if (DistanceToPlayer <= AIOwner->AttackRange && bIsPlayerDetected && !AIOwner->GetIsAttacking())
				{
					AIOwner->StartAttack();
					StopMovement();
				}
				else if (DistanceToPlayer <= AIOwner->ChasePlayerRange &&!AIOwner->GetIsAttacking())
				{
					MoveToActor(DetectedPlayer);
				}
			}
		}
	}
}

void AEnemyAiController::StartMoveCheck()
{
	GetWorld()->GetTimerManager().SetTimer(MoveCheckTimerHandle, this, &AEnemyAiController::CheckAndMoveToTarget, 0.5f, true);
}

void AEnemyAiController::StopMoveCheck()
{
	GetWorld()->GetTimerManager().ClearTimer(MoveCheckTimerHandle);
}

void AEnemyAiController::CheckAndMoveToTarget()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DetectedPlayer);
	if (IsValid(PlayerCharacter)) 
	{
		float DistanceToPlayer = FVector::Dist(PlayerCharacter->GetActorLocation(), AIOwner->GetActorLocation());
		if ((bIsPlayerDetected && !AIOwner->GetIsAttacking()))
		{
			MoveToActor(DetectedPlayer);
		}
		else if (DistanceToPlayer <= AIOwner->ChasePlayerRange && !AIOwner->GetIsAttacking())
		{
			MoveToActor(DetectedPlayer);
		}
		else if (DistanceToPlayer > AIOwner->ChasePlayerRange && !AIOwner->GetIsAttacking() && !bIsPlayerDetected)
		{
			StopMoveCheck();
			StopMovement();
			DetectedPlayer = nullptr;
		}
	}
}

void AEnemyAiController::StartAimCheck()
{
	GetWorld()->GetTimerManager().SetTimer(AimCheckTimerHandle, this, &AEnemyAiController::CheckAndAimAtTarget, 0.7f, true);
}

void AEnemyAiController::StopAimCheck()
{
	GetWorld()->GetTimerManager().ClearTimer(AimCheckTimerHandle);
}

void AEnemyAiController::CheckAndAimAtTarget()
{
	if (bIsPlayerDetected && !AIOwner->GetIsAttacking())
	{
		AimAtTarget(DetectedPlayer);
	}
	else if (!AIOwner->GetIsAttacking() && !bIsPlayerDetected)
	{
		StopAimCheck();
		ClearFocus(EAIFocusPriority::Gameplay);
		DetectedPlayer = nullptr;
	}
}

void AEnemyAiController::AimAtTarget(AActor* Target)
{
	if (!Target) return;

	SetFocus(Target);

	AIOwner->StartAttack();
}


