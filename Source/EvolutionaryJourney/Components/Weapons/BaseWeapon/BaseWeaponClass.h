// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseWeaponClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVOLUTIONARYJOURNEY_API UBaseWeaponClass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseWeaponClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Damage")
	int Damage;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WeaponIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimInstance* CustomAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Weapons")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	class AActor* WeaponOwner;

	UFUNCTION(BlueprintCallable)
	virtual void LineTrace();

	virtual void StartAttack();

	void AttackAnimDelay();
};
