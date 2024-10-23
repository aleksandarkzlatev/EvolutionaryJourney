// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvolutionaryJourney/Components/Weapons/BaseWeaponComponent/BaseWeaponComponent.h"
#include "LongRangeWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYJOURNEY_API ULongRangeWeaponComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts
	void BeginPlay() override;


public:
	ULongRangeWeaponComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseProjectile> ProjectileActor;

	UPROPERTY(VisibleAnywhere)
	class ABaseProjectile* SpawnedProjectile;

	void StartAttack() override;

	virtual void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;

};
