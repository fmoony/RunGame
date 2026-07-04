#include "Character/RunGameMovementComponent.h"
#include "Character/RunGameInputBufferComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Character/RunGameCharacter.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "RunGame.h"

URunGameMovementComponent::URunGameMovementComponent()
{
	bOrientRotationToMovement = false;
	bUseControllerDesiredRotation = true;
}

void URunGameMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxWalkSpeed = BaseMaxWalkSpeed;
	SmoothedMaxWalkSpeed = BaseMaxWalkSpeed;
	DefaultGroundFriction = GroundFriction;
	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();

	// 初始化 DesireRotation 为角色当前朝向——否则会被插值拉歪 Initialize DesireRotation to current facing
	if (ACharacter* Owner = CharacterOwner.Get())
	{
		DesireRotation = Owner->GetActorRotation();
	}

	BindOwnerEvents();

	// 绑状态机——运动状态自行响应 Bind state machine — movement self-reacts
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddDynamic(this, &URunGameMovementComponent::OnCharacterStateChanged);
	}
}

void URunGameMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindOwnerEvents();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CoyoteTimer);

		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->OnCharacterStateChanged.RemoveDynamic(this, &URunGameMovementComponent::OnCharacterStateChanged);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void URunGameMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 速度曲线 + 复合乘数 → 平滑 → MaxWalkSpeed Speed curve + composite multiplier → smooth → MaxWalkSpeed
	float DesiredMaxSpeed = BaseMaxWalkSpeed;
	if (MaxSpeedCurve && TimerSubsystem && TimerSubsystem->IsTimerRunning())
	{
		DesiredMaxSpeed = MaxSpeedCurve->GetFloatValue(TimerSubsystem->GetTotalTimeSeconds());
	}
	const float TargetSpeed = DesiredMaxSpeed * CachedCompositeSpeedMultiplier;
	const float InterpSpeed = 3.0f / FMath::Max(SpeedTransitionDuration, 0.01f);
	SmoothedMaxWalkSpeed = FMath::FInterpTo(SmoothedMaxWalkSpeed, TargetSpeed, DeltaTime, InterpSpeed);
	MaxWalkSpeed = SmoothedMaxWalkSpeed;

	// 自动前进 Auto-forward
	ACharacter* Owner = CharacterOwner.Get();
	if (!Owner) return;

	AController* Controller = Owner->GetController();
	if (!Controller) return;

	const FRotator YawRotation(0, DesireRotation.Yaw, 0);
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	Owner->AddMovementInput(ForwardDir, 1.0f);

	// 方向修正 Direction correction
	UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!PRS) return;

	const ERunGameCharacterState State = PRS->GetCharacterState();

	// 非滑铲时：控制器朝向平滑插值到 DesireRotation Normal: smoothly interpolate controller toward DesireRotation
	// 滑铲时：跳过插值，锁定当前朝向 Slide: skip interpolation, lock current facing
	if (State != ERunGameCharacterState::Sliding)
	{
		const FRotator CurrentRotation = Controller->GetControlRotation();
		const FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, DesireRotation, DeltaTime, 10.f);
		Controller->SetControlRotation(SmoothRotation);
	}
}

// ---- State reactions ----

void URunGameMovementComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	ACharacter* Owner = CharacterOwner.Get();
	if (!Owner) return;

	if (NewState == ERunGameCharacterState::Sliding)
	{
		Owner->Crouch();
		GroundFriction = 0.0f;
	}

	if (OldState == ERunGameCharacterState::Sliding)
	{
		Owner->UnCrouch();
		GroundFriction = DefaultGroundFriction;
	}

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

	if (OldState == ERunGameCharacterState::CoyoteTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(CoyoteTimer);
	}

	if (NewState == ERunGameCharacterState::Idle)
	{
		bAirJumpAvailable = true;
		bJumpLaunchPending = false;
		PendingJumpStartState = ERunGameCharacterState::Idle;
	}

	if (NewState == ERunGameCharacterState::Dead)
	{
		bTurn = false;
		bInTurnBox = false;
		bAirJumpAvailable = false;
		bJumpLaunchPending = false;
		PendingJumpStartState = ERunGameCharacterState::Dead;
		SpeedModifiers.Empty();
		CachedCompositeSpeedMultiplier = 1.0f;

		GetWorld()->GetTimerManager().ClearTimer(CoyoteTimer);

		SetMovementMode(MOVE_None);
		DisableMovement();
	}

	// 从 Dead 恢复 → 重启移动 + 复原碰撞 Leaving Dead → re-enable movement + restore collision
	if (OldState == ERunGameCharacterState::Dead)
	{
		UE_LOG(LogRunGame, Warning, TEXT("MovementComponent: Re-enabling movement + resetting facing"));
		SetMovementMode(MOVE_Walking);

		// 重生时重置朝向 — 对齐 Actor 当前旋转（由 SpawnPlayer 设置）
		// Reset facing on respawn — align with Actor's current rotation (set by SpawnPlayer)
		if (Owner)
		{
			DesireRotation = Owner->GetActorRotation();
		}
	}
}

