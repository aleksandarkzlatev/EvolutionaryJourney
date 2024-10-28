// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponSystem.generated.h"

UCLASS()
class EVOLUTIONARYJOURNEY_API ABaseWeaponSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WeaponIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimInstance* CustomAnimInstance;

	UPROPERTY(VisibleAnywhere)
	class AActor* WeaponOwner;

	virtual void StartAttack();

	void AttackAnimDelay();

};
