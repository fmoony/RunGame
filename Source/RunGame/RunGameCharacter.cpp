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

	CurrentCharacterState = ERunGameCharacterState::Idle;
	PendingInputState = ERunGameCharacterState::MAX;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
}

void ARunGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;
	BaseMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SmoothedMaxWalkSpeed = BaseMaxWalkSpeed;

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

	// 监听自身状态变化 —— 滑动/转弯等系统响应式执行
	OnCharacterStateChanged.AddDynamic(this, &ARunGameCharacter::OnCharacterStateChangedCallback);

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

	if (HealthComponent)
	{
		HealthComponent->OnDeath.RemoveDynamic(this, &ARunGameCharacter::OnHealthDepleted);
		HealthComponent->OnDamageTaken.RemoveDynamic(this, &ARunGameCharacter::OnHitReaction);
	}

	OnCharacterStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnCharacterStateChangedCallback);

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
		const float TargetSpeed = DesiredMaxSpeed * CachedCompositeSpeedMultiplier;
		const float InterpSpeed = 3.0f / FMath::Max(SpeedTransitionDuration, 0.01f);
		SmoothedMaxWalkSpeed = FMath::FInterpTo(SmoothedMaxWalkSpeed, TargetSpeed, DeltaSeconds, InterpSpeed);
		GetCharacterMovement()->MaxWalkSpeed = SmoothedMaxWalkSpeed;
	}

	FRotator CurrentRotation = GetController()->GetControlRotation();
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, DesireRotation, DeltaSeconds, 10.f);
	GetController()->SetControlRotation(SmoothRotation);

	const FRotator YawRotation(0, DesireRotation.Yaw, 0);
	const FVector Desiron = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Desiron, 1.0f);

	// 空中检测：落地由 Landed() 处理，离地在此检测
	if (GetCharacterMovement()->IsFalling() && CurrentCharacterState == ERunGameCharacterState::Idle)
	{
		SetCharacterState(ERunGameCharacterState::Airborne);
	}

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
	// 纯状态请求 —— 滑动逻辑在 OnCharacterStateChangedCallback 中响应式执行
	SetCharacterState(ERunGameCharacterState::Sliding);

	// 如果转移被拒绝（如在 Airborne / Turning / Dead），缓冲输入
	if (CurrentCharacterState != ERunGameCharacterState::Sliding)
	{
		PendingInputState = ERunGameCharacterState::Sliding;
	}
}

void ARunGameCharacter::OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == SlideMontage)
	{
		// 请求回到 Idle —— 滑动清理在 OnCharacterStateChangedCallback 中响应式执行
		SetCharacterState(ERunGameCharacterState::Idle);
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
	// 状态机守卫：防止重复执行死亡逻辑
	SetCharacterState(ERunGameCharacterState::Dead);

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
	// 死亡或滑铲中不能使用技能
	if (CurrentCharacterState == ERunGameCharacterState::Dead
		|| CurrentCharacterState == ERunGameCharacterState::Sliding)
	{
		return;
	}

	if (SkillComponent)
	{
		SkillComponent->TryActivateSkill(SkillTag);
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
	return CurrentCharacterState == ERunGameCharacterState::Dead;
}

// ~end IDamagable interface

// -- State Machine --

void ARunGameCharacter::SetCharacterState(ERunGameCharacterState NewState)
{
	// Guard：相同状态直接返回
	if (CurrentCharacterState == NewState)
	{
		return;
	}

	// Guard：转移校验
	if (!IsCharacterStateTransitionAllowed(NewState))
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Blocked invalid state transition from %d to %d"),
			static_cast<int32>(CurrentCharacterState), static_cast<int32>(NewState));
		return;
	}

	// Runtime guard：不能在非地面时进入 Sliding
	if (NewState == ERunGameCharacterState::Sliding && GetCharacterMovement()->IsFalling())
	{
		return;
	}

	// Save old → Modify → Broadcast（纯状态管理，无副作用）
	const ERunGameCharacterState OldState = CurrentCharacterState;
	CurrentCharacterState = NewState;
	OnCharacterStateChanged.Broadcast(OldState, CurrentCharacterState);

	UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Character State Changed from %d to %d"),
		static_cast<int32>(OldState), static_cast<int32>(NewState));
}

