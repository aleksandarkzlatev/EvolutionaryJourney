// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/CloseRange/CloseRangeWeaponComponent.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Components/Weapons/WeaponInterface/WeaponInterface.h"


UCloseRangeWeaponComponent::UCloseRangeWeaponComponent()
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
                WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Close Range Weapon"));
                WeaponMesh->SetupAttachment(MeshComp, TEXT("WeaponSocket"));
                UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
                if (IsValid(AnimInstance))
                {
                    CustomAnimInstance = AnimInstance;
                    IWeaponInterface* WeaponUser = Cast<IWeaponInterface>(WeaponOwner);
                    if (WeaponUser)
                    {
                        WeaponUser->SetIsAttacking(false);
                    }
                    else {
                        UE_LOG(LogTemp, Error, TEXT("UBaseWeaponClass: WeaponUser is not valid"));
                        return;
                    }
                }
                else {
                    UE_LOG(LogTemp, Error, TEXT("UBaseWeaponClass: AnimInstance is not valid"));
                    return;
                }
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("UBaseWeaponClass: MeshComp is not valid"));
                return;
            }
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("UBaseWeaponClass: Character is not valid"));
            return;
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("UBaseWeaponClass: Owner is not valid"));
        return;
    }
	


}