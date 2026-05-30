#include "Character/RunGameMovementComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Character/RunGameCharacter.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "RunGame.h"

URunGameMovementComponent::URunGameMovementComponent()
{
	bOrientRotationToMovement = true;
	bUseControllerDesiredRotation = false;
}

void URunGameMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	BaseMaxWalkSpeed = MaxWalkSpeed;
	SmoothedMaxWalkSpeed = MaxWalkSpeed;
	DefaultGroundFriction = GroundFriction;
	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();

	// 初始化 DesireRotation 为角色当前朝向——否则会被插值拉歪 Initialize DesireRotation to current facing
	if (ACharacter* Owner = CharacterOwner.Get())
	{
		DesireRotation = Owner->GetActorRotation();
	}

	// 绑状态机——运动状态自行响应 Bind state machine — movement self-reacts
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddDynamic(this, &URunGameMovementComponent::OnCharacterStateChanged);
	}
}

void URunGameMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
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

	// 方向修正 + 空中检测 Direction correction + airborne detection
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

	if (NewState == ERunGameCharacterState::Dead)
	{
		bTurn = false;
		bInTurnBox = false;
		SpeedModifiers.Empty();
		CachedCompositeSpeedMultiplier = 1.0f;

		SetMovementMode(MOVE_None);
		DisableMovement();
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

// ---- Movement mode event-driven airborne detection ----

void URunGameMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	const EMovementMode OldMode = MovementMode;
	Super::SetMovementMode(NewMovementMode, NewCustomMode);

	// 事件驱动：进入 Falling → Airborne。替代 Tick 中每帧 IsFalling 轮询
	if (OldMode != MOVE_Falling && NewMovementMode == MOVE_Falling)
	{
		UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
		if (PRS && PRS->GetCharacterState() == ERunGameCharacterState::Idle)
		{
			if (ARunGameCharacter* Char = Cast<ARunGameCharacter>(CharacterOwner))
			{
				Char->SetCharacterState(ERunGameCharacterState::Airborne);
			}
		}
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
