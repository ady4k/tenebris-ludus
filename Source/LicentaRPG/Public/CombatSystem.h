// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <EnemyCharacter.h>

#include "CombatSystem.generated.h"


class ALicentaRPGCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LICENTARPG_API UCombatSystem : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	ALicentaRPGCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
    AEnemyCharacter* EnemyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TArray<float> DamageValues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	int32 DamageValueIndex;

public:	
	// Sets default values for this component's properties
	UCombatSystem();

	UFUNCTION(BlueprintCallable, Category = "Melee Attack")
	void StartAttack();

	void ResetAttack();

	bool GetIsAttacking() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Melee Attack")
	void SwordAttackCombo();

	UFUNCTION(BlueprintCallable, Category = "Melee Attack")
	void SwordEndCombo();

	UFUNCTION(BlueprintCallable, Category = "Melee Attack")
	void StartSwordTrace();

	UFUNCTION(BlueprintCallable, Category = "Melee Attack")
	void StopSwordTrace();

	UFUNCTION(BlueprintImplementableEvent, Category = "Melee Attack")
	void SwordTrace();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Misc Attack")
	void SphereTrace();

private:
	void SwordAttack();

	void PlayAttackMontage();
	void CheckArraysIndexes();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	bool IsAttacking = false;
	bool CanAttack = true;
	bool SaveAttack = false;

	int32 AttackIndex = 0;

	float AttackStaminaCost = 10;

	FTimerHandle TraceTimerHandle;
	float const TraceTimer = 0.01f;
};
