// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/LongRange/LongRangeWeaponComponent.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Weapons/BaseProjectile/BaseProjectile.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"

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
    IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
    if (IsValid(WeaponOwner) && WeaponIsActive && !WeaponUser->GetIsAttacking()) {
        if (WeaponUser)
        {
            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(false);
            FVector OwnerLocation = WeaponOwner->GetActorLocation();
            FRotator OwnerRotation = WeaponOwner->GetActorRotation();

            FVector ForwardVector = WeaponOwner->GetActorForwardVector();

            float SpawnDistance = 200.0f;
            FVector SpawnLocation = OwnerLocation + (ForwardVector * SpawnDistance);

            SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, SpawnLocation, OwnerRotation);
            SpawnedProjectile->OnActorHit.AddDynamic(this, &ULongRangeWeaponComponent::OnHit);
            FTimerHandle InvincibilityDelay;
            GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UBaseWeaponComponent::AttackAnimDelay, 0.3f, false);

            WeaponUser->SetIsAttacking(false);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: WeaponUser is not valid"));
            return;
        }
    }
}

void ULongRangeWeaponComponent::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if (IsValid(OtherActor) && OtherActor != WeaponOwner)
    {
		UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
        if (IsValid(HealthComponent)) {
			HealthComponent->TakeDamage(SpawnedProjectile->Damage);
        }
        SpawnedProjectile->Destroy();
    }
}
