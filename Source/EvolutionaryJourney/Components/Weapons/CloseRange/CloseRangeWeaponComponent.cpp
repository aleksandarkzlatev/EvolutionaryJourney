// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/CloseRange/CloseRangeWeaponComponent.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Components/Weapons/BaseCloseRangeWeapon/BaseCloseRangeWeapon.h"

UCloseRangeWeaponComponent::UCloseRangeWeaponComponent()
{
	CloseRangeWeapon = CreateDefaultSubobject<ABaseCloseRangeWeapon>(TEXT("Close Range Weapon"));
    /*CloseRangeWeapon->OnActorBeginOverlap.AddDynamic(this, &UCloseRangeWeaponComponent::BeginOverlap);
    CloseRangeWeapon->OnActorEndOverlap.AddDynamic(this, &UCloseRangeWeaponComponent::EndOverlap);*/
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
        IAttackInterface* WeaponUser = Cast<IAttackInterface>(WeaponOwner);
        if (WeaponUser && !WeaponUser->GetIsAttacking() && WeaponIsActive)
        {
            WeaponUser->SetIsAttacking(true);
            WeaponUser->SetAttackIsCloseRange(true);
            FTimerHandle InvincibilityDelay;
            GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UBaseWeaponComponent::AttackAnimDelay, 0.3f, false);
            WeaponUser->SetIsAttacking(false);
        }
    }
}

void UCloseRangeWeaponComponent::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (IsValid(OtherActor) && OtherActor != WeaponOwner)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap"));
    }
}

void UCloseRangeWeaponComponent::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap"));
}