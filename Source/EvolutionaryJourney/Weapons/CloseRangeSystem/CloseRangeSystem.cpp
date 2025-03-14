// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/CloseRangeSystem/CloseRangeSystem.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"



ACloseRangeSystem::ACloseRangeSystem()
{
    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(CollisionComponent);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Damage = 1;
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
        if (ACharacter* Character = Cast<ACharacter>(WeaponOwner))
        {
            if (USkeletalMeshComponent* MeshComp = Character->GetMesh())
            {       
                if (IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner))
                {
                    OnActorBeginOverlap.AddDynamic(this, &ACloseRangeSystem::BeginOverlap);
                    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

                    WeaponUser->SetIsAttacking(false);
                }
                else 
                {
                    UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: WeaponUser is not valid"));
                    return;
                }
            }
            else 
            {
                UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: MeshComp is not valid"));
                return;
            }
        }
        else 
        {
            UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: Character is not valid"));
            return;
        }
    }
    else 
    {
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
            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(true);
        }
    }
	else 
    {
		UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: Owner is not valid"));
		return;
	}
}

void ACloseRangeSystem::EndAttack()
{
    if (IsValid(WeaponOwner))
    {
        IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
        if (WeaponUser)
        {
            WeaponUser->SetIsAttacking(false);
            FTimerHandle AttackDelayTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(AttackDelayTimerHandle, this, &ACloseRangeSystem::EndAttackDelay, 0.2f, false);
        }
    }
}

void ACloseRangeSystem::EndAttackDelay()
{
    if (IsValid(WeaponOwner))
    {
        IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
        if (WeaponUser)
        {
            WeaponUser->SetIsAttacking(false);
        }
    }
}

void ACloseRangeSystem::TurnCollisionOn()
{
    if (IsValid(CollisionComponent)) CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACloseRangeSystem::TurnCollisionOff()
{
	if (IsValid(CollisionComponent)) CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACloseRangeSystem::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (IsValid(OtherActor) && OtherActor != WeaponOwner)
    {
        if (WeaponOwner->IsA(ABaseEnemy::StaticClass()) && OtherActor->IsA(ABaseEnemy::StaticClass())) return;
        UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
        if (IsValid(HealthComponent))
        { 
            HealthComponent->TakeDamage(WeaponOwner, Damage);
        }
    }
}