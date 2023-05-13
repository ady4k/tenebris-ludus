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

float UCharacterStats::GetCurrentMana() const
{
	return CurrentMana;
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

void UCharacterStats::IncreaseLevel()
{
	CurrentLevel++;
	IncreaseMaxExperience(15);
	IncreaseMaxHealth(3);
	IncreaseMaxMana(2);
	IncreaseMaxStamina(3);
	AvailableStatsPoints++;

	UpdateLevelText(CurrentLevel);
}


// ----- STATS POINTS -----
void UCharacterStats::IncreaseStats(EStatsSelection StatToChange)
{
	switch (StatToChange)
	{
		case EStatsSelection::Strength:
			IncreaseMaxHealth(10);
			IncreaseMaxMana(2);
			IncreaseMaxStamina(3);
			AvailableStatsPoints--;
			break;
		case EStatsSelection::Dexterity:
			IncreaseMaxHealth(7);
			IncreaseMaxMana(3);
			IncreaseMaxStamina(5);
			AvailableStatsPoints--;
			break;
		case EStatsSelection::Intelligence:
			IncreaseMaxHealth(4);
			IncreaseMaxMana(8);
			IncreaseMaxStamina(2);
			AvailableStatsPoints--;
			break;
		default:
			break;
	}
}
