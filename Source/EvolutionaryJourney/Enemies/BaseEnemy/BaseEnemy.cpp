// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Enemies/EnemyAiController/EnemyAiController.h"
#include "Perception/AIPerceptionComponent.h"

ABaseEnemy::ABaseEnemy()
{
		// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnemyAiController::StaticClass();


	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	AEnemyAiController* EnemyAiController = Cast<AEnemyAiController>(GetController());
	if (EnemyAiController && PerceptionComponent)
	{
		EnemyAiController->SetPerceptionComponent(*PerceptionComponent);
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AEnemyAiController* EnemyAiController = Cast<AEnemyAiController>(GetController());
	if (EnemyAiController)
	{
		EnemyAiController->MoveToActor(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
}

UAnimInstance* ABaseEnemy::GetCustomAnimInstance() const
{
	return nullptr;
}

void ABaseEnemy::SetIsAttacking(bool bIsAttacking)
{
}

bool ABaseEnemy::GetIsAttacking() const
{
	return false;
}

void ABaseEnemy::SetAttackIsCloseRange(bool bIsCloseRange)
{
}

bool ABaseEnemy::GetAttackIsCloseRange() const
{
	return false;
}


