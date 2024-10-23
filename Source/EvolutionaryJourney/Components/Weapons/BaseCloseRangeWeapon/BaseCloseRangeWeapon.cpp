// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Components/Weapons/BaseCloseRangeWeapon/BaseCloseRangeWeapon.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseCloseRangeWeapon::ABaseCloseRangeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(CollisionComponent);

	Damage = 1;
}

// Called when the game starts or when spawned
void ABaseCloseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCloseRangeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

