// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Animations/PlayerCharacter/PlayerCharacterAnimations.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerCharacterAnimations::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Owner = TryGetPawnOwner();
	if (!IsValid(Owner)) return;

	FVector Velocity = Owner->GetVelocity();
	FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.0f);
	Speed = LateralVelocity.Size();

	ACharacter* Character = Cast<ACharacter>(Owner);
	if (Character)
	{
		bIsJumping = Character->GetCharacterMovement()->IsFalling();
	}
}

UAnimInstance* UPlayerCharacterAnimations::GetCustomAnimInstance() const
{
	return nullptr;
}

void UPlayerCharacterAnimations::SetIsAttacking(bool IsAttacking)
{
	bIsAttacking = IsAttacking;
}

bool UPlayerCharacterAnimations::GetIsAttacking() const
{
	return bIsAttacking;
}

void UPlayerCharacterAnimations::SetAttackIsCloseRange(bool bIsCloseRange)
{
	bAttackIsCloseRange = bIsCloseRange;
}

bool UPlayerCharacterAnimations::GetAttackIsCloseRange() const
{
	return bAttackIsCloseRange;
}
