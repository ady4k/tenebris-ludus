#include "CharacterStats.h"

UCharacterStats::UCharacterStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UCharacterStats::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
	CurrentStamina = MaxStamina;
}


// Called every frame
void UCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// ----- HEALTH -----
void UCharacterStats::IncreaseMaxHealth(float const Amount)
{
	MaxHealth += Amount;
	CurrentHealth = MaxHealth;
	UpdateHealthBar(CurrentHealth, MaxHealth);
}

void UCharacterStats::IncreaseHealth(float const Amount)
{
	CurrentHealth += Amount;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	UpdateHealthBar(CurrentHealth, MaxHealth);
}

void UCharacterStats::DecreaseHealth(float const Amount)
{
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
	}
	UpdateHealthBar(CurrentHealth, MaxHealth);
}

void UCharacterStats::SetCurrentHealth(float const Amount)
{
	CurrentHealth = Amount;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	UpdateHealthBar(CurrentHealth, MaxHealth);
}

float UCharacterStats::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UCharacterStats::GetMaxHealth() const
{
	return MaxHealth;
}


// ----- MANA -----
void UCharacterStats::IncreaseMaxMana(float const Amount)
{
	MaxMana += Amount;
	CurrentMana = MaxMana;
	UpdateManaBar(CurrentMana, MaxMana);
}

void UCharacterStats::IncreaseMana(float const Amount)
{
	CurrentMana += Amount;
	if (CurrentMana > MaxMana)
	{
		CurrentMana = MaxMana;
	}
	UpdateManaBar(CurrentMana, MaxMana);
}

void UCharacterStats::DecreaseMana(float const Amount)
{
	CurrentMana -= Amount;
	if (CurrentMana <= 0)
	{
		CurrentMana = 0;
	}
	UpdateManaBar(CurrentMana, MaxMana);
}

void UCharacterStats::SetCurrentMana(float const Amount)
{
	CurrentMana = Amount;
	if (CurrentMana > MaxMana)
	{
		CurrentMana = MaxMana;
	}
	UpdateManaBar(CurrentMana, MaxMana);
}

float UCharacterStats::GetCurrentMana() const
{
	return CurrentMana;
}

float UCharacterStats::GetMaxMana() const
{
	return MaxMana;
}


// ----- STAMINA -----
void UCharacterStats::IncreaseMaxStamina(float const Amount)
{
	MaxStamina += Amount;
	CurrentStamina = MaxStamina;
	UpdateStaminaBar(CurrentStamina, MaxStamina);
}

void UCharacterStats::IncreaseStamina(float const Amount)
{
	CurrentStamina += Amount;
	if (CurrentStamina > MaxStamina)
	{
		CurrentStamina = MaxStamina;
	}
	UpdateStaminaBar(CurrentStamina, MaxStamina);
}

void UCharacterStats::DecreaseStamina(float const Amount)
{
	CurrentStamina -= Amount;
	if (CurrentStamina <= 0)
	{
		CurrentStamina = 0;
	}
	UpdateStaminaBar(CurrentStamina, MaxStamina);
}

void UCharacterStats::SetCurrentStamina(float const Amount)
{
	CurrentStamina = Amount;
	if (CurrentStamina > MaxStamina)
	{
				CurrentStamina = MaxStamina;
	}
	UpdateStaminaBar(CurrentStamina, MaxStamina);
}

float UCharacterStats::GetCurrentStamina() const
{
	return CurrentStamina;
}

float UCharacterStats::GetMaxStamina() const
{
	return MaxStamina;
}

// ----- EXPERIENCE -----
void UCharacterStats::IncreaseMaxExperience(float const Amount)
{
	MaxExperience += Amount;
}

void UCharacterStats::IncreaseExperience(float const Amount)
{
	CurrentExperience += Amount;
	while (CurrentExperience >= MaxExperience)
	{
		CurrentExperience -= MaxExperience;
		IncreaseLevel();
	}
	UpdateExperienceBar(CurrentExperience, MaxExperience);
}

void UCharacterStats::SetCurrentExperience(float const Amount)
{
	CurrentExperience = Amount;
	UpdateExperienceBar(CurrentExperience, MaxExperience);
}

float UCharacterStats::GetCurrentExperience() const
{
	return CurrentExperience;
}

void UCharacterStats::IncreaseLevel()
{
	CurrentLevel++;
	AvailableStatsPoints++;
	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
	UpdateLevelText(CurrentLevel);
}

void UCharacterStats::SetCurrentLevel(int32 const Amount)
{
	CurrentLevel = Amount;
	UpdateLevelText(CurrentLevel);
}

int32 UCharacterStats::GetCurrentLevel() const
{
	return CurrentLevel;
}


// ----- STATS POINTS -----
void UCharacterStats::SetStrength(int32 const Amount)
{
	Strength = Amount;
}

void UCharacterStats::SetDexterity(int32 const Amount)
{
	Dexterity = Amount;
}

void UCharacterStats::SetIntelligence(int32 const Amount)
{
	Intelligence = Amount;
}

int32 UCharacterStats::GetStrength() const
{
	return Strength;
}

int32 UCharacterStats::GetDexterity() const
{
	return Dexterity;
}

int32 UCharacterStats::GetIntelligence() const
{
	return Intelligence;
}

void UCharacterStats::SetAvailableStatsPoints(int32 const Amount)
{
	AvailableStatsPoints = Amount;
}

int32 UCharacterStats::GetAvailableStatsPoints() const
{
	return AvailableStatsPoints;
}

void UCharacterStats::IncreaseStats(EStatsSelection StatToChange)
{
	switch (StatToChange)
	{
		case EStatsSelection::Strength:
			Strength++;
			break;
		case EStatsSelection::Dexterity:
			Dexterity++;
			break;
		case EStatsSelection::Intelligence:
			Intelligence++;
			break;
		default:
			return;
	}
	AvailableStatsPoints--;
	CalculateStats();
}

void UCharacterStats::CalculateStats()
{
	MaxHealth = (Strength * 5) + (Dexterity * 3) + (Intelligence * 2);
	MaxMana = (Strength * 1) + (Dexterity * 3) + (Intelligence * 6);
	MaxStamina = (Strength * 3) + (Dexterity * 5) + (Intelligence * 2);
	UpdateHealthBar(CurrentHealth, MaxHealth);
	UpdateManaBar(CurrentMana, MaxMana);
	UpdateStaminaBar(CurrentStamina, MaxStamina);
	UpdateExperienceBar(CurrentExperience, MaxExperience);
}

