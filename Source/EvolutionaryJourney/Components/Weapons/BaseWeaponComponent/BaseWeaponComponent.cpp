// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/BaseWeaponComponent/BaseWeaponComponent.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"

UBaseWeaponComponent::UBaseWeaponComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBaseWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
}


// Called every frame
void UBaseWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UBaseWeaponComponent::StartAttack()
{
}

void UBaseWeaponComponent::AttackAnimDelay()
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

void UBaseWeaponComponent::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
}


