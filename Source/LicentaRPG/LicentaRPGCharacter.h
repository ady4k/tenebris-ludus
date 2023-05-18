#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** User Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* MainHUD;

	/** Character Stats */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	class UCharacterStats* CharacterStats;

	/** Combat System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	class UCombatSystem* CombatSystem;

public:
	ALicentaRPGCharacter();

	UFUNCTION(BlueprintCallable, Category = "CharacterStats")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Difficulty")
	void ChangeDifficultyMultipliers();

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



	UFUNCTION(BlueprintImplementableEvent, Category = "Physics")
	void OnCharacterDeath();

private:
	// -------- STAMINA SYSTEM -------- //
	bool IsOutOfStamina(float const Offset) const;
	bool HasMaximumStamina() const;
	void UpdateLastStaminaUsageTime();
	void EnableStaminaRegen();
	void DisableStaminaRegen();
	void RegenStamina() const;

	// -------- MOVEMENT -------- //
	bool IsCharacterMoving() const;
	void SetMaxWalkSpeed(float const Speed) const;
	bool IsCharacterOnGround() const;

	// -------- FALL DAMAGE -------- //
	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	float CalculateFallDamage(float const OnLandingVelocity, float const FallingDistance, float const PlayerMaxHealth) const;

	// -------- COMBAT SYSTEM -------- //
	void InvokeAttack();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// To add mapping context
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

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
	float StaminaEnableRegenMultiplier = 1.f;

// -------- CONSTANTS -------- //
private:
	// -------- MOVEMENT -------- //
	float const MaxWalkSpeed = 300.0f;
	float const MaxSprintSpeed = 500.0f;
	float const MaxCrouchWalkSpeed = 200.0f;
	float const MaxCrouchSprintSpeed = 300.0f;

	// -------- STAMINA REGEN -------- //
	float const EnableStaminaRegenDelay = 3.0f;
	float const StaminaRegenDelay = 0.1f;
	float const StaminaRegenAmount = 2.5f;

	// ------- STAMINA COSTS ------- //
	float const CrouchSprintStaminaCost = 10.0f;
	float const SprintStaminaCost = 15.0f;
	float const JumpStaminaCost = 20.0f;

	// -------- FALL DAMAGE -------- //
	float const FallDistanceThreshold = 600.0f;
	float const LandingVelocityThreshold = 1300.0f;
	float const FallDamageMultiplier = 0.0001f;
};
