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
	SetEnemyDamageMultiplier(0.75f + DifficultyLevel * 0.25f);
	SetEnemyDamageReudctionMultiplier(1.25f - DifficultyLevel * 0.25f);
	SetEnemyMovementSpeedMultiplier(0.75f + DifficultyLevel * 0.25f);
	SetEnemyHealthRegenMultiplier(0.5f + DifficultyLevel * 0.5f);
	SetEnemyHealthRegenDelayAdditive(10.f - DifficultyLevel * 2.5f);
	SetEnemyAttackSpeed(1.6f - DifficultyLevel * 0.4f);

	SetStaminaRegenMultiplier(1.25f - DifficultyLevel * 0.25f);
	SetStaminaEnableRegenAdditive(1.f + DifficultyLevel * 1.f);
}

void UDifficultyManager::SetEnemyDamageMultiplier(const float Multiplier)
{
	if (Multiplier < 0.75f || Multiplier > 1.5f)
	{
		return;
	}
	EnemyDamageMultiplier = Multiplier;
}

void UDifficultyManager::SetEnemyDamageReudctionMultiplier(const float Multiplier)
{
	if (Multiplier < 0.5f || Multiplier > 1.25f)
	{
		return;
	}
	EnemyDamageReductionMultiplier = Multiplier;
}

void UDifficultyManager::SetEnemyMovementSpeedMultiplier(float Multiplier)
{
	if (Multiplier < 0.75f || Multiplier > 1.5f)
	{
		return;
	}
	EnemyMovementSpeedMultiplier = Multiplier;
}

void UDifficultyManager::SetEnemyHealthRegenMultiplier(float Multiplier)
{
	if (Multiplier < 0.5f || Multiplier > 2.f)
	{
		return;
	}
	EnemyHealthRegenMultiplier = Multiplier;
}

void UDifficultyManager::SetEnemyHealthRegenDelayAdditive(float Additive)
{
	if (Additive < 0.f || Additive > 3.f)
	{
		return;
	}
	EnemyHealthRegenDelayAdditive = Additive;
}

void UDifficultyManager::SetStaminaRegenMultiplier(const float Multiplier)
{
	if (Multiplier < 0.5f || Multiplier > 1.25f)
	{
		return;
	}
	StaminaRegenMultiplier = Multiplier;
}

void UDifficultyManager::SetStaminaEnableRegenAdditive(const float Additive)
{
	if (Additive < 1.f || Additive > 4.f)
	{
		return;
	}
	StaminaEnableRegenAdditive = Additive;
}

void UDifficultyManager::SetEnemyAttackSpeed(const float AttackSpeed)
{
	if (AttackSpeed < 0.39f || AttackSpeed > 1.6f)
	{
		return;
	}
	EnemyAttackSpeed = AttackSpeed;
}

float UDifficultyManager::GetEnemyDamageMultiplier() const
{
	return EnemyDamageMultiplier;
}

float UDifficultyManager::GetEnemyDamageReductionMultiplier() const
{
	return EnemyDamageReductionMultiplier;
}

float UDifficultyManager::GetEnemyMovementSpeedMultiplier() const
{
	return EnemyMovementSpeedMultiplier;
}

float UDifficultyManager::GetEnemyHealthRegenMultiplier() const
{
	return EnemyHealthRegenMultiplier;
}

float UDifficultyManager::GetEnemyHealthRegenDelayAdditive() const
{
	return EnemyHealthRegenDelayAdditive;
}

float UDifficultyManager::GetStaminaRegenMultiplier() const
{
	return StaminaRegenMultiplier;
}

float UDifficultyManager::GetStaminaEnableRegenAdditive() const
{
	return StaminaEnableRegenAdditive;
}

float UDifficultyManager::GetEnemyAttackSpeed() const
{
	return EnemyAttackSpeed;
}
