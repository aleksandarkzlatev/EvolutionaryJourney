// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/CloseRangeSystem/CloseRangeSystem.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Weapons/BaseCloseRangeWeapon/BaseCloseRangeWeapon.h"
#include "Components/CapsuleComponent.h"



ACloseRangeSystem::ACloseRangeSystem()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ACloseRangeSystem::BeginPlay()
{
	Super::BeginPlay();
}

void ACloseRangeSystem::InitializeWeapon(AActor* InitOwner)
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
                    Weapon->OnActorBeginOverlap.AddDynamic(this, &ACloseRangeSystem::BeginOverlap);
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


void ACloseRangeSystem::StartAttack()
{
    if (IsValid(WeaponOwner))
    {
        IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
        if (WeaponUser && !WeaponUser->GetIsAttacking() && WeaponIsActive)
        {
			Weapon->CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(true);
            FTimerHandle AttackDelay;
            GetWorld()->GetTimerManager().SetTimer(AttackDelay, this, &ACloseRangeSystem::AttackAnimDelay, 1.0f, false);
        }
    }
	else {
		UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: Owner is not valid"));
		return;
	}
}

void ACloseRangeSystem::AttackAnimDelay()
{
    IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
    WeaponUser->SetIsAttacking(false);
    Weapon->CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACloseRangeSystem::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (IsValid(OtherActor) && OtherActor != WeaponOwner)
    {
        UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
        if (IsValid(HealthComponent)) {
            HealthComponent->TakeDamage(Weapon->Damage);
        }
    }
}