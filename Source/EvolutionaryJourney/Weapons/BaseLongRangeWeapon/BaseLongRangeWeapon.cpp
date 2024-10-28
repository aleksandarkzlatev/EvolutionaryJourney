// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/BaseLongRangeWeapon/BaseLongRangeWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "EvolutionaryJourney/Weapons/BaseProjectile/BaseProjectile.h"

// Sets default values
ABaseLongRangeWeapon::ABaseLongRangeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Damage = 1;

}

// Called when the game starts or when spawned
void ABaseLongRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseLongRangeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

