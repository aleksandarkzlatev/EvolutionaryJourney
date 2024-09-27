// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/CloseRange/CloseRangeWeaponComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Components/Weapons/WeaponInterface/WeaponInterface.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"

UCloseRangeWeaponComponent::UCloseRangeWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    Damage = 1;
}


// Called when the game starts
void UCloseRangeWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
   

    AActor* Owner = GetOwner();
    if (Owner)
    {
		WeaponOwner = Owner;
        ACharacter* Character = Cast<ACharacter>(Owner);
        if (Character)
        {
            USkeletalMeshComponent* MeshComp = Character->GetMesh();
            if (MeshComp)
            {
                UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
                if (AnimInstance)
                {
					CustomAnimInstance = AnimInstance;
                    IWeaponInterface* WeaponUser = Cast<IWeaponInterface>(WeaponOwner);
                    if (WeaponUser)
                    {
						WeaponUser->SetIsAttacking(false);
                    }
                }
            }
        }
    }
}


// Called every frame
void UCloseRangeWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCloseRangeWeaponComponent::StartAttack()
{
    if (WeaponOwner)
    {
        IWeaponInterface* WeaponUser = Cast<IWeaponInterface>(WeaponOwner);
        if (WeaponUser)
        {
			IAttackInterface* AnimInstance = Cast<IAttackInterface>(CustomAnimInstance);
            if (AnimInstance && !AnimInstance->GetIsAttacking())
            {
                WeaponUser->SetIsAttacking(true);
            }
        }
    }
}

void UCloseRangeWeaponComponent::LineTrace()
{
    if (WeaponOwner) {
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
