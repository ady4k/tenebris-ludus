// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CombatSystem.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LICENTARPG_API UCombatSystem : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackMontages;

public:	
	// Sets default values for this component's properties
	UCombatSystem();

	UFUNCTION(BlueprintCallable, Category = "Attacking")
	void SwordAttackCombo();

	UFUNCTION(BlueprintCallable, Category = "Attacking")
	void SwordEndCombo();

private:
	void StartAttack();
	void SwordAttack();

	void PlayAttackMontage();
	void CheckMontageIndex();

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
};
