#include "LicentaRPGCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"

//////////////////////////////////////////////////////////////////////////
// ALicentaRPGCharacter
ALicentaRPGCharacter::ALicentaRPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}


void ALicentaRPGCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (CharacterStats == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterStats is null!"));
		CharacterStats = NewObject<UCharacterStats>(this, TEXT("CharacterStats"));
		CharacterStats->RegisterComponent();
	}

	if (CombatSystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CombatSystem is null!"));
		CombatSystem = NewObject<UCombatSystem>(this, TEXT("CombatSystem"));
		CombatSystem->RegisterComponent();
	}

	if (GetWorld())
	{
		GameModeInstance = Cast<ALicentaRPGGameMode>(GetWorld()->GetAuthGameMode());
	}

	if (GameModeInstance)
	{
		ChangeDifficultyMultipliers();
	}
}

void ALicentaRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSprinting)
	{
		if (!IsOutOfStamina(0.1f))
		{
			const float StaminaCost = IsCrouched ? CrouchSprintStaminaCost : SprintStaminaCost;
			if (IsCharacterOnGround() && IsCharacterMoving())
			{
				DecreaseStamina(StaminaCost * DeltaTime);
			}
		}
		else
		{
			SprintStop();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALicentaRPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALicentaRPGCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALicentaRPGCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALicentaRPGCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ALicentaRPGCharacter::Crouch);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
		                                   &ALicentaRPGCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
		                                   &ALicentaRPGCharacter::SprintStop);

		// Attacking
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this,
		                                   &ALicentaRPGCharacter::InvokeAttack);

		// Dodging
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ALicentaRPGCharacter::Dodge);

		// Saving
		EnhancedInputComponent->BindAction(QuickSaveAction, ETriggerEvent::Started, this,
		                                   &ALicentaRPGCharacter::QuickSave);
		EnhancedInputComponent->BindAction(QuickLoadAction, ETriggerEvent::Started, this,
		                                   &ALicentaRPGCharacter::QuickLoad);

		// Pausing
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ALicentaRPGCharacter::Pause);

		// Toggle Stats Menu
		EnhancedInputComponent->BindAction(ToggleStatsAction, ETriggerEvent::Started, this,
					                                   &ALicentaRPGCharacter::ToggleStats);
	}
}

void ALicentaRPGCharacter::Jump()
{
	if (!IsOutOfStamina(JumpStaminaCost) && IsCharacterOnGround() && !IsCrouched)
	{
		Super::Jump();
		DecreaseStamina(JumpStaminaCost);
		if (CanRegenStamina)
		{
			DisableStaminaRegen();
		}
	}
}

void ALicentaRPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALicentaRPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALicentaRPGCharacter::Crouch()
{
	IsCrouched = !IsCrouched;
	if (IsSprinting)
	{
		IsCrouched ? SetMaxWalkSpeed(MaxCrouchSprintSpeed) : SetMaxWalkSpeed(MaxSprintSpeed);
	}
	else
	{
		IsCrouched ? SetMaxWalkSpeed(MaxCrouchWalkSpeed) : SetMaxWalkSpeed(MaxWalkSpeed);
	}
}

void ALicentaRPGCharacter::SprintStart()
{
	if (!IsOutOfStamina(1))
	{
		IsCrouched ? SetMaxWalkSpeed(MaxCrouchSprintSpeed) : SetMaxWalkSpeed(MaxSprintSpeed);
		IsSprinting = true;
		DisableStaminaRegen();
	}
}

void ALicentaRPGCharacter::SprintStop()
{
	IsCrouched ? SetMaxWalkSpeed(MaxCrouchWalkSpeed) : SetMaxWalkSpeed(MaxWalkSpeed);
	IsSprinting = false;
}

void ALicentaRPGCharacter::Dodge()
{
	if (!IsDodging && !IsOutOfStamina(RollStaminaCost))
	{
		IsDodging = true;
		IsInvincible = true;
		CharacterStats->DecreaseStamina(RollStaminaCost);
		DisableStaminaRegen();
		if (CombatSystem->GetIsAttacking())
		{
			CombatSystem->ResetAttack();
		}
		PlayAnimMontage(DodgeAnimation, 1.0f);
	}
}

void ALicentaRPGCharacter::QuickSave()
{
	GameModeInstance->SaveGame("quick-save");
}

void ALicentaRPGCharacter::QuickLoad()
{
	GameModeInstance->LoadGame("quick-save");
}


// ---------------------------------------------------------
// Dodge System
// ---------------------------------------------------------

void ALicentaRPGCharacter::OnRollEnd()
{
	DisableInvincibility();
	IsDodging = false;
}

// ---------------------------------------------------------
// Character Stats
// ---------------------------------------------------------

void ALicentaRPGCharacter::GainExperience(const float Experience)
{
	const float ExperienceGained = Experience * ExperienceGainMultiplier;
	CharacterStats->IncreaseExperience(ExperienceGained);
}


// ---------------------------------------------------------
// Stamina System
// ---------------------------------------------------------

bool ALicentaRPGCharacter::IsOutOfStamina(const float Offset) const
{
	return CharacterStats->GetCurrentStamina() <= 0.0f + Offset;
}

bool ALicentaRPGCharacter::HasMaximumStamina() const
{
	return CharacterStats->GetCurrentStamina() >= CharacterStats->GetMaxStamina();
}

void ALicentaRPGCharacter::UpdateStaminaRegenTimers()
{
	GetWorld()->GetTimerManager().ClearTimer(EnableStaminaRegenTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(EnableStaminaRegenTimerHandle, this,
	                                       &ALicentaRPGCharacter::EnableStaminaRegen,
	                                       EnableStaminaRegenDelay + StaminaEnableRegenAdditive, false);
}

void ALicentaRPGCharacter::EnableStaminaRegen()
{
	CanRegenStamina = true;
	GetWorldTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ALicentaRPGCharacter::RegenStamina,
	                                StaminaRegenDelay, true);
}

