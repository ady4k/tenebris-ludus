#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/StatsEnum.h"
#include "InputActionValue.h"
#include "CharacterStats.h"
#include "CombatSystem.h"
#include "LicentaRPGGameMode.h"

#include "LicentaRPGCharacter.generated.h"

UCLASS(config=Game)
class ALicentaRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
	ALicentaRPGGameMode* GameModeInstance;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;


	// All the input actions that the character can perform
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SecondaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuickSaveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuickLoadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleStatsAction;

	/** User Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	UUserWidget* MainHUD;

	/** Character Stats */
	UPROPERTY()
	UCharacterStats* CharacterStats;

	/** Combat System */
	UPROPERTY()
	UCombatSystem* CombatSystem;

public:
	ALicentaRPGCharacter();

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Difficulty")
	void ChangeDifficultyMultipliers();

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool CheckIfMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void PickupHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	bool CheckIfMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void PickupMana() const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void DecreaseStamina(const float StaminaCost);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseStat(EStatsSelection StatType) const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetCharacterStats(UCharacterStats* Stats);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetCombatSystem(UCombatSystem* Combat);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	UCharacterStats* GetCharacterStats() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	UCombatSystem* GetCombatSystem() const;

protected:
	virtual void Jump() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouching input */
	void Crouch();

	/** Called for sprinting input */
	void SprintStart();
	void SprintStop();

	/** Called for dodging input */
	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "Dodging")
	void OnRollEnd();

	void QuickSave();
	void QuickLoad();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void Pause();

	UFUNCTION(BlueprintImplementableEvent, Category = "Stats Menu")
	void ToggleStats();

	UFUNCTION(BlueprintImplementableEvent, Category = "Physics")
	void OnCharacterDeath();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void GainExperience(const float Experience);

private:
	// -------- STAMINA SYSTEM -------- //
	bool IsOutOfStamina(const float Offset) const;
	bool HasMaximumStamina() const;
	void UpdateStaminaRegenTimers();
	void EnableStaminaRegen();
	void DisableStaminaRegen();
	void RegenStamina() const;

	// -------- MOVEMENT -------- //
	bool IsCharacterMoving() const;
	void SetMaxWalkSpeed(const float Speed) const;
	bool IsCharacterOnGround() const;

	// -------- FALL DAMAGE -------- //
	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	float CalculateFallDamage(const float OnLandingVelocity, const float FallingDistance,
	                          const float PlayerMaxHealth) const;

	// -------- COMBAT SYSTEM -------- //
	void InvokeAttack();
	void DisableInvincibility();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// To add mapping context
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	// -------- MOVEMENT -------- //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool IsCrouched = false;
	bool IsSprinting = false;

	// -------- STAMINA REGEN -------- //
	FTimerHandle EnableStaminaRegenTimerHandle;
	FTimerHandle StaminaRegenTimerHandle;
	bool CanRegenStamina = true;

	// -------- FALL DAMAGE -------- //
	float InitialVerticalPosition = 0.0f;
	float LandingVerticalPosition = 0.0f;
	float FallDistance = 0.0f;
	float LandingVelocity = 0.0f;
	float FallDamage = 0.0f;

	// -------- DIFFICULTY -------- //
	float StaminaRegenMultiplier = 1.f;
	float StaminaEnableRegenAdditive = 1.f;
	float ExperienceGainMultiplier = 1.f;

	// -------- COMBAT SYSTEM -------- //
	FTimerHandle DisableInvincibilityTimerHandle;
	bool IsInvincible = false;

	// -------- DODGING -------- //
	bool IsDodging = false;
	bool ShouldRotate = false;
	float RotationTime = 0.1f;
	FRotator TargetRotation = FRotator(0, 0, 0);
	FRotator CurrentRotation = FRotator(0, 0, 0);

	// -------- MENU -------- //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool StatsMenuOpen = false;

	// -------- CONSTANTS -------- //
	// -------- MOVEMENT -------- //
	const float MaxWalkSpeed = 300.0f;
	const float MaxSprintSpeed = 500.0f;
	const float MaxCrouchWalkSpeed = 200.0f;
	const float MaxCrouchSprintSpeed = 300.0f;

	// -------- STAMINA REGEN -------- //
	const float EnableStaminaRegenDelay = 0.5f;
	const float StaminaRegenDelay = 0.1f;
	const float StaminaRegenAmount = 4.0f;

	// ------- STAMINA COSTS ------- //
	const float CrouchSprintStaminaCost = 7.0f;
	const float SprintStaminaCost = 10.0f;
	const float JumpStaminaCost = 15.0f;
	const float RollStaminaCost = 15.0f;

	// -------- FALL DAMAGE -------- //
	const float FallDistanceThreshold = 600.0f;
	const float LandingVelocityThreshold = 1300.0f;
	const float FallDamageMultiplier = 0.0001f;

	// -------- COMBAT SYSTEM -------- //
	const float InvincibilityTime = 0.5f;
};
