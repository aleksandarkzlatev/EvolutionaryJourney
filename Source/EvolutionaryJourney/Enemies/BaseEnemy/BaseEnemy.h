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

	void DeathDelay();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange;

	UPROPERTY(EditAnywhere, Category="AI")
	float ChasePlayerRange;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float DroppedEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UEnemyHealthBar> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detection")
	class UBoxComponent* ProximityBox;


	UAnimInstance* GetCustomAnimInstance() const;
	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bIsAttacking);
	bool GetIsAttacking() const;
	void SetAttackIsCloseRange(bool bIsCloseRange);
	bool GetAttackIsCloseRange() const;
	void SetIsDead(bool IsDead);
	bool GetIsDead() const;
	void SetIsUsingMagic(bool IsUsingMagic);
	bool GetIsUsingMagic() const;

	void StartAttack();

	UFUNCTION()
	void OnPlayerEnterProximity(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerExitProximity(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void Death();
};
