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
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FTransform PlayerTransform;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentMana;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentStamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentExperience;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 CurrentLevel;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 CurrentStrength;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 CurrentDexterity;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 CurrentIntelligence;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 CurrentAvailablePoints;
};
