#include "RunGameCharacter.h"
#include "RunGamePlayerController.h"
#include "Actor/Component/HealthComponent.h"
#include "Actor/Component/SkillComponent.h"
#include "Skill/RunGameSkillConfigData.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
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
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

	PrimaryActorTick.bCanEverTick = true;

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

	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		// 响应角色状态变化：自身逻辑 + 转发给蓝图监听者 Reactive to state: self logic + forward to BP listeners
		RS->OnCharacterStateChanged.AddDynamic(this, &ARunGameCharacter::OnCharacterStateChangedCallback);
		RS->OnCharacterStateChanged.AddDynamic(this, &ARunGameCharacter::OnRSCharacterStateChanged);
	}

	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &ARunGameCharacter::OnGameStateChanged);
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ARunGameCharacter::OnHealthDepleted);
		HealthComponent->OnDamageTaken.AddDynamic(this, &ARunGameCharacter::OnHitReaction);
	}
}

void ARunGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DissolveTimerHandle);

		if (UPlayerRuntimeState* RS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			RS->OnCharacterStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnCharacterStateChangedCallback);
			RS->OnCharacterStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnRSCharacterStateChanged);
		}

		if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
		{
			GS->OnGameStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnGameStateChanged);
		}
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.RemoveDynamic(this, &ARunGameCharacter::OnHealthDepleted);
		HealthComponent->OnDamageTaken.RemoveDynamic(this, &ARunGameCharacter::OnHitReaction);
	}

	Super::EndPlay(EndPlayReason);
}

void ARunGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunGameCharacter::Move);

		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ARunGameCharacter::StartSlide);

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

	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

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

	// 空中检测 Airborne detection
	if (GetCharacterMovement()->IsFalling() && RS->GetCharacterState() == ERunGameCharacterState::Idle)
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

			if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
			{
				FVector CurrentVelocity = MovementComponent->Velocity;
				FVector NewVelocity = QuatB.RotateVector(CurrentVelocity);
				MovementComponent->Velocity = NewVelocity;
			}

			bTurn = false;
		}
		else
		{
			if (!bInTurnBox)
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
	SetCharacterState(ERunGameCharacterState::Sliding);

	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		if (RS->GetCharacterState() != ERunGameCharacterState::Sliding)
		{
			PendingInputState = ERunGameCharacterState::Sliding;
		}
	}
}

void ARunGameCharacter::OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == SlideMontage)
	{
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
	// 设置角色状态为 Dead（RS 负责广播）Set character state to Dead (RS broadcasts)
	SetCharacterState(ERunGameCharacterState::Dead);

	// Die() 仅处理角色自身的视觉呈现：摄像机、动画、溶解
	// GameOver 由 GameMode 监听 OnCharacterDied 响应式设置
	// Die() only handles character presentation: camera, animation, dissolve
	// GameOver is set reactively by GameMode listening to OnCharacterDied

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

	// Broadcast via RS + own delegate
	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->OnCharacterDied.Broadcast(DamageType, this);
	}
	OnCharacterDied.Broadcast(DamageType, this);

	UAnimMontage** FoundMontage = DeathMontages.Find(DamageType);
	UAnimMontage* DeathMontage = FoundMontage ? *FoundMontage : nullptr;

	UE_LOG(LogRunGame, Warning, TEXT("Die: DamageType=%s, DeathMontage=%s, AnimInstance=%s"),
		*DamageType.ToString(),
		*GetNameSafe(DeathMontage),
		*GetNameSafe(GetMesh()->GetAnimInstance()));

	if (DeathMontage && GetMesh()->GetAnimInstance())
	{
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

// ---- GameState 响应 ----

void ARunGameCharacter::OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState)
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
	default:
		break;
	}
}

// ---- RS 转发 ----

void ARunGameCharacter::OnRSCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	// 转发 RS 的状态变化到自身委托（向后兼容外部蓝图监听）Forward RS state to own delegate for BP listeners
	OnCharacterStateChanged.Broadcast(OldState, NewState);
}

void ARunGameCharacter::ActivateSkillByTag(FGameplayTag SkillTag)
{
	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

	const ERunGameCharacterState CurrentState = RS->GetCharacterState();
	if (CurrentState == ERunGameCharacterState::Dead
		|| CurrentState == ERunGameCharacterState::Sliding)
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

// -- State Machine --

void ARunGameCharacter::SetCharacterState(ERunGameCharacterState NewState)
{
	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

	const ERunGameCharacterState CurrentState = RS->GetCharacterState();

	if (CurrentState == NewState)
	{
		return;
	}

	if (!IsCharacterStateTransitionAllowed(NewState))
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Blocked invalid state transition from %d to %d"),
			static_cast<int32>(CurrentState), static_cast<int32>(NewState));
		return;
	}

	if (NewState == ERunGameCharacterState::Sliding && GetCharacterMovement()->IsFalling())
	{
		return;
	}

	// 转移校验通过，写入 RS（RS 负责广播）Validation passed, write to RS (RS broadcasts)
	RS->SetCharacterState(NewState);
}

bool ARunGameCharacter::IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const
{
	const UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return false;

	return RS->IsCharacterStateTransitionAllowed(NewState);
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

	// === 转弯系统：本地旗帜管理 Turn system: local flags ===
	if (NewState == ERunGameCharacterState::Turning)
	{
		bTurn = true;
		bInTurnBox = true;
	}
	if (OldState == ERunGameCharacterState::Turning)
	{
		bTurn = false;
		bInTurnBox = false;
	}

	// === 死亡：清除缓冲输入 + 停止动画 Death: clear buffered input + stop all montages ===
	if (NewState == ERunGameCharacterState::Dead)
	{
		bTurn = false;
		bInTurnBox = false;
		SpeedModifiers.Empty();
		CachedCompositeSpeedMultiplier = 1.0f;

		PendingInputState = ERunGameCharacterState::MAX;

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->StopAllMontages(0.0f);
		}
	}
}

void ARunGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

	if (RS->GetCharacterState() == ERunGameCharacterState::Airborne)
	{
		SetCharacterState(ERunGameCharacterState::Idle);

		if (PendingInputState == ERunGameCharacterState::Sliding)
		{
			PendingInputState = ERunGameCharacterState::MAX;
			SetCharacterState(ERunGameCharacterState::Sliding);
		}
	}
}

ERunGameCharacterState ARunGameCharacter::GetCharacterState() const
{
	if (const UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		return RS->GetCharacterState();
	}
	return ERunGameCharacterState::Idle;
}

bool ARunGameCharacter::IsSliding() const
{
	return GetCharacterState() == ERunGameCharacterState::Sliding;
}

float ARunGameCharacter::GetCompositeSpeedMultiplier() const
{
	return CachedCompositeSpeedMultiplier;
}

// -- Speed modifiers —

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
