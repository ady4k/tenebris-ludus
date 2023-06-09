// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem.h"

#include "Camera/CameraComponent.h"
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

	if (AActor* OwningActor = GetOwner(); OwningActor != nullptr)
	{
		// impelemntation done so if the main owner of the component is the player, the enemy character will be null and vice versa
		OwnerCharacter = Cast<ALicentaRPGCharacter>(OwningActor);
		if (OwnerCharacter == nullptr)
		{
			EnemyCharacter = Cast<AEnemyCharacter>(OwningActor);
		}
	}
	// ...
	
}

void UCombatSystem::StartAttack()
{
	if (CanAttack == true)
	{
		SwordAttack();
	}
}

void UCombatSystem::ResetAttack()
{
	SwordEndCombo();
}

bool UCombatSystem::GetIsAttacking() const
{
	return IsAttacking;
}

void UCombatSystem::SwordAttack()
{
	if (IsAttacking)
	{
		SaveAttack = true;
	}
	else
	{
		IsAttacking = true;
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
	AttackStaminaCost = 5;
	DamageValueIndex = 0;
}

void UCombatSystem::StartSwordTrace()
{
	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UCombatSystem::SwordTrace, TraceTimer, true);

}

void UCombatSystem::StopSwordTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
}

void UCombatSystem::PlayAttackMontage()
{
	UAnimMontage* MontageToPlay = AttackMontages[AttackIndex];
	if (OwnerCharacter != nullptr)
	{
		// Set the target rotation to the camera's yaw
		TargetRotation = FRotator(0.0f, OwnerCharacter->GetFollowCamera()->GetComponentRotation().Yaw, 0.0f);
		// Store the current rotation
		CurrentRotation = OwnerCharacter->GetActorRotation();

		// Set the flag and timer
		ShouldRotate = true;
		RotationTime = 0.1f;

		OwnerCharacter->PlayAnimMontage(MontageToPlay, 1.0);
		OwnerCharacter->DecreaseStamina(AttackStaminaCost);
	}
	else
	{
		EnemyCharacter->PlayAnimMontage(MontageToPlay, 1.0);
	}

	AttackStaminaCost += 3;
	DamageValueIndex++;
	AttackIndex++;

	CheckArraysIndexes();
}

void UCombatSystem::CheckArraysIndexes()
{
	DamageValueIndex = DamageValueIndex % DamageValues.Num();
	AttackIndex = AttackIndex % AttackMontages.Num();
}

// Called every frame
void UCombatSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldRotate)
	{
		// Update the rotation time
		RotationTime -= DeltaTime;
		if (RotationTime <= 0)
		{
			// Stop the rotation when the time is up
			ShouldRotate = false;
			CurrentRotation = TargetRotation;
		}
		else
		{
			// Calculate and apply the new rotation
			CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0f);
			OwnerCharacter->SetActorRotation(CurrentRotation);
		}
	}
}

