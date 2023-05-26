// Copyright Epic Games, Inc. All Rights Reserved.

#include "LicentaRPGGameMode.h"
#include "UObject/ConstructorHelpers.h"

ALicentaRPGGameMode::ALicentaRPGGameMode()
{
	DifficultyManager = CreateDefaultSubobject<UDifficultyManager>(TEXT("DifficultyManager"));
}

UDifficultyManager* ALicentaRPGGameMode::GetDifficultyManager() const
{
	return DifficultyManager;
}
