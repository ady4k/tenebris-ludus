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
	void SetEnemyHealthMultiplier(float Multiplier);
	void SetStaminaRegenMultiplier(float Multiplier);
	void SetStaminaEnableRegenMultiplier(float Multiplier);

	float GetEnemyDamageMultiplier() const;
	float GetEnemyHealthMultiplier() const;
	float GetStaminaRegenMultiplier() const;
	float GetStaminaEnableRegenMultiplier() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	int DifficultyLevel = 1;

	float EnemyDamageMultiplier = 1.f;
	float EnemyHealthMultiplier = 1.f;
	float StaminaRegenMultiplier = 1.f;
	float StaminaEnableRegenMultiplier = 1.f;
};
