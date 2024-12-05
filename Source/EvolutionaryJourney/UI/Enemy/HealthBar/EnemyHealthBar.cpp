// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/UI/Enemy/HealthBar/EnemyHealthBar.h"
#include "EvolutionaryJourney/Enemies/BaseEnemy/BaseEnemy.h"


void UEnemyHealthBar::SetOwnerEnemy(ABaseEnemy* NewOwner)
{
    Owner = NewOwner;
}

void UEnemyHealthBar::NativeConstruct()
{
    Super::NativeConstruct();
}