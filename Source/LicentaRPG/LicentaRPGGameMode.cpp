// Copyright Epic Games, Inc. All Rights Reserved.

#include "LicentaRPGGameMode.h"
#include "LicentaRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALicentaRPGGameMode::ALicentaRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
