// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/CloseRange/CloseRangeWeaponComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EvolutionaryJourney/Player/PlayerCharacter.h"
#include "EvolutionaryJourney//Animations/PlayerCharacter/PlayerCharacterAnimations.h"

// Sets default values for this component's properties
UCloseRangeWeaponComponent::UCloseRangeWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
}


// Called when the game starts
void UCloseRangeWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
    if (WeaponOwner)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
        UAnimInstance* AnimInstance = WeaponOwner->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            CustomAnimInstance = Cast<UPlayerCharacterAnimations>(AnimInstance);
            if (CustomAnimInstance)
            {
                CustomAnimInstance->SetIsAttacking(false);
            }
        }
    }

}


// Called every frame
void UCloseRangeWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UCloseRangeWeaponComponent::StartAttack()
{
    if (WeaponOwner)
    {
        if (CustomAnimInstance && !CustomAnimInstance->bIsAttacking)
        {
            CustomAnimInstance->SetIsAttacking(true);
        }
    }
}

void UCloseRangeWeaponComponent::LineTrace()
{
    if (WeaponOwner) {
        FVector StartLocation = Weapon->GetSocketLocation(FName("Start"));
        FVector EndLocation = Weapon->GetSocketLocation(FName("End"));

        FHitResult HitResult;
        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(WeaponOwner);

        GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

        if (HitResult.bBlockingHit) {
            AActor* ActorHit = HitResult.GetActor();
            ActorHit->Destroy();
        }
    }
    
}
