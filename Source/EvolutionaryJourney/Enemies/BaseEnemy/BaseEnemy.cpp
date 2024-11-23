// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Enemies/EnemyAiController/EnemyAiController.h"
#include "Perception/AIPerceptionComponent.h"
#include "EvolutionaryJourney/Animations/PlayerCharacter/PlayerCharacterAnimations.h"
#include "Components/ChildActorComponent.h"
#include "EvolutionaryJourney/Weapons/BaseWeaponSystem/BaseWeaponSystem.h"
#include "EvolutionaryJourney/Weapons/CloseRangeSystem/CloseRangeSystem.h"
#include "EvolutionaryJourney/Weapons/LongRangeSystem/LongRangeSystem.h"
#include "Kismet/GameplayStatics.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"


ABaseEnemy::ABaseEnemy()
{
		// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnemyAiController::StaticClass();

	CloseRangeSystem = CreateDefaultSubobject<UChildActorComponent>(TEXT("CloseRangeSystem"));
	if (IsValid(CloseRangeSystem))
	{
		CloseRangeSystem->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemy: CloseRangeWeaponComponent->WeaponMesh is not valid"));
	}

	LongRangeSystem = CreateDefaultSubobject<UChildActorComponent>(TEXT("LongRangeSystem"));
	if (IsValid(LongRangeSystem))
	{
		LongRangeSystem->SetupAttachment(GetMesh(), TEXT("BowSocket"));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemy: LongRangeWeaponComponent->WeaponMesh is not valid"));
	}

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AEnemyAiController>(GetController());


	ACloseRangeSystem* CloseRangeWeapon = Cast<ACloseRangeSystem>(CloseRangeSystem->GetChildActor());
	if (IsValid(CloseRangeWeapon))
	{
		CloseRangeWeapon->InitializeWeapon(this);
	}

	ALongRangeSystem* LongRangeWeapon = Cast<ALongRangeSystem>(LongRangeSystem->GetChildActor());
	if (IsValid(LongRangeWeapon))
	{
		LongRangeWeapon->InitializeWeapon(this);
	}
	
	if (IsValid(CloseRangeWeapon) && CloseRangeSystem->IsVisible())
	{
		ActiveWeapon = CloseRangeWeapon;
		CloseRangeWeapon->WeaponIsActive = true;
		LongRangeWeapon->WeaponIsActive = false;
		CloseRangeWeapon->SetActorHiddenInGame(false);
		LongRangeWeapon->SetActorHiddenInGame(true);
	}
	else if (IsValid(LongRangeWeapon) && LongRangeSystem->IsVisible())
	{
		ActiveWeapon = LongRangeWeapon;
		CloseRangeWeapon->WeaponIsActive = false;
		LongRangeWeapon->WeaponIsActive = true;
		CloseRangeWeapon->SetActorHiddenInGame(true);
		LongRangeWeapon->SetActorHiddenInGame(false);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemy: ChosenWeapon is not valid"));
	}

	AnimInstance = Cast<UPlayerCharacterAnimations>(GetCustomAnimInstance());
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(AiController))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(AiController->DetectedPlayer);
		if (IsValid(PlayerCharacter)) {
			float DistanceToPlayer = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());

			if (ActiveWeapon == CloseRangeSystem->GetChildActor())
			{
				if (DistanceToPlayer <= AttackRange && AiController->bIsPlayerDetected)
				{
					StartAttack();
					AiController->StopMovement();
				}
				else if(!GetIsAttacking()){
					AiController->MoveToActor(PlayerCharacter);
				}
			}
			else if (ActiveWeapon == LongRangeSystem->GetChildActor())
			{
				if (AiController->bIsPlayerDetected && !GetIsAttacking())
				{
					AiController->AimAtTarget(PlayerCharacter);
					StartAttack();
				}
			}
		}
	}
}

UAnimInstance* ABaseEnemy::GetCustomAnimInstance() const
{
	return GetMesh()->GetAnimInstance();
}

void ABaseEnemy::SetIsAttacking(bool bIsAttacking)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->SetIsAttacking(bIsAttacking);
	}
}

bool ABaseEnemy::GetIsAttacking() const
{
	if (IsValid(AnimInstance))
	{
		return AnimInstance->GetIsAttacking();
	}
	return false;
}

void ABaseEnemy::SetAttackIsCloseRange(bool bIsCloseRange)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->SetAttackIsCloseRange(bIsCloseRange);
	}
}

bool ABaseEnemy::GetAttackIsCloseRange() const
{
	if (IsValid(AnimInstance))
	{
		return AnimInstance->GetAttackIsCloseRange();
	}
	return false;
}

void ABaseEnemy::StartAttack()
{
	if (IsValid(ActiveWeapon) && !GetIsAttacking())
	{
		ActiveWeapon->StartAttack();
	}
}

