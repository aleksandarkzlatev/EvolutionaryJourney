// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComponent->SightRadius = 1500.0f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.0f);
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ABaseEnemy::OnSeePlayer);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::OnSeePlayer(APawn* Pawn)
{
	if (IsValid(Pawn)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I see you!"));
	}
}

