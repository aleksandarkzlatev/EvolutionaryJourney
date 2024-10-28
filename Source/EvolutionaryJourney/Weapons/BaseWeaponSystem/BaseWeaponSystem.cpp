// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/BaseWeaponSystem/BaseWeaponSystem.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "Components/ChildActorComponent.h"

// Sets default values
ABaseWeaponSystem::ABaseWeaponSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseWeaponSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeaponSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeaponSystem::StartAttack()
{
}

void ABaseWeaponSystem::AttackAnimDelay()
{
    if (IsValid(WeaponOwner))
    {
        if (CustomAnimInstance == nullptr)
        {
            IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
            CustomAnimInstance = WeaponUser->GetCustomAnimInstance();
        }
        else {
            IAttackInterface* AnimInstance = Cast<IAttackInterface>(CustomAnimInstance);
            if (AnimInstance)
            {
                AnimInstance->SetIsAttacking(true);
            }
        }
    }
}
