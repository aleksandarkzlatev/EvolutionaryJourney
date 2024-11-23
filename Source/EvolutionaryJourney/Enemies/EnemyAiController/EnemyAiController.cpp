// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Enemies/EnemyAiController/EnemyAiController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"
#include "EvolutionaryJourney/Weapons/LongRangeSystem/LongRangeSystem.h"


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
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAiController::OntargetDetected);
	}
	
}

void AEnemyAiController::BeginPlay()
{
	Super::BeginPlay();

	AIOwner = GetPawn();
}

void AEnemyAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEnemyAiController::OntargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor && Actor->IsA(APlayerCharacter::StaticClass()))
    {
		if (Stimulus.WasSuccessfullySensed())
		{
			bIsPlayerDetected = true;
			bIsPlayerInSight = true;
			DetectedPlayer = Cast<APlayerCharacter>(Actor);

			ABaseEnemy* Enemy = Cast<ABaseEnemy>(AIOwner);
			ALongRangeSystem* LongRangeWeapon = Cast<ALongRangeSystem>(Enemy->LongRangeSystem->GetChildActor());
			if (Enemy->ActiveWeapon != LongRangeWeapon) {
				MoveToActor(Actor);
			}
		}
		else 
		{
			bIsPlayerDetected = false;
			bIsPlayerInSight = false;
			DetectedPlayer = nullptr;
			StopMovement();
		}
    }
}

void AEnemyAiController::AimAtTarget(AActor* Target)
{
	if (!Target) return;

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	FVector EnemyLocation = ControlledPawn->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FRotator LookAtRotation = (TargetLocation - EnemyLocation).Rotation();
	ControlledPawn->SetActorRotation(FRotator(ControlledPawn->GetActorRotation().Pitch, LookAtRotation.Yaw, ControlledPawn->GetActorRotation().Roll));
}


