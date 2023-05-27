#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DifficultyManager.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LICENTARPG_API UDifficultyManager : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UDifficultyManager();

	UFUNCTION(BlueprintCallable, Category = "Difficulty")
	int GetDifficultyLevel();

	UFUNCTION(BlueprintCallable, Category = "Difficulty")
	void SetDifficultyLevel(int Difficulty);

	void CalculateDifficultyMultipliers();

	void SetEnemyDamageMultiplier(float Multiplier);
	void SetEnemyDamageReudctionMultiplier(float Multiplier);
	void SetEnemyMovementSpeedMultiplier(float Multiplier);
	void SetEnemyHealthRegenMultiplier(float Multiplier);
	void SetEnemyHealthRegenDelayAdditive(float Additive);
	void SetStaminaRegenMultiplier(float Multiplier);
	void SetStaminaEnableRegenAdditive(float Multiplier);
	void SetEnemyAttackSpeed(float Additive);

	float GetEnemyDamageMultiplier() const;
	float GetEnemyDamageReductionMultiplier() const;
	float GetEnemyMovementSpeedMultiplier() const;
	float GetEnemyHealthRegenMultiplier() const;
	float GetEnemyHealthRegenDelayAdditive() const;
	float GetStaminaRegenMultiplier() const;
	float GetStaminaEnableRegenAdditive() const;
	float GetEnemyAttackSpeed() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	int DifficultyLevel = 1;

	// -------- ENEMY DIFFICULTY SETTINGS -------- //
	float EnemyDamageMultiplier = 1.f;
	float EnemyDamageReductionMultiplier = 1.f;
	float EnemyMovementSpeedMultiplier = 1.f;
	float EnemyHealthRegenMultiplier = 1.f;
	float EnemyHealthRegenDelayAdditive = 1.f;
	float EnemyAttackSpeed = 1.2f;

	// -------- PLAYER DIFFICULTY SETTINGS -------- //
	float StaminaRegenMultiplier = 1.f;
	float StaminaEnableRegenAdditive = 1.f;
};
