// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunGameCharacter.h"
#include "RunGamePlayerController.h"
#include "Actor/Component/HealthComponent.h"
#include "Actor/Component/SkillComponent.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "RunGame.h"
#include "RunGameGameState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Curves/CurveFloat.h"
#include "Materials/MaterialInstanceDynamic.h"

ARunGameCharacter::ARunGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.0f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

	PrimaryActorTick.bCanEverTick = true;
	bIsSliding = false;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
}

void ARunGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;
	BaseMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();

	if (ARunGameGameState* GameState = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GameState->OnGameStateChanged.AddDynamic(this, &ARunGameCharacter::OnGameStateChangedCallback);
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ARunGameCharacter::OnHealthDepleted);
		HealthComponent->OnDamageTaken.AddDynamic(this, &ARunGameCharacter::OnHitReaction);
	}

	if (SkillComponent)
	{
		SkillComponent->OnSkillExecuted.AddDynamic(this, &ARunGameCharacter::HandleSkillExecuted);
	}
}

void ARunGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DissolveTimerHandle);

		if (ARunGameGameState* GameState = World->GetGameState<ARunGameGameState>())
		{
			GameState->OnGameStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnGameStateChangedCallback);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ARunGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunGameCharacter::Move);

		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARunGameCharacter::Look);

		//Slide
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ARunGameCharacter::StartSlide);
		//EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Completed, this, &ARunGameCharacter::EndSlide);

		// Dynamic skill input bindings from SkillConfig data asset
		if (SkillComponent)
		{
			if (USkillConfigData* Config = SkillComponent->SkillConfig)
			{
				for (const FSkillDefinition& SkillDef : Config->Skills)
				{
					if (SkillDef.InputAction && SkillDef.SkillTag.IsValid())
					{
						EnhancedInputComponent->BindAction(
							SkillDef.InputAction,
							ETriggerEvent::Started,
							this,
							&ARunGameCharacter::ActivateSkillByTag,
							SkillDef.SkillTag
						);
					}
				}
			}
		}

	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameCharacter: '%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARunGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetController() == nullptr) return;

	if (MaxSpeedCurve && TimerSubsystem && TimerSubsystem->IsTimerRunning())
	{
		const float ElapsedTime = TimerSubsystem->GetTotalTimeSeconds();
		const float DesiredMaxSpeed = MaxSpeedCurve->GetFloatValue(ElapsedTime);
		GetCharacterMovement()->MaxWalkSpeed = DesiredMaxSpeed;
	}

	FRotator CurrentRotation = GetController()->GetControlRotation();
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, DesireRotation, DeltaSeconds, 10.f);
	GetController()->SetControlRotation(SmoothRotation);

	const FRotator YawRotation(0, DesireRotation.Yaw, 0);
	const FVector Desiron = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Desiron, 1.0f);

}

void ARunGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	DoMove(MovementVector.X, MovementVector.Y);
}

void ARunGameCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ARunGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr && Right != 0.0f)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		if (bTurn)
		{
			FRotator NewRotation = FRotator(0.f, 90.f * Right, 0.f);

			FQuat QuatA = FQuat(DesireRotation);
			FQuat QuatB = FQuat(NewRotation);
			DesireRotation = FRotator(QuatA * QuatB);

			if(UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
			{
				FVector CurrentVelocity = MovementComponent->Velocity;
				FVector NewVelocity = QuatB.RotateVector(CurrentVelocity);
				MovementComponent->Velocity = NewVelocity;
			}

			bTurn = false;
		}
		else
		{
			if (!InTurnBox)
			{
				AddMovementInput(RightDirection, Right);
			}
		}

	}
}

void ARunGameCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ARunGameCharacter::DoJumpStart()
{
	Jump();
}

void ARunGameCharacter::DoJumpEnd()
{
	StopJumping();
}

