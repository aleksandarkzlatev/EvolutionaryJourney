// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/LongRangeSystem/LongRangeSystem.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Weapons/BaseProjectile/BaseProjectile.h"



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

void ALongRangeSystem::StartAttack()
{
    IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
    if (IsValid(WeaponOwner) && WeaponIsActive && !WeaponUser->GetIsAttacking()) {
        if (WeaponUser)
        {
            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(false);
            FTimerHandle AttackDelay;
            GetWorld()->GetTimerManager().SetTimer(AttackDelay, this, &ALongRangeSystem::SpawnProjectile, 3.3f, false);

        }
        else {
            UE_LOG(LogTemp, Error, TEXT("ULongRangeWeaponComponent: WeaponUser is not valid"));
            return;
        }
    }
}

void ALongRangeSystem::SpawnProjectile()
{
    IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
    FVector OwnerLocation = WeaponOwner->GetActorLocation();
    FRotator OwnerRotation = WeaponOwner->GetActorRotation();

    FVector ForwardVector = WeaponOwner->GetActorForwardVector();

    float SpawnDistance = 200.0f;
    FVector SpawnLocation = OwnerLocation + (ForwardVector * SpawnDistance);

    SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, SpawnLocation, OwnerRotation);
    SpawnedProjectile->OnActorBeginOverlap.AddDynamic(this, &ALongRangeSystem::BeginOverlap);
    WeaponUser->SetIsAttacking(false);
}

void ALongRangeSystem::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
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