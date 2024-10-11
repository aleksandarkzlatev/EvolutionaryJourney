// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/BaseWeapon/BaseWeaponClass.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Components/Weapons/WeaponInterface/WeaponInterface.h"

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
    if (IsValid(WeaponOwner))
    {
       IAttackInterface* AnimInstance = Cast<IAttackInterface>(CustomAnimInstance);
       if (AnimInstance && !AnimInstance->GetIsAttacking())
       {
            AnimInstance->SetIsAttacking(true);

            FTimerHandle InvincibilityDelay;
            GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UBaseWeaponClass::AttackAnimDelay, 0.1f, false);
            AnimInstance->SetIsAttacking(false);
            // Weapon->SetGeberateOverlapEvents(true);
       }
    }
}

void UBaseWeaponClass::AttackAnimDelay()
{
    if (IsValid(WeaponOwner))
	{
        IAttackInterface* AnimInstance = Cast<IAttackInterface>(CustomAnimInstance);
		if (AnimInstance)
		{
            AnimInstance->SetIsAttacking(true);
		}
	}
}

void UBaseWeaponClass::LineTrace()
{
    if (IsValid(WeaponOwner)) {
        FVector StartLocation = WeaponMesh->GetSocketLocation(FName("Start"));
        FVector EndLocation = WeaponMesh->GetSocketLocation(FName("End"));

        FHitResult HitResult;
        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(WeaponOwner);

        GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

        if (HitResult.bBlockingHit) {
            AActor* ActorHit = HitResult.GetActor();
            UHealthComponent* EnemyHit = ActorHit->FindComponentByClass<UHealthComponent>();

            if (IsValid(EnemyHit)) {
                EnemyHit->TakeDamge(Damage);
            }
        }
    }
}