void ARunGameCharacter::StartSlide()
{
	if (!bIsSliding && !GetCharacterMovement()->IsFalling())
	{
		bIsSliding = true;

		Crouch();

		GetCharacterMovement()->GroundFriction = 0.0f;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &ARunGameCharacter::OnSlideBlendingOut);

			if (SlideMontage)
			{
				float FinalPlayRate = MontagePlayRate;
				if (MaxSpeedCurve && TimerSubsystem && TimerSubsystem->IsTimerRunning())
				{
					const float ElapsedTime = TimerSubsystem->GetTotalTimeSeconds();
					const float DesiredMaxSpeed = MaxSpeedCurve->GetFloatValue(ElapsedTime);
					FinalPlayRate = MontagePlayRate * DesiredMaxSpeed / BaseMaxWalkSpeed;
				}
				AnimRootMotionTranslationScale = RootMotionScale;
				AnimInstance->Montage_Play(SlideMontage, FinalPlayRate);
				UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Slide started. Playing montage: %s"), *SlideMontage->GetName());
			}
		}
	}
}
void ARunGameCharacter::EndSlide()
{
	if (bIsSliding)
	{
		bIsSliding = false;

		UnCrouch();

		GetCharacterMovement()->GroundFriction = DefaultGroundFriction;
		AnimRootMotionTranslationScale = 1.0f;
		UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Slide ended."));
	}
}

void ARunGameCharacter::OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == SlideMontage)
	{
		EndSlide();

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ARunGameCharacter::OnSlideBlendingOut);
		}
	}
}

void ARunGameCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	if (CameraBoom)
	{
		CameraBoom->TargetOffset.Z += ScaledHalfHeightAdjust;
	}
}

void ARunGameCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	if (CameraBoom)
	{
		CameraBoom->TargetOffset.Z -= ScaledHalfHeightAdjust;
	}
}

void ARunGameCharacter::Die(FGameplayTag DamageType, float DestroyDelay, AActor* DeathCauser)
{
	// Notify GameState of game over
	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->SetGameState(ERunGameGameState::GameOver);
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
	}

	// Spawn death camera before playing animation to avoid camera jitter
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		const FVector CamLocation = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation();
		const FRotator CamRotation = FollowCamera ? FollowCamera->GetComponentRotation() : GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (ACameraActor* DeathCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CamLocation, CamRotation, SpawnParams))
		{
			PC->SetViewTargetWithBlend(DeathCamera, 0.1f);
			PC->bAutoManageActiveCameraTarget = false;
			UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Death camera spawned at %s, view blended."), *CamLocation.ToString());
		}
	}

	// Broadcast to listeners (Controller handles SetInputModeToUIOnly, etc.)
	OnCharacterDied.Broadcast(DamageType, this);

	UAnimMontage** FoundMontage = DeathMontages.Find(DamageType);
	UAnimMontage* DeathMontage = FoundMontage ? *FoundMontage : nullptr;

	UE_LOG(LogRunGame, Warning, TEXT("Die: DamageType=%s, DeathMontage=%s, AnimInstance=%s"),
		*DamageType.ToString(),
		*GetNameSafe(DeathMontage),
		*GetNameSafe(GetMesh()->GetAnimInstance()));

	if (DeathMontage && GetMesh()->GetAnimInstance())
	{
		// Disable movement
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		GetCharacterMovement()->DisableMovement();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &ARunGameCharacter::OnDeathMontageBlendingOut);
		AnimInstance->Montage_Play(DeathMontage);
	}
	else
	{
		FTimerHandle DissolveDelayHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DissolveDelayHandle,
			[this]()
			{
				if (IsValid(this))
				{
					StartDissolve();
				}
			},
			DestroyDelay,
			false
		);
	}
}

void ARunGameCharacter::OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ARunGameCharacter::OnDeathMontageBlendingOut);
	}

	GetCharacterMovement()->StopMovementImmediately(); 

	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		SkelMesh->bPauseAnims = true;
		SkelMesh->bNoSkeletonUpdate = true;
	}

	StartDissolve();
}

