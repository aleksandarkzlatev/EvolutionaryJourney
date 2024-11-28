// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EvolutionaryJourney/Animations/AnimationInterface/AttackInterface.h"
#include "BaseEnemy.generated.h"


UCLASS()
class EVOLUTIONARYJOURNEY_API ABaseEnemy : public ACharacter, public IAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float DroppedEXP;

	UPROPERTY(EditAnywhere)
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere)
	class AEnemyAiController* AiController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	class UChildActorComponent* CloseRangeSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	class UChildActorComponent* LongRangeSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	class ABaseWeaponSystem* ActiveWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations")
	class UPlayerCharacterAnimations* AnimInstance;


	UAnimInstance* GetCustomAnimInstance() const;
	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bIsAttacking);
	bool GetIsAttacking() const;
	void SetAttackIsCloseRange(bool bIsCloseRange);
	bool GetAttackIsCloseRange() const;

	void StartAttack();
};
