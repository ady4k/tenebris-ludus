#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStats.generated.h"

enum EStatsSelection : uint8
{
	Strength = 1,
	Dexterity = 2,
	Intelligence = 3
};

UCLASS(Blueprintable, BlueprintType,  ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LICENTARPG_API UCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStats();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateHealthBar(float CurrentHealthValue, float MaxHealthValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateManaBar(float CurrentManaValue, float MaxManaValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateStaminaBar(float CurrentStaminaValue, float MaxStaminaValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateExperienceBar(float CurrentExperienceValue, float MaxExperienceValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void IncreaseMaxHealth(float const Amount);
	void IncreaseHealth(float const Amount);
	void DecreaseHealth(float const Amount);
	float GetCurrentHealth() const;

	void IncreaseMaxMana(float const Amount);
	void IncreaseMana(float const Amount);
	void DecreaseMana(float const Amount);
	float GetCurrentMana() const;

	void IncreaseMaxStamina(float const Amount);
	void IncreaseStamina(float const Amount);
	void DecreaseStamina(float const Amount);
	float GetCurrentStamina() const;
	float GetMaxStamina() const;

	void IncreaseMaxExperience(float const Amount);
	void IncreaseExperience(float const Amount);

	void IncreaseLevel();
	void IncreaseStats(EStatsSelection StatToChange);

private:
	// Character stats
	UPROPERTY(EditAnywhere, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Mana")
	float CurrentMana;

	UPROPERTY(EditAnywhere, Category = "Mana")
	float MaxMana = 100;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float CurrentExperience = 0;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float MaxExperience = 100;

	UPROPERTY(EditAnywhere, Category = "Experience")
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Strength = 1;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Dexterity = 1;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Intelligence = 1;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 AvailableStatsPoints = 0;
};

