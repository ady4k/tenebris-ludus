#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/StatsEnum.h"
#include "CharacterStats.generated.h"

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

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateLevelText(int32 CurrentLevelValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateAvailablePointsText(int32 AvailablePointsValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;
	void IncreaseMaxHealth(float const Amount);
	void IncreaseHealth(float const Amount);
	void DecreaseHealth(float const Amount);
	void SetCurrentHealth(float const Amount);
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	float GetMaxMana() const;
	void IncreaseMaxMana(float const Amount);
	void IncreaseMana(float const Amount);
	void DecreaseMana(float const Amount);
	void SetCurrentMana(float const Amount);
	float GetCurrentMana() const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float GetMaxStamina() const;
	void IncreaseMaxStamina(float const Amount);
	void IncreaseStamina(float const Amount);
	void DecreaseStamina(float const Amount);
	void SetCurrentStamina(float const Amount);
	float GetCurrentStamina() const;

	void SetMaxExperience(float const Amount);
	void IncreaseExperience(float const Amount);
	void SetCurrentExperience(float const Amount);
	float GetCurrentExperience() const;

	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetCurrentLevel() const;
	void SetCurrentLevel(int32 const Amount);
	void IncreaseLevel();
	void IncreaseStats(EStatsSelection StatToChange);
	void CalculateStats();

	void SetStrength(int32 const Amount);
	void SetDexterity(int32 const Amount);
	void SetIntelligence(int32 const Amount);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetStrength() const;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetDexterity() const;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetIntelligence() const;

	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetAvailableStatsPoints() const;
	void SetAvailableStatsPoints(int32 const Amount);

private:
	// Character stats
	UPROPERTY(EditAnywhere, Category = "Health")
	float CurrentHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Mana")
	float CurrentMana = 100;

	UPROPERTY(EditAnywhere, Category = "Mana")
	float MaxMana = 100;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float CurrentStamina = 100;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float CurrentExperience = 0;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float MaxExperience = 100;

	UPROPERTY(EditAnywhere, Category = "Experience")
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Strength = 10;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Dexterity = 10;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Intelligence = 10;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 AvailableStatsPoints = 0;
};