void URunGameMovementComponent::BindOwnerEvents()
{
	ARunGameCharacter* Owner = Cast<ARunGameCharacter>(CharacterOwner.Get());
	if (!Owner) return;

	// Character 只广播事件；MovementComponent 订阅并处理移动域规则
	// Character only broadcasts events; MovementComponent subscribes and owns movement-domain rules
	Owner->OnInputCommandReady.AddUObject(this, &URunGameMovementComponent::OnInputCommandReady);
	Owner->OnJumpInputReleased.AddUObject(this, &URunGameMovementComponent::OnJumpInputReleased);
	Owner->OnCharacterJumped.AddUObject(this, &URunGameMovementComponent::HandleOwnerJumped);
	Owner->OnCharacterLanded.AddUObject(this, &URunGameMovementComponent::HandleOwnerLanded);
	Owner->CanStartJumpQuery.BindUObject(this, &URunGameMovementComponent::CanStartJump);
}

void URunGameMovementComponent::UnbindOwnerEvents()
{
	ARunGameCharacter* Owner = Cast<ARunGameCharacter>(CharacterOwner.Get());
	if (!Owner) return;

	Owner->OnInputCommandReady.RemoveAll(this);
	Owner->OnJumpInputReleased.RemoveAll(this);
	Owner->OnCharacterJumped.RemoveAll(this);
	Owner->OnCharacterLanded.RemoveAll(this);
	Owner->CanStartJumpQuery.Unbind();
}

void URunGameMovementComponent::OnInputCommandReady(FRunGameInputCommandRequest& Request)
{
	switch (Request.Command)
	{
	case ERunGameInputCommand::Jump:
		if (TryStartJump())
		{
			Request.MarkHandled();
		}
		break;

	case ERunGameInputCommand::Slide:
		if (!IsFalling())
		{
			if (RequestCharacterState(ERunGameCharacterState::Sliding))
			{
				Request.MarkHandled();
			}
		}
		break;

	default:
		break;
	}
}

void URunGameMovementComponent::OnJumpInputReleased()
{
	if (ACharacter* Owner = CharacterOwner.Get())
	{
		Owner->StopJumping();
	}
}

// ---- Speed modifiers ----

void URunGameMovementComponent::AddSpeedModifier(FGameplayTag ModifierTag, float Multiplier)
{
	if (float* Existing = SpeedModifiers.Find(ModifierTag))
	{
		CachedCompositeSpeedMultiplier /= *Existing;
	}
	SpeedModifiers.Add(ModifierTag, Multiplier);
	CachedCompositeSpeedMultiplier *= Multiplier;
}

void URunGameMovementComponent::RemoveSpeedModifier(FGameplayTag ModifierTag)
{
	if (float* Existing = SpeedModifiers.Find(ModifierTag))
	{
		CachedCompositeSpeedMultiplier /= *Existing;
	}
	SpeedModifiers.Remove(ModifierTag);
}

// ---- Jump rules ----

bool URunGameMovementComponent::TryStartJump()
{
	ARunGameCharacter* Owner = Cast<ARunGameCharacter>(CharacterOwner);
	if (!Owner || !Owner->CanJump())
	{
		return false;
	}

	PendingJumpStartState = GetRuntimeCharacterState();
	bJumpLaunchPending = true;
	Owner->Jump();
	return true;
}

bool URunGameMovementComponent::CanStartJump(bool bDefaultCanJump) const
{
	if (bDefaultCanJump)
	{
		return true;
	}

	const ERunGameCharacterState State = GetRuntimeCharacterState();
	if (State == ERunGameCharacterState::CoyoteTime)
	{
		return true;
	}

	return State == ERunGameCharacterState::Airborne && bAirJumpAvailable;
}