void ARunGameCharacter::StartDissolve()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const int32 NumMaterials = GetMesh()->GetNumMaterials();
	for (int32 i = 0; i < NumMaterials; ++i)
	{
		if (UMaterialInterface* Mat = GetMesh()->GetMaterial(i))
		{
			UMaterialInstanceDynamic* DMI = UMaterialInstanceDynamic::Create(Mat, this);
			DissolveMaterials.Add(DMI);
			GetMesh()->SetMaterial(i, DMI);
		}
	}

	DissolveElapsed = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(
		DissolveTimerHandle,
		this,
		&ARunGameCharacter::TickDissolve,
		0.033f,
		true
	);
}

void ARunGameCharacter::TickDissolve()
{
	DissolveElapsed += 0.033f;
	const float Alpha = FMath::Clamp(DissolveElapsed / DissolveDuration, 0.0f, 1.0f);

	for (UMaterialInstanceDynamic* DMI : DissolveMaterials)
	{
		if (DMI)
		{
			DMI->SetScalarParameterValue(DissolveParameterName, Alpha);
		}
	}

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DissolveTimerHandle);
		Destroy();
	}
}

void ARunGameCharacter::OnHitReaction(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (HitReactionMontages.IsEmpty())
	{
		return;
	}

	UAnimMontage** Found = HitReactionMontages.Find(DamageType);
	UAnimMontage* Montage = Found ? *Found : nullptr;
	if (!Montage)
	{
		return;
	}

	PlayAnimMontage(Montage);
}

void ARunGameCharacter::OnHealthDepleted(FGameplayTag DamageType, AActor* DeathCauser)
{
	Die(DamageType, 3.0f, DeathCauser);
}

void ARunGameCharacter::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (OldState == NewState)
	{
		return;
	}

	switch (NewState)
	{
	case ERunGameGameState::MainMenu:
		Destroy();
		break;
	case ERunGameGameState::CountDown:
		break;
	case ERunGameGameState::InGame:
		break;
	case ERunGameGameState::Pause:
		break;
	case ERunGameGameState::GameOver:
		break;
	case ERunGameGameState::MAX:
		break;
	default:
		break;
	}
}

void ARunGameCharacter::ActivateSkillByTag(FGameplayTag SkillTag)
{
	if (SkillComponent)
	{
		SkillComponent->TryActivateSkill(SkillTag);
	}
}

void ARunGameCharacter::HandleSkillExecuted(FGameplayTag SkillTag)
{
	// Play montage if one is mapped for this skill tag
	if (UAnimMontage** FoundMontage = SkillMontages.Find(SkillTag))
	{
		PlayAnimMontage(*FoundMontage);
	}

	// Apply forward impulse if one is mapped for this skill tag
	if (float* Impulse = SkillImpulseStrengths.Find(SkillTag))
	{
		if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
		{
			MovementComp->AddImpulse(GetActorForwardVector() * (*Impulse), true);
		}
	}
}

// ~begin IDamagable interface

void ARunGameCharacter::OnTakeDamage_Implementation(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(Damage, DamageType, DamageCauser);
	}
}

void ARunGameCharacter::OnTakeHealing_Implementation(float HealAmount, AActor* Healer)
{
	if (HealthComponent)
	{
		HealthComponent->Heal(HealAmount, Healer);
	}
}

void ARunGameCharacter::OnDeath_Implementation(AActor* DeathCauser)
{
	// death logic is driven by HealthComponent::OnDeath delegate, bound to OnHealthDepleted
}

float ARunGameCharacter::GetCurrentHP_Implementation() const
{
	return HealthComponent ? HealthComponent->GetCurrentHP() : 0.0f;
}

float ARunGameCharacter::GetMaxHP_Implementation() const
{
	return HealthComponent ? HealthComponent->GetMaxHP() : 0.0f;
}

bool ARunGameCharacter::IsDead_Implementation() const
{
	return HealthComponent ? HealthComponent->IsDead() : false;
}

// ~end IDamagable interface