void ALicentaRPGCharacter::DisableStaminaRegen()
{
	CanRegenStamina = false;
	GetWorldTimerManager().ClearTimer(StaminaRegenTimerHandle);
}

void ALicentaRPGCharacter::RegenStamina() const
{
	if (CanRegenStamina && !HasMaximumStamina())
	{
		CharacterStats->IncreaseStamina(StaminaRegenAmount * StaminaRegenMultiplier);
	}
}

void ALicentaRPGCharacter::DecreaseStamina(const float StaminaCost)
{
	CharacterStats->DecreaseStamina(StaminaCost);
	UpdateStaminaRegenTimers();
}

void ALicentaRPGCharacter::IncreaseStat(const EStatsSelection StatType) const
{
	CharacterStats->IncreaseStats(StatType);
}

void ALicentaRPGCharacter::SetCharacterStats(UCharacterStats* Stats)
{
	CharacterStats = Stats;
}

void ALicentaRPGCharacter::SetCombatSystem(UCombatSystem* Combat)
{
	CombatSystem = Combat;
}

UCharacterStats* ALicentaRPGCharacter::GetCharacterStats() const
{
	return CharacterStats;
}

UCombatSystem* ALicentaRPGCharacter::GetCombatSystem() const
{
	return CombatSystem;
}

bool ALicentaRPGCharacter::IsCharacterMoving() const
{
	return GetVelocity().Size() > 0.0f;
}

void ALicentaRPGCharacter::SetMaxWalkSpeed(const float Speed) const
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

bool ALicentaRPGCharacter::IsCharacterOnGround() const
{
	return !GetCharacterMovement()->IsFalling();
}

bool ALicentaRPGCharacter::CheckIfMaxHealth() const
{
	return CharacterStats->GetCurrentHealth() >= CharacterStats->GetMaxHealth();
}

void ALicentaRPGCharacter::PickupHealth() const
{
	CharacterStats->IncreaseHealth(CharacterStats->GetMaxHealth() / 3);
}

bool ALicentaRPGCharacter::CheckIfMaxMana() const
{
	return CharacterStats->GetCurrentMana() >= CharacterStats->GetMaxMana();
}

void ALicentaRPGCharacter::PickupMana() const
{
	CharacterStats->IncreaseMana(CharacterStats->GetMaxMana() / 3);
}


// ---------------------------------------------------------
// Fall Damage System
// ---------------------------------------------------------

void ALicentaRPGCharacter::Landed(const FHitResult& Hit)
{
	OnLanded(Hit);
	LandingVelocity = GetVelocity().Size();
}

void ALicentaRPGCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		InitialVerticalPosition = GetActorLocation().Z;
		IsCrouched = false;
	}
	if (PrevMovementMode == MOVE_Falling)
	{
		LandingVerticalPosition = GetActorLocation().Z;
		FallDistance = InitialVerticalPosition - LandingVerticalPosition;
		if (LandingVelocity > LandingVelocityThreshold && FallDistance > FallDistanceThreshold)
		{
			const float PlayerMaxHealth = CharacterStats->GetMaxHealth();
			FallDamage = CalculateFallDamage(LandingVelocity, FallDistance, PlayerMaxHealth);
			TakeDamage(FallDamage, FDamageEvent(), nullptr, nullptr);
		}
	}
}

float ALicentaRPGCharacter::CalculateFallDamage(const float OnLandingVelocity, const float FallingDistance,
                                                const float PlayerMaxHealth) const
{
	const float MaxHealthMultiplier = PlayerMaxHealth / 100 + ((PlayerMaxHealth / 100) * 0.66);
	const float Damage = (OnLandingVelocity - LandingVelocityThreshold) * (FallingDistance - FallDistanceThreshold) *
		FallDamageMultiplier * MaxHealthMultiplier;
	return Damage;
}


// ---------------------------------------------------------
// Difficulty System
// ---------------------------------------------------------

void ALicentaRPGCharacter::ChangeDifficultyMultipliers()
{
	StaminaRegenMultiplier = GameModeInstance->GetDifficultyManager()->GetStaminaRegenMultiplier();
	StaminaEnableRegenAdditive = GameModeInstance->GetDifficultyManager()->GetStaminaEnableRegenAdditive();
	ExperienceGainMultiplier = GameModeInstance->GetDifficultyManager()->GetExperienceGainMultiplier();
}



// ---------------------------------------------------------
// Damage
// ---------------------------------------------------------

float ALicentaRPGCharacter::TakeDamage(const float DamageAmount, const FDamageEvent& DamageEvent,
                                       AController* EventInstigator,
                                       AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsInvincible == false)
	{
		CharacterStats->DecreaseHealth(ActualDamage);
		if (CharacterStats->GetCurrentHealth() <= 0.f)
		{
			OnCharacterDeath();
		}
		IsInvincible = true;
		GetWorldTimerManager().SetTimer(DisableInvincibilityTimerHandle, this,
		                                &ALicentaRPGCharacter::DisableInvincibility, InvincibilityTime, false);
	}

	return ActualDamage;
}


// ---------------------------------------------------------
// Combat System
// ---------------------------------------------------------

void ALicentaRPGCharacter::InvokeAttack()
{
	if (!IsOutOfStamina(5.f) && IsCharacterOnGround() && !IsDodging)
	{
		CombatSystem->StartAttack();
		DisableStaminaRegen();
	}
}

void ALicentaRPGCharacter::DisableInvincibility()
{
	IsInvincible = false;
}
