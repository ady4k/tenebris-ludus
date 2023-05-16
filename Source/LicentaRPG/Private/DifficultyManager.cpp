// Fill out your copyright notice in the Description page of Project Settings.


#include "DifficultyManager.h"

// Sets default values
UDifficultyManager::UDifficultyManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UDifficultyManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDifficultyManager::SetDifficultyLevel(int Difficulty)
{
	if (Difficulty < 0 || Difficulty > 3)
	{
		return;
	}
	DifficultyLevel = Difficulty;
	CalculateDifficultyMultipliers();
}

int UDifficultyManager::GetDifficultyLevel()
{
	return DifficultyLevel;
}

// Called every frame
void UDifficultyManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDifficultyManager::CalculateDifficultyMultipliers()
{
	float const EnemyMultiplier = 0.75f + DifficultyLevel * 0.25f;
	SetEnemyDamageMultiplier(EnemyMultiplier);
	SetEnemyHealthMultiplier(EnemyMultiplier);
	SetStaminaRegenMultiplier(1.25f - DifficultyLevel * 0.25f);
	SetStaminaEnableRegenMultiplier(1.f + DifficultyLevel * 1.f);
}

void UDifficultyManager::SetEnemyDamageMultiplier(const float Multiplier)
{
	if (Multiplier < 0.75f || Multiplier > 1.5f)
	{
		return;
	}
	EnemyDamageMultiplier = Multiplier;
}

void UDifficultyManager::SetEnemyHealthMultiplier(const float Multiplier)
{
	if (Multiplier < 0.75f || Multiplier > 1.5f)
	{
		return;
	}
	EnemyHealthMultiplier = Multiplier;
}

void UDifficultyManager::SetStaminaRegenMultiplier(const float Multiplier)
{
	if (Multiplier < 0.5f || Multiplier > 1.25f)
	{
		return;
	}
	StaminaRegenMultiplier = Multiplier;
}

void UDifficultyManager::SetStaminaEnableRegenMultiplier(const float Multiplier)
{
	if (Multiplier < 1.f || Multiplier > 4.f)
	{
		return;
	}
	StaminaEnableRegenMultiplier = Multiplier;
}

float UDifficultyManager::GetEnemyDamageMultiplier() const
{
	return EnemyDamageMultiplier;
}

float UDifficultyManager::GetEnemyHealthMultiplier() const
{
	return EnemyHealthMultiplier;
}

float UDifficultyManager::GetStaminaRegenMultiplier() const
{
	return StaminaRegenMultiplier;
}

float UDifficultyManager::GetStaminaEnableRegenMultiplier() const
{
	return StaminaEnableRegenMultiplier;
}