void URunGameMovementComponent::HandleOwnerJumped()
{
	if (PendingJumpStartState == ERunGameCharacterState::CoyoteTime)
	{
		// 土狼跳仍算一段跳，保留二段跳资格 / Coyote jump is still the first jump, keep double-jump available
		bAirJumpAvailable = true;
		RequestCharacterState(ERunGameCharacterState::Airborne);
	}
	else if (PendingJumpStartState == ERunGameCharacterState::Airborne)
	{
		// 只有真正的二段跳才消耗空中跳资格 / Only a real double jump consumes air-jump availability
		bAirJumpAvailable = false;
	}
	else if (PendingJumpStartState == ERunGameCharacterState::Idle
		|| PendingJumpStartState == ERunGameCharacterState::Turning)
	{
		// 主动地面跳直接进入 Airborne，并保留二段跳资格 / Intentional ground jump enters Airborne and keeps double-jump available
		bAirJumpAvailable = true;
		RequestCharacterState(ERunGameCharacterState::Airborne);
	}

	bJumpLaunchPending = false;
}

void URunGameMovementComponent::HandleOwnerLanded(const FHitResult& /*Hit*/)
{
	bAirJumpAvailable = true;
	bJumpLaunchPending = false;
	PendingJumpStartState = ERunGameCharacterState::Idle;

	const ERunGameCharacterState State = GetRuntimeCharacterState();
	if (State == ERunGameCharacterState::Airborne || State == ERunGameCharacterState::CoyoteTime)
	{
		RequestCharacterState(ERunGameCharacterState::Idle);
	}
}

ERunGameCharacterState URunGameMovementComponent::GetRuntimeCharacterState() const
{
	const UWorld* World = GetWorld();
	const UPlayerRuntimeState* PRS = World ? World->GetSubsystem<UPlayerRuntimeState>() : nullptr;
	return PRS ? PRS->GetCharacterState() : ERunGameCharacterState::Idle;
}

bool URunGameMovementComponent::RequestCharacterState(ERunGameCharacterState NewState) const
{
	UWorld* World = GetWorld();
	UPlayerRuntimeState* PRS = World ? World->GetSubsystem<UPlayerRuntimeState>() : nullptr;
	if (!PRS)
	{
		return false;
	}

	if (PRS->GetCharacterState() == NewState)
	{
		return true;
	}

	PRS->SetCharacterState(NewState);
	return PRS->GetCharacterState() == NewState;
}

bool URunGameMovementComponent::ConsumePendingJumpLaunch()
{
	if (!bJumpLaunchPending)
	{
		return false;
	}

	bJumpLaunchPending = false;
	return true;
}

// ---- Movement mode event-driven airborne detection ----

void URunGameMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	const EMovementMode OldMode = MovementMode;
	Super::SetMovementMode(NewMovementMode, NewCustomMode);

	// 事件驱动：进入 Falling → CoyoteTime（土狼时间缓冲，而非直接 Airborne）
	// Event-driven: entering Falling → CoyoteTime (grace period, not immediate Airborne)
	if (OldMode != MOVE_Falling && NewMovementMode == MOVE_Falling)
	{
		const ERunGameCharacterState CurrentState = GetRuntimeCharacterState();
		if (CurrentState == ERunGameCharacterState::Idle || CurrentState == ERunGameCharacterState::Turning)
		{
			if (ConsumePendingJumpLaunch())
			{
				// 主动跳跃进入空中，不开启土狼时间 / Intentional jump enters Airborne without arming coyote time
				RequestCharacterState(ERunGameCharacterState::Airborne);
				GetWorld()->GetTimerManager().ClearTimer(CoyoteTimer);
				return;
			}

			RequestCharacterState(ERunGameCharacterState::CoyoteTime);

			// 启动 CoyoteTime 定时器——到期后自动转入 Airborne
			// Arm coyote timer — auto-transition to Airborne on expiry
			GetWorld()->GetTimerManager().SetTimer(
				CoyoteTimer,
				this,
				&URunGameMovementComponent::OnCoyoteTimeExpired,
				CoyoteTimeDuration,
				false
			);
		}
	}

	// 着陆检测——清除 CoyoteTimer 和空中跳跃标志 Landing detection — clear coyote timer + air jump flag
	if (OldMode == MOVE_Falling && NewMovementMode != MOVE_Falling)
	{
		GetWorld()->GetTimerManager().ClearTimer(CoyoteTimer);
	}
}

void URunGameMovementComponent::OnCoyoteTimeExpired()
{
	if (GetRuntimeCharacterState() == ERunGameCharacterState::CoyoteTime)
	{
		RequestCharacterState(ERunGameCharacterState::Airborne);
	}
}

// ---- Turn ----

bool URunGameMovementComponent::ApplyTurnRotation(float Right)
{
	if (!bTurn) return false;

	const FRotator NewRotation(0.f, 90.f * Right, 0.f);
	const FQuat QuatB(NewRotation);
	DesireRotation = FRotator(FQuat(DesireRotation) * QuatB);
	Velocity = QuatB.RotateVector(Velocity);
	bTurn = false;

	return bInTurnBox;
}
