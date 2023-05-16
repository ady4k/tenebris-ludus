// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DifficultyManager.h"
#include "LicentaRPGGameMode.generated.h"

UCLASS(minimalapi)
class ALicentaRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	UDifficultyManager* DifficultyManager;

public:
	ALicentaRPGGameMode();
	UDifficultyManager* GetDifficultyManager() const;
};



