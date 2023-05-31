// Copyright Epic Games, Inc. All Rights Reserved.

#include "LicentaRPGGameMode.h"

#include "Config/CryptoKey.h"
#include "LicentaRPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/MemoryWriter.h"
#include "UObject/ConstructorHelpers.h"


ALicentaRPGGameMode::ALicentaRPGGameMode()
{
	DifficultyManager = CreateDefaultSubobject<UDifficultyManager>(TEXT("DifficultyManager"));
	EnemySpawnManager = CreateDefaultSubobject<UEnemySpawnManager>(TEXT("EnemySpawnManager"));
	PickupsManager = CreateDefaultSubobject<UPickupsManager>(TEXT("PickupsManager"));
}

void ALicentaRPGGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetPlayerCharacterStats();
	EnemySpawnManager->RespawnEnemies();
	PickupsManager->RespawnPickups();

	GetWorldTimerManager().SetTimer(AutoSaveTimerHandle, this, &ALicentaRPGGameMode::AutoSave, AutoSaveTime, true);
}

UDifficultyManager* ALicentaRPGGameMode::GetDifficultyManager() const
{
	return DifficultyManager;
}


void ALicentaRPGGameMode::AutoSave()
{
	SaveGame("quick-save");
}

// ---------------------------------------------------------
void ALicentaRPGGameMode::SetPlayerCharacterStats()
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const APawn* PlayerPawn = PlayerController->GetPawn();
	const ACharacter* PlayerCharacter = Cast<ALicentaRPGCharacter>(PlayerPawn);
	UCharacterStats* CharacterStats = PlayerCharacter->FindComponentByClass<UCharacterStats>();
	PlayerCharacterStats = CharacterStats;
}

UCharacterStats* ALicentaRPGGameMode::GetPlayerCharacterStats() const
{
	return PlayerCharacterStats;
}


// ---------------------------------------------------------
void ALicentaRPGGameMode::SaveGame(const FString SlotName)
{
	URPGSaveGame* SaveGameInstance = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));

	if (const UCharacterStats* CharacterStats = GetPlayerCharacterStats(); CharacterStats && SaveGameInstance)
	{
		SaveGameInstance->PlayerTransform = CharacterStats->GetOwner()->GetActorTransform();
		SaveGameInstance->CurrentHealth = CharacterStats->GetCurrentHealth();
		SaveGameInstance->CurrentMana = CharacterStats->GetCurrentMana();
		SaveGameInstance->CurrentStamina = CharacterStats->GetCurrentStamina();
		SaveGameInstance->CurrentExperience = CharacterStats->GetCurrentExperience();
		SaveGameInstance->CurrentLevel = CharacterStats->GetCurrentLevel();
		SaveGameInstance->CurrentStrength = CharacterStats->GetStrength();
		SaveGameInstance->CurrentDexterity = CharacterStats->GetDexterity();
		SaveGameInstance->CurrentIntelligence = CharacterStats->GetIntelligence();
		SaveGameInstance->CurrentAvailablePoints = CharacterStats->GetAvailableStatsPoints();

		// Conversion to byte array
		const TArray<uint8> Data = ConvertSaveGameToByteArray(SaveGameInstance);
		if (Data.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to convert save game to byte array!"));
			return;
		}

		TArray<uint8> EncryptedData = EncryptSaveFile(Data, EncryptionKey);
		if (EncryptedData.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to encrypt save file!"));
			return;
		}

		if (SaveGameDataToFile(GetFilePath(SlotName), EncryptedData) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to save game data to file!"));
		}
	}
}

void ALicentaRPGGameMode::LoadGame(const FString SlotName) const
{
	const TArray<uint8> EncryptedData = LoadGameDataFromFile(GetFilePath(SlotName));
	if (EncryptedData.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load game data from file!"));
		return;
	}

	TArray<uint8> Data = DecryptSaveFile(EncryptedData, EncryptionKey);
	if (Data.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to decrypt save file!"));
		return;
	}

	const URPGSaveGame* SaveGameInstance = ConvertByteArrayToSaveGame(Data);
	if (UCharacterStats* CharacterStats = GetPlayerCharacterStats(); CharacterStats && SaveGameInstance)
	{
		CharacterStats->GetOwner()->SetActorTransform(SaveGameInstance->PlayerTransform);
		CharacterStats->SetCurrentHealth(SaveGameInstance->CurrentHealth);
		CharacterStats->SetCurrentMana(SaveGameInstance->CurrentMana);
		CharacterStats->SetCurrentStamina(SaveGameInstance->CurrentStamina);
		CharacterStats->SetCurrentExperience(SaveGameInstance->CurrentExperience);
		CharacterStats->SetCurrentLevel(SaveGameInstance->CurrentLevel);
		CharacterStats->SetStrength(SaveGameInstance->CurrentStrength);
		CharacterStats->SetDexterity(SaveGameInstance->CurrentDexterity);
		CharacterStats->SetIntelligence(SaveGameInstance->CurrentIntelligence);
		CharacterStats->SetAvailableStatsPoints(SaveGameInstance->CurrentAvailablePoints);
		CharacterStats->CalculateStats();
	}
	EnemySpawnManager->RespawnEnemies();
	
}


