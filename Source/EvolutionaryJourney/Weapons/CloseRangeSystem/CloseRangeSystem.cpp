// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/CloseRangeSystem/CloseRangeSystem.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"




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

            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(true);
            FTimerHandle InvincibilityDelay;
            GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &ACloseRangeSystem::AttackAnimDelay, 0.3f, false);
            WeaponUser->SetIsAttacking(false);
        }
    }
	else {
		UE_LOG(LogTemp, Error, TEXT("UCloseRangeWeaponComponent: Owner is not valid"));
		return;
	}
}

void ACloseRangeSystem::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (IsValid(OtherActor) && OtherActor != WeaponOwner)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap"));
    }
}

void ACloseRangeSystem::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap"));
}