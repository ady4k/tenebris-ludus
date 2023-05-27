// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "CombatSystem.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CharacterStats = CreateDefaultSubobject<UCharacterStats>(TEXT("EnemyCharacterStats"));

	CombatSystem = CreateDefaultSubobject<UCombatSystem>(TEXT("EnemyCombatSystem"));
}



// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		GameModeInstance = Cast<ALicentaRPGGameMode>(GetWorld()->GetAuthGameMode());
	}
	if (GameModeInstance)
	{
		ChangeDifficultyMultipliers();
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// ---------------------------------------------------------
// Health Regen System
// ---------------------------------------------------------

void AEnemyCharacter::UpdateHealthRegenTimers()
{
	GetWorldTimerManager().ClearTimer(EnableHealthRegenTimerHandle);
	GetWorldTimerManager().SetTimer(EnableHealthRegenTimerHandle, this, &AEnemyCharacter::EnableHealthRegen, EnableHealthRegenDelay + HealthRegenDelayAdditive, false);
}

void AEnemyCharacter::EnableHealthRegen()
{
	CanRegenHealth = true;
	GetWorldTimerManager().SetTimer(HealthRegenTimerHandle, this, &AEnemyCharacter::RegenHealth, HealthRegenDelay, true);
}

void AEnemyCharacter::DisableHealthRegen()
{
	CanRegenHealth = false;
	GetWorldTimerManager().ClearTimer(HealthRegenTimerHandle);
}

void AEnemyCharacter::RegenHealth()
{
	if (CanRegenHealth)
	{
		CharacterStats->IncreaseHealth(HealthRegenAmount * HealthRegenMultiplier);
		if (CharacterStats->GetCurrentHealth() >= CharacterStats->GetMaxHealth())
		{
			DisableHealthRegen();
		}
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

	float const ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsInvincible == false)
	{
		GetAndPlayHitReactMontage();

		CharacterStats->DecreaseHealth(ActualDamage * DamageReductionMultiplier);
		if (CharacterStats->GetCurrentHealth() <= 0.f)
		{
			OnCharacterDeath();
			IsInvincible = true;
			return ActualDamage;
		}

		UpdateHealthRegenTimers();
		if (CanRegenHealth == true)
		{
			DisableHealthRegen();
		}

		IsInvincible = true;
		GetWorldTimerManager().SetTimer(DisableInvincibilityTimerHandle, this, &AEnemyCharacter::DisableInvincibility, InvincibilityTime, false);

		ApplyCharacterKnockback();
	}

	return ActualDamage;
}

void AEnemyCharacter::GetAndPlayHitReactMontage()
{
	int32 const RandomIndex = FMath::RandRange(0, HitReactMontages.Num() - 1);
	UAnimMontage* AnimMontageToPlay = HitReactMontages[RandomIndex];
	PlayAnimMontage(AnimMontageToPlay, 1.0f);

}

void AEnemyCharacter::DisableInvincibility()
{
	IsInvincible = false;
}

void AEnemyCharacter::ApplyCharacterKnockback()
{
	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController(); PlayerController != nullptr)
	{
		if (const ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn()); PlayerCharacter != nullptr)
		{
			const FRotator ActorRotation = PlayerCharacter->GetActorRotation();
			const FVector ForwardVector = ActorRotation.Vector();
			LaunchCharacter(ForwardVector * KnockbackForce, true, true);
		}
	}
}

void AEnemyCharacter::DestroyActor()
{
	Destroy();
}

void AEnemyCharacter::ChangeDifficultyMultipliers()
{
	DamageMultiplier = GameModeInstance->GetDifficultyManager()->GetEnemyDamageMultiplier();
	DamageReductionMultiplier = GameModeInstance->GetDifficultyManager()->GetEnemyDamageReductionMultiplier();
	HealthRegenMultiplier = GameModeInstance->GetDifficultyManager()->GetEnemyHealthRegenMultiplier();
	HealthRegenDelayAdditive = GameModeInstance->GetDifficultyManager()->GetEnemyHealthRegenDelayAdditive();

	float const MovementSpeedMultiplier = GameModeInstance->GetDifficultyManager()->GetEnemyMovementSpeedMultiplier();
	MaxMovementSpeed = BaseMaxMovementSpeed * MovementSpeedMultiplier;
}
