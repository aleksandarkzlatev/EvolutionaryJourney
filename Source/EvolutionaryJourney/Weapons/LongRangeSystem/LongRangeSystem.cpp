// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/LongRangeSystem/LongRangeSystem.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Weapons/BaseProjectile/BaseProjectile.h"
#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"
#include "EvolutionaryJourney/Enemies/EnemyAiController/EnemyAiController.h"



ALongRangeSystem::ALongRangeSystem()
{
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALongRangeSystem::BeginPlay()
{
	Super::BeginPlay();
}

void ALongRangeSystem::InitializeWeapon(AActor* InitOwner)
{
    if (IsValid(InitOwner))
    {
        WeaponOwner = InitOwner;
        if (ACharacter* Character = Cast<ACharacter>(WeaponOwner))
        {            
            if (USkeletalMeshComponent* MeshComp = Character->GetMesh())
            {
                if (IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner))
                {
                    WeaponUser->SetIsAttacking(false);
                }
                else 
                {
                    UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: WeaponUser is not valid"));
                    return;
                }
            }
            else 
            {
                UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: MeshComp is not valid"));
                return;
            }
        }
        else 
        {
            UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: Character is not valid"));
            return;
        }
    }
    else 
    {
        UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: Owner is not valid"));
        return;
    }
}

void ALongRangeSystem::StartAttack()
{
    if (IsValid(WeaponOwner))
    {
        IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
        if (WeaponUser && !WeaponUser->GetIsAttacking() && WeaponIsActive)
        {
            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: Owner is not valid"));
        return;
    }
}

void ALongRangeSystem::EndAttack()
{
    IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
    WeaponUser->SetIsAttacking(false);
}

void ALongRangeSystem::SpawnProjectile()
{
    FVector OwnerLocation = WeaponOwner->GetActorLocation();
    FRotator OwnerRotation = WeaponOwner->GetActorRotation();

    FVector ForwardVector = WeaponOwner->GetActorForwardVector();

    float SpawnDistance = 200.0f;
    FVector SpawnLocation = OwnerLocation + (ForwardVector * SpawnDistance);

    SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, SpawnLocation, OwnerRotation);
    SpawnedProjectile->OnActorBeginOverlap.AddDynamic(this, &ALongRangeSystem::BeginOverlap);
}

void ALongRangeSystem::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (IsValid(OtherActor) && OtherActor != WeaponOwner)
    {
        if (WeaponOwner->IsA(ABaseEnemy::StaticClass()) && OtherActor->IsA(ABaseEnemy::StaticClass())) return;
        UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
        if (IsValid(HealthComponent)) 
        {
            HealthComponent->TakeDamage(WeaponOwner, SpawnedProjectile->Damage);
        }
        SpawnedProjectile->Destroy();
    }
}