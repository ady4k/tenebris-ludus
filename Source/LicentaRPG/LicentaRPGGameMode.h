// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DifficultyManager.h"
#include "RPGSaveGame.h"
#include "CharacterStats.h"
#include "EnemySpawnManager.h"
#include "PickupsManager.h"
#include "LicentaRPGGameMode.generated.h"

UCLASS(minimalapi)
class ALicentaRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	UDifficultyManager* DifficultyManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	UEnemySpawnManager* EnemySpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	UPickupsManager* PickupsManager;

	UPROPERTY()
	UCharacterStats* PlayerCharacterStats;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ALicentaRPGGameMode();
	UDifficultyManager* GetDifficultyManager() const;

	UFUNCTION(BlueprintCallable)
	void SaveGame(FString SlotName);

	UFUNCTION(BlueprintCallable)
	void LoadGame(FString SlotName) const;

private:
	FTimerHandle AutoSaveTimerHandle;
	float const AutoSaveTime = 60.0f;

	void AutoSave();

	void SetPlayerCharacterStats();
	UCharacterStats* GetPlayerCharacterStats() const;

	TArray<uint8> ConvertSaveGameToByteArray(URPGSaveGame* SaveGameInstance);
	URPGSaveGame* ConvertByteArrayToSaveGame(TArray<uint8>& Data) const;

	static FString GetFilePath(const FString& SlotName);

	TArray<uint8> EncryptSaveFile(const TArray<uint8>& Data, const TArray<uint8>& Key) const;
	TArray<uint8> DecryptSaveFile(const TArray<uint8>& Data, const TArray<uint8>& Key) const;

	bool SaveGameDataToFile(const FString& FilePath, TArray<uint8>& Data) const;
	TArray<uint8> LoadGameDataFromFile(const FString& FilePath) const;
};



