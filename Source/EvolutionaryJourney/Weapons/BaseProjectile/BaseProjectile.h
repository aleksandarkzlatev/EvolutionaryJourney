// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class EVOLUTIONARYJOURNEY_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int Damage;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

};
