// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/LongRange/LongRangeWeaponComponent.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Weapons/Projectile/ProjectileActor.h"

ULongRangeWeaponComponent::ULongRangeWeaponComponent()
{
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Long Range Weapon"));
    
}

void ULongRangeWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
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
                    UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: WeaponUser is not valid"));
                    return;
                }
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: MeshComp is not valid"));
                return;
            }
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: Character is not valid"));
            return;
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: Owner is not valid"));
        return;
    }
}


void ULongRangeWeaponComponent::StartAttack()
{
    UE_LOG(LogTemp, Warning, TEXT("ULongRangeWeaponComponent: WeaponOwner: %s"), *WeaponOwner->GetName());
    if (IsValid(WeaponOwner) && WeaponIsActive) {
        FVector OwnerLocation = WeaponOwner->GetActorLocation();
        FRotator OwnerRotation = WeaponOwner->GetActorRotation();

        FVector ForwardVector = WeaponOwner->GetActorForwardVector();

        float SpawnDistance = 100.0f;
        FVector SpawnLocation = OwnerLocation + (ForwardVector * SpawnDistance);

        AProjectileActor* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileActor>(ProjectileActor, SpawnLocation, OwnerRotation);

        IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
        if (WeaponUser)
        {
            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(false);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: WeaponUser is not valid"));
            return;
        }
    }
}
