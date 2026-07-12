#include "Character/Locomotion/Movement/RunGameMovementComponent.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Character/RunGameCharacter.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
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

	// 初始化期望朝向，避免第一帧控制器插值拉回默认方向 Initialize desired facing to avoid first-frame rotation pullback
	if (ACharacter* Owner = CharacterOwner.Get())
	{
		DesireRotation = Owner->GetActorRotation();
	}

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

void URunGameMovementComponent::ExecuteMoveInput(float Right)
{
	ACharacter* Owner = CharacterOwner.Get();
	if (!Owner || Right == 0.0f)
	{
		return;
	}

	if (!ApplyTurnRotation(Right))
	{
		const FRotator YawRotation(0.0f, DesireRotation.Yaw, 0.0f);
		Owner->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Right);
	}
}

bool URunGameMovementComponent::ExecuteJump()
{
	ACharacter* Owner = CharacterOwner.Get();
	if (!Owner || !Owner->CanJump())
	{
		return false;
	}

	Owner->Jump();
	return true;
}

void URunGameMovementComponent::ExecuteStopJumping()
{
	if (ACharacter* Owner = CharacterOwner.Get())
	{
		Owner->StopJumping();
	}
}

bool URunGameMovementComponent::ExecuteSlide()
{
	ACharacter* Owner = CharacterOwner.Get();
	if (!Owner || IsFalling())
	{
		return false;
	}

	Owner->Crouch();
	GroundFriction = 0.0f;
	return true;
}

void URunGameMovementComponent::ExecuteSlideEnd()
{
	if (ACharacter* Owner = CharacterOwner.Get())
	{
		Owner->UnCrouch();
	}
	GroundFriction = DefaultGroundFriction;
}

void URunGameMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float DesiredMaxSpeed = BaseMaxWalkSpeed;
	if (MaxSpeedCurve && TimerSubsystem && TimerSubsystem->IsTimerRunning())
	{
		DesiredMaxSpeed = MaxSpeedCurve->GetFloatValue(TimerSubsystem->GetTotalTimeSeconds());
	}

	const float TargetSpeed = DesiredMaxSpeed * CachedCompositeSpeedMultiplier;
	const float InterpSpeed = 3.0f / FMath::Max(SpeedTransitionDuration, 0.01f);
	SmoothedMaxWalkSpeed = FMath::FInterpTo(SmoothedMaxWalkSpeed, TargetSpeed, DeltaTime, InterpSpeed);
	MaxWalkSpeed = SmoothedMaxWalkSpeed;

	ACharacter* Owner = CharacterOwner.Get();
	if (!Owner)
	{
		return;
	}

	AController* Controller = Owner->GetController();
	if (!Controller)
	{
		return;
	}

	const FRotator YawRotation(0, DesireRotation.Yaw, 0);
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	Owner->AddMovementInput(ForwardDir, 1.0f);

}

void URunGameMovementComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	ACharacter* Owner = CharacterOwner.Get();
	if (!Owner)
	{
		return;
	}

	// 外部权威状态可能直接打断滑铲，确保物理设置不会遗留。
	// External authoritative state changes may interrupt a slide; restore its physical settings.
	if (OldState == ERunGameCharacterState::Sliding && NewState != ERunGameCharacterState::Sliding)
	{
		ExecuteSlideEnd();
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

	if (OldState == ERunGameCharacterState::Dead)
	{
		UE_LOG(LogRunGame, Warning, TEXT("MovementComponent: Re-enabling movement + resetting facing"));
		SetMovementMode(MOVE_Walking);
		DesireRotation = Owner->GetActorRotation();
	}
}

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

void URunGameMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	const EMovementMode OldMode = MovementMode;
	Super::SetMovementMode(NewMovementMode, NewCustomMode);

	if (OldMode != MovementMode)
	{
		OnRunGameMovementModeChanged.Broadcast(OldMode, MovementMode);
	}
}

bool URunGameMovementComponent::ApplyTurnRotation(float Right)
{
	if (!bTurn) return false;

	const float OldYaw = DesireRotation.Yaw;
	const FRotator NewRotation(0.f, 90.f * Right, 0.f);
	const FQuat QuatB(NewRotation);
	DesireRotation = FRotator(FQuat(DesireRotation) * QuatB);
	DesireRotation.Normalize();
	Velocity = QuatB.RotateVector(Velocity);
	bTurn = false;
	OnRunDirectionChanged.Broadcast(OldYaw, DesireRotation.Yaw);

	return bInTurnBox;
}
