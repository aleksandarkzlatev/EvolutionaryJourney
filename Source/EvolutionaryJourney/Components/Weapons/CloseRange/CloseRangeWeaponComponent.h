// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CloseRangeWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVOLUTIONARYJOURNEY_API UCloseRangeWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCloseRangeWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Damage")
	int Damage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Weapon;

	UPROPERTY(EditAnywhere)
	class APlayerCharacter* WeaponOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPlayerCharacterAnimations* CustomAnimInstance;


	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void LineTrace();

};
