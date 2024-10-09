// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/BaseWeapon/BaseWeaponClass.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Components/Health/HealthComponent.h"
#include "EvolutionaryJourney/Components/Weapons/WeaponInterface/WeaponInterface.h"

// Sets default values for this component's properties
UBaseWeaponClass::UBaseWeaponClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Damage = 1;
}


// Called when the game starts
void UBaseWeaponClass::BeginPlay()
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


// Called every frame
void UBaseWeaponClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseWeaponClass::StartAttack()
{ 
    if (IsValid(WeaponOwner))
    {
        IWeaponInterface* WeaponUser = Cast<IWeaponInterface>(WeaponOwner);
        if (WeaponUser)
        {
            IAttackInterface* AnimInstance = Cast<IAttackInterface>(CustomAnimInstance);
            if (AnimInstance && !AnimInstance->GetIsAttacking())
            {
                WeaponUser->SetIsAttacking(true);

                FTimerHandle InvincibilityDelay;
                GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UBaseWeaponClass::AttackAnimDelay, 0.4f, false);
                // WeaponMesh->SetGeberateOverlapEvents(true);
            }
        }
    }
}

void UBaseWeaponClass::AttackAnimDelay()
{
    if (IsValid(WeaponOwner))
	{
		IWeaponInterface* WeaponUser = Cast<IWeaponInterface>(WeaponOwner);
		if (WeaponUser)
		{
			WeaponUser->SetIsAttacking(false);
		}
	}
}

void UBaseWeaponClass::LineTrace()
{
    if (IsValid(WeaponOwner)) {
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



