#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStats.h"

#include "LicentaRPGCharacter.generated.h"

UCLASS(config=Game)
class ALicentaRPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	/** User Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* MainHUD;

	/** Character Stats */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	class UCharacterStats* CharacterStats;

public:
	ALicentaRPGCharacter();

	UFUNCTION(BlueprintCallable, Category = "CharacterStats")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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
	bool IsOutOfStamina(float const Offset) const;
	bool HasMaximumStamina() const;
	void UpdateLastStaminaUsageTime();
	void EnableStaminaRegen();
	void DisableStaminaRegen();
	void RegenStamina() const;

	void SetMaxWalkSpeed(float const Speed) const;
	bool IsCharacterOnGround() const;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool IsCrouched = false;
	bool IsSprinting = false;

	FTimerHandle EnableStaminaRegenTimerHandle;
	FTimerHandle StaminaRegenTimerHandle;
	bool CanRegenStamina = true;

// -------- CONSTANTS -------- //
private:
	// -------- MOVEMENT -------- //
	const float MaxWalkSpeed = 300.0f;
	const float MaxSprintSpeed = 500.0f;
	const float MaxCrouchWalkSpeed = 200.0f;
	const float MaxCrouchSprintSpeed = 300.0f;

	// -------- STAMINA REGEN -------- //
	float const EnableStaminaRegenDelay = 3.0f;
	float const StaminaRegenDelay = 0.1f;
	float const StaminaRegenAmount = 2.5f;

	// ------- STAMINA COSTS ------- //
	float const CrouchSprintStaminaCost = 10.0f;
	float const SprintStaminaCost = 15.0f;
	float const JumpStaminaCost = 20.0f;
};
