// Fill out your copyright notice in the Description page of Project Settings.


#include "EvolutionaryJourney/GameInstance/Player/PlayerGameInstance.h"

UPlayerGameInstance::UPlayerGameInstance()
{
	Health = 0;
	MaxHealth = 0;
	Content.Empty();
	Level = 0;
	CurrentEXP = 0;
	EXPToLevelUp = 0;
}