bool ARunGameCharacter::IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const
{
	// Dead 是终态 —— 不能离开
	if (CurrentCharacterState == ERunGameCharacterState::Dead)
	{
		return false;
	}

	switch (NewState)
	{
	case ERunGameCharacterState::Idle:
		// 可以从 Sliding、Airborne 或 Turning 回到 Idle
		return CurrentCharacterState == ERunGameCharacterState::Sliding
			|| CurrentCharacterState == ERunGameCharacterState::Airborne
			|| CurrentCharacterState == ERunGameCharacterState::Turning;

	case ERunGameCharacterState::Airborne:
		// 只能从 Idle 或 Turning 进入空中（Sliding 先由回调清理再进入空中）
		return CurrentCharacterState == ERunGameCharacterState::Idle
			|| CurrentCharacterState == ERunGameCharacterState::Turning;

	case ERunGameCharacterState::Sliding:
		// 只能从 Idle 进入滑铲
		return CurrentCharacterState == ERunGameCharacterState::Idle;

	case ERunGameCharacterState::Turning:
		// 可以从 Idle、Sliding 或 Airborne 进入转弯
		return CurrentCharacterState == ERunGameCharacterState::Idle
			|| CurrentCharacterState == ERunGameCharacterState::Sliding
			|| CurrentCharacterState == ERunGameCharacterState::Airborne;

	case ERunGameCharacterState::Dead:
		// 可以从任何非 Dead 状态进入死亡
		return CurrentCharacterState != ERunGameCharacterState::Dead;

	default:
		return false;
	}
}

void ARunGameCharacter::OnCharacterStateChangedCallback(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	// === 滑铲系统：进入 Sliding ===
	if (NewState == ERunGameCharacterState::Sliding)
	{
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

	// === 滑铲系统：离开 Sliding ===
	if (OldState == ERunGameCharacterState::Sliding)
	{
		UnCrouch();
		GetCharacterMovement()->GroundFriction = DefaultGroundFriction;
		AnimRootMotionTranslationScale = 1.0f;

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ARunGameCharacter::OnSlideBlendingOut);
		}
		UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Slide ended."));
	}

	// === 转弯系统：进入 Turning ===
	if (NewState == ERunGameCharacterState::Turning)
	{
		bTurn = true;
		InTurnBox = true;
	}

	// === 转弯系统：离开 Turning ===
	if (OldState == ERunGameCharacterState::Turning)
	{
		bTurn = false;
		InTurnBox = false;
	}

	// === 死亡：清理转弯标志 ===
	if (NewState == ERunGameCharacterState::Dead)
	{
		bTurn = false;
		InTurnBox = false;
	}
}

void ARunGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (CurrentCharacterState == ERunGameCharacterState::Airborne)
	{
		SetCharacterState(ERunGameCharacterState::Idle);

		// 输入缓冲：落地后执行缓冲的输入
		if (PendingInputState == ERunGameCharacterState::Sliding)
		{
			PendingInputState = ERunGameCharacterState::MAX;
			SetCharacterState(ERunGameCharacterState::Sliding);
		}
	}
}

// -- Speed modifiers --

void ARunGameCharacter::AddSpeedModifier(FGameplayTag ModifierTag, float Multiplier)
{
	if (float* Existing = SpeedModifiers.Find(ModifierTag))
	{
		CachedCompositeSpeedMultiplier /= *Existing;
	}
	SpeedModifiers.Add(ModifierTag, Multiplier);
	CachedCompositeSpeedMultiplier *= Multiplier;
}

void ARunGameCharacter::RemoveSpeedModifier(FGameplayTag ModifierTag)
{
	if (float* Existing = SpeedModifiers.Find(ModifierTag))
	{
		CachedCompositeSpeedMultiplier /= *Existing;
	}
	SpeedModifiers.Remove(ModifierTag);
}