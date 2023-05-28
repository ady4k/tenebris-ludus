// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../LicentaRPGGameMode.h"
#include "CharacterStats.h"

#include "EnemyCharacter.generated.h"

class UCombatSystem;

UCLASS()
class LICENTARPG_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
	ALicentaRPGGameMode* GameModeInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	UCharacterStats* CharacterStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> HitReactMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	UCombatSystem* CombatSystem;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Difficulty")
	void ChangeDifficultyMultipliers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Physics")
	void OnCharacterDeath();

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void DestroyActor();


private:
	// -------- DAMAGE -------- //
	void GetAndPlayHitReactMontage();

	// -------- HEALTH REGEN SYSTEM -------- //
	void UpdateHealthRegenTimers();
	void EnableHealthRegen();
	void DisableHealthRegen();
	void RegenHealth();

	void DisableInvincibility();

	// -------- COMBAT -------- //
	void ApplyCharacterKnockback();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FTimerHandle EnableHealthRegenTimerHandle;
	FTimerHandle HealthRegenTimerHandle;
	bool CanRegenHealth = true;

	FTimerHandle DisableInvincibilityTimerHandle;
	bool IsInvincible = false;

	// -------- DIFFICULTY -------- //
	float DamageMultiplier = 1.f;
	float DamageReductionMultiplier = 1.f;
	float HealthRegenMultiplier = 1.f;
	float HealthRegenDelayAdditive = 0.f;

	// -------- MOVEMENT -------- //
	float MaxMovementSpeed = 250.0f;

	// -------- COMBAT -------- //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackSpeed = 1.2f;

// -------- CONSTANTS -------- //
	// -------- MOVEMENT -------- //
	float const BaseMaxMovementSpeed = 250.0f;

	// -------- HEALTH REGEN -------- //
	float const EnableHealthRegenDelay = 10.0f;
	float const HealthRegenDelay = 0.5f;
	float const HealthRegenAmount = 1.0f;

	// -------- COMBAT -------- //
	float const InvincibilityTime = 0.5f;
	float const KnockbackForce = 750.f;
};
