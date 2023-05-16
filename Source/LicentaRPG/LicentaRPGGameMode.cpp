// Copyright Epic Games, Inc. All Rights Reserved.

#include "LicentaRPGGameMode.h"
#include "UObject/ConstructorHelpers.h"

ALicentaRPGGameMode::ALicentaRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	DifficultyManager = CreateDefaultSubobject<UDifficultyManager>(TEXT("DifficultyManager"));

}

UDifficultyManager* ALicentaRPGGameMode::GetDifficultyManager() const
{
	return DifficultyManager;
}
