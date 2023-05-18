// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem.h"

// Sets default values for this component's properties
UCombatSystem::UCombatSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCombatSystem::PlayAttackMontage(UAnimMontage* MontageToPlay, float const PlayRate)
{
}

// Called when the game starts
void UCombatSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCombatSystem::StartAttack()
{
	if (CanAttack == true && IsAttacking == false)
	{
		if (AttackIndex == 0)
		{
			SwordAttack();
		}
		else
		{
			SwordAttackCombo();
		}
	}
	
}

void UCombatSystem::SwordAttack()
{
	
	
}

void UCombatSystem::SwordAttackCombo()
{
}

void UCombatSystem::SwordEndCombo()
{
}


// Called every frame
void UCombatSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

