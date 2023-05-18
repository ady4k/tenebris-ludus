// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem.h"

#include "LicentaRPG/LicentaRPGCharacter.h"


// Sets default values for this component's properties
UCombatSystem::UCombatSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




// Called when the game starts
void UCombatSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCombatSystem::StartAttack()
{
	if (CanAttack == true)
	{
		SwordAttack();
	}
}

void UCombatSystem::SwordAttack()
{
	if (IsAttacking)
	{
		SaveAttack = true;
	}
	else
	{
		PlayAttackMontage();
	}
}

void UCombatSystem::SwordAttackCombo()
{
	if (SaveAttack)
	{
		SaveAttack = false;
		PlayAttackMontage();
	}
	else
	{
		SwordEndCombo();
	}
}

void UCombatSystem::SwordEndCombo()
{
	IsAttacking = false;
	AttackIndex = 0;
}

void UCombatSystem::PlayAttackMontage()
{
	CheckMontageIndex();
	if (AActor* OwningActor = GetOwner(); OwningActor != nullptr)
	{
		if (ALicentaRPGCharacter* ParentActor = Cast<ALicentaRPGCharacter>(OwningActor); ParentActor != nullptr)
		{
			UAnimMontage* MontageToPlay = AttackMontages[AttackIndex];
			ParentActor->PlayAnimMontage(MontageToPlay, 1.0);
		}
	}
	AttackIndex++;
}

void UCombatSystem::CheckMontageIndex()
{
	AttackIndex = AttackIndex % AttackMontages.Num();
}

// Called every frame
void UCombatSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

