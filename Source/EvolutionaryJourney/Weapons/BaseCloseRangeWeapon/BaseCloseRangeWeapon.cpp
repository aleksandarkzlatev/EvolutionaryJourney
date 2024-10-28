// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/Weapons/BaseCloseRangeWeapon/BaseCloseRangeWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"


ABaseCloseRangeWeapon::ABaseCloseRangeWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(CollisionComponent);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