// ---------------------------------------------------------
FString ALicentaRPGGameMode::GetFilePath(const FString& SlotName)
{
	 return FPaths::ProjectSavedDir() + TEXT("SaveGames/") + SlotName;
}


// ---------------------------------------------------------
TArray<uint8> ALicentaRPGGameMode::ConvertSaveGameToByteArray(URPGSaveGame* SaveGameInstance)
{
	TArray<uint8> Data;
	FMemoryWriter MemoryWriter(Data, true);
	MemoryWriter.ArIsSaveGame = true;

	// Serialize each property of SaveGameInstance manually
	MemoryWriter.Serialize(&SaveGameInstance->PlayerTransform, sizeof(FTransform));
	MemoryWriter << SaveGameInstance->CurrentHealth;
	MemoryWriter << SaveGameInstance->CurrentMana;
	MemoryWriter << SaveGameInstance->CurrentStamina;
	MemoryWriter << SaveGameInstance->CurrentExperience;
	MemoryWriter << SaveGameInstance->CurrentLevel;
	MemoryWriter << SaveGameInstance->CurrentStrength;
	MemoryWriter << SaveGameInstance->CurrentDexterity;
	MemoryWriter << SaveGameInstance->CurrentIntelligence;
	MemoryWriter << SaveGameInstance->CurrentAvailablePoints;

	MemoryWriter.Close();

	return Data;
}


URPGSaveGame* ALicentaRPGGameMode::ConvertByteArrayToSaveGame(TArray<uint8>& Data) const
{
	URPGSaveGame* SaveGameInstance = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));

	FMemoryReader MemoryReader(Data, true);
	MemoryReader.ArIsSaveGame = true;

	// Deserialize each property of SaveGameInstance manually
	MemoryReader.Serialize(&SaveGameInstance->PlayerTransform, sizeof(FTransform));
	MemoryReader << SaveGameInstance->CurrentHealth;
	MemoryReader << SaveGameInstance->CurrentMana;
	MemoryReader << SaveGameInstance->CurrentStamina;
	MemoryReader << SaveGameInstance->CurrentExperience;
	MemoryReader << SaveGameInstance->CurrentLevel;
	MemoryReader << SaveGameInstance->CurrentStrength;
	MemoryReader << SaveGameInstance->CurrentDexterity;
	MemoryReader << SaveGameInstance->CurrentIntelligence;
	MemoryReader << SaveGameInstance->CurrentAvailablePoints;

	MemoryReader.Close();

	return SaveGameInstance;
}


// ---------------------------------------------------------
TArray<uint8> ALicentaRPGGameMode::EncryptSaveFile(const TArray<uint8>& Data, const TArray<uint8>& Key) const
{
	FAES::FAESKey AESKey;
	FMemory::Memcpy(AESKey.Key, Key.GetData(), Key.Num());

	const int32 NumBytes = Data.Num();
	const int32 PaddingSize = FAES::AESBlockSize - (NumBytes % FAES::AESBlockSize);
	const int32 PaddedSize = NumBytes + PaddingSize;

	TArray<uint8> EncryptedData;
	EncryptedData.SetNumZeroed(PaddedSize);
	FMemory::Memcpy(EncryptedData.GetData(), Data.GetData(), Data.Num());

	EncryptedData[PaddedSize - 1] = PaddingSize;

	FAES::EncryptData(EncryptedData.GetData(), EncryptedData.Num(), AESKey);

	return EncryptedData;
}

TArray<uint8> ALicentaRPGGameMode::DecryptSaveFile(const TArray<uint8>& Data, const TArray<uint8>& Key) const
{
	FAES::FAESKey AESKey;
	FMemory::Memcpy(AESKey.Key, Key.GetData(), Key.Num());

	TArray<uint8> DecryptedData;
	DecryptedData.SetNum(Data.Num());
	FMemory::Memcpy(DecryptedData.GetData(), Data.GetData(), Data.Num());

	FAES::DecryptData(DecryptedData.GetData(), Data.Num(), AESKey); 

	const int32 PaddingSize = DecryptedData[DecryptedData.Num() - 1];
	DecryptedData.RemoveAt(DecryptedData.Num() - PaddingSize, PaddingSize, false);

	return DecryptedData;
}


// ---------------------------------------------------------
bool ALicentaRPGGameMode::SaveGameDataToFile(const FString& FilePath, TArray<uint8>& Data) const
{
	if (!FFileHelper::SaveArrayToFile(Data, *FilePath))
	{
		return false;
	}
	return true;
}

TArray<uint8> ALicentaRPGGameMode::LoadGameDataFromFile(const FString& FilePath) const
{
	TArray<uint8> Data;
	if (!FFileHelper::LoadFileToArray(Data, *FilePath))
	{
		return TArray<uint8>();
	}
	return Data;
}
