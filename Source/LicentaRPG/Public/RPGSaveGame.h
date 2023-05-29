// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LICENTARPG_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform PlayerTransform;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	float CurrentMana;

	UPROPERTY()
	float CurrentStamina;

	UPROPERTY()
	float CurrentExperience;

	UPROPERTY()
	int32 CurrentLevel;

	UPROPERTY()
	int32 CurrentStrength;

	UPROPERTY()
	int32 CurrentDexterity;

	UPROPERTY()
	int32 CurrentIntelligence;

	UPROPERTY()
	int32 CurrentAvailablePoints;

};
