// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/BaseWeapon/BaseWeaponClass.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"


// Sets default values for this component's properties
UBaseWeaponClass::UBaseWeaponClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Damage = 1;
}


// Called when the game starts
void UBaseWeaponClass::BeginPlay()
{
    Super::BeginPlay();
}


// Called every frame
void UBaseWeaponClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseWeaponClass::StartAttack()
{
}

void UBaseWeaponClass::AttackAnimDelay()
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

void UBaseWeaponClass::LineTrace()
{
}



