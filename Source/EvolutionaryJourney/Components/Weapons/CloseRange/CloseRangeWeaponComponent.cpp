// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/CloseRange/CloseRangeWeaponComponent.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"

UCloseRangeWeaponComponent::UCloseRangeWeaponComponent()
{
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Close Range Weapon"));
    
}

void UCloseRangeWeaponComponent::BeginPlay()
{
    AActor* Owner = GetOwner();
    if (IsValid(Owner))
    {
        WeaponOwner = Owner;
        ACharacter* Character = Cast<ACharacter>(WeaponOwner);
        if (IsValid(Character))
        {
            USkeletalMeshComponent* MeshComp = Character->GetMesh();
            if (IsValid(MeshComp))
            {
                IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
                if (WeaponUser)
                {
                    WeaponUser->SetIsAttacking(false);
                }
                else {
                    UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: WeaponUser is not valid"));
                    return;
                }
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: MeshComp is not valid"));
                return;
            }
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: Character is not valid"));
            return;
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: Owner is not valid"));
        return;
    }
}


void UCloseRangeWeaponComponent::StartAttack()
{
    if (IsValid(WeaponOwner))
    {
        IAttackInterface* AnimInstance = Cast<IAttackInterface>(CustomAnimInstance);
        if (AnimInstance && !AnimInstance->GetIsAttacking() && WeaponIsActive)
        {
            AnimInstance->SetIsAttacking(true);
			AnimInstance->SetAttackIsCloseRange(true);
            FTimerHandle InvincibilityDelay;
            GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UBaseWeaponClass::AttackAnimDelay, 0.3f, false);
            AnimInstance->SetIsAttacking(false);
            // Weapon->SetGeberateOverlapEvents(true);
        }
	}
}

void UCloseRangeWeaponComponent::LineTrace()
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
