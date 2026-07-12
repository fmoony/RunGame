#include "Character/Locomotion/RunGameLocomotionComponent.h"

#include "Character/Animation/RunGameAnimInstance.h"
#include "Character/Locomotion/Movement/RunGameMovementComponent.h"
#include "Character/RunGameCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"

URunGameLocomotionComponent::URunGameLocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameLocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	MovementComponent = OwnerCharacter ? OwnerCharacter->GetRunGameMovementComponent() : nullptr;

	if (MovementComponent)
	{
		MovementModeChangedHandle = MovementComponent->OnRunGameMovementModeChanged.AddUObject(
			this,
			&URunGameLocomotionComponent::HandleMovementModeChanged);
	}

	if (UWorld* World = GetWorld())
	{
		RuntimeState = World->GetSubsystem<UPlayerRuntimeState>();
		if (RuntimeState)
		{
			RuntimeState->OnCharacterStateChanged.AddDynamic(this, &URunGameLocomotionComponent::OnCharacterStateChanged);
		}
	}

	BindAnimationEvents();
}

void URunGameLocomotionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindAnimationEvents();

	if (MovementComponent && MovementModeChangedHandle.IsValid())
	{
		MovementComponent->OnRunGameMovementModeChanged.Remove(MovementModeChangedHandle);
		MovementModeChangedHandle.Reset();
	}

	if (RuntimeState)
	{
		RuntimeState->OnCharacterStateChanged.RemoveDynamic(this, &URunGameLocomotionComponent::OnCharacterStateChanged);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CoyoteTimer);
	}

	PendingSignals.Reset();
	OwnerCharacter = nullptr;
	MovementComponent = nullptr;
	AnimInstance = nullptr;
	RuntimeState = nullptr;
	Super::EndPlay(EndPlayReason);
}

ERunGameCharacterState URunGameLocomotionComponent::GetRuntimeCharacterState() const
{
	return RuntimeState ? RuntimeState->GetCharacterState() : ERunGameCharacterState::Idle;
}

bool URunGameLocomotionComponent::CanExecuteJump() const
{
	const ERunGameCharacterState State = GetRuntimeCharacterState();
	const bool bStateAllowsJump = State == ERunGameCharacterState::Idle
		|| State == ERunGameCharacterState::Turning
		|| State == ERunGameCharacterState::CoyoteTime
		|| State == ERunGameCharacterState::Airborne;

	return bStateAllowsJump && OwnerCharacter && OwnerCharacter->CanJump();
}

bool URunGameLocomotionComponent::CanExecuteSlide() const
{
	return MovementComponent
		&& !MovementComponent->IsFalling()
		&& GetRuntimeCharacterState() == ERunGameCharacterState::Idle;
}

void URunGameLocomotionComponent::NotifyJumpRequested(ERunGameCharacterState PreviousState)
{
	PendingJumpStartState = PreviousState;
	bJumpLaunchPending = true;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CoyoteTimer);
	}
}

void URunGameLocomotionComponent::HandleOwnerJumped()
{
	if (!bJumpLaunchPending)
	{
		return;
	}

	// 只有空中起跳消耗二段跳资格；地面和土狼跳仍保留一次空中跳。
	// Only an airborne jump consumes the extra jump; ground and coyote jumps keep it available.
	bAirJumpAvailable = PendingJumpStartState != ERunGameCharacterState::Airborne;
	bJumpLaunchPending = false;
}

bool URunGameLocomotionComponent::CanStartJump(bool bDefaultCanJump) const
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

void URunGameLocomotionComponent::HandleOwnerLanded(const FHitResult& /*Hit*/)
{
	bJumpLaunchPending = false;
	PendingSignals.Add(ERunGameLocomotionSignal::Landed);
}

bool URunGameLocomotionComponent::DequeueSignal(ERunGameLocomotionSignal& OutSignal)
{
	if (PendingSignals.IsEmpty())
	{
		return false;
	}

	OutSignal = PendingSignals[0];
	PendingSignals.RemoveAt(0);
	return true;
}

void URunGameLocomotionComponent::ClearSignals()
{
	PendingSignals.Reset();
}

bool URunGameLocomotionComponent::EvaluateSignal(
	ERunGameLocomotionSignal Signal,
	ERunGameCharacterState CurrentState,
	ERunGameCharacterState& OutNewState) const
{
	switch (Signal)
	{
	case ERunGameLocomotionSignal::Landed:
		if (CurrentState == ERunGameCharacterState::Airborne
			|| CurrentState == ERunGameCharacterState::CoyoteTime)
		{
			OutNewState = ERunGameCharacterState::Idle;
			return true;
		}
		break;

	case ERunGameLocomotionSignal::StartedFalling:
		if (CurrentState == ERunGameCharacterState::Idle
			|| CurrentState == ERunGameCharacterState::Turning)
		{
			OutNewState = ERunGameCharacterState::CoyoteTime;
			return true;
		}
		break;

	case ERunGameLocomotionSignal::CoyoteExpired:
		if (CurrentState == ERunGameCharacterState::CoyoteTime)
		{
			OutNewState = ERunGameCharacterState::Airborne;
			return true;
		}
		break;

	case ERunGameLocomotionSignal::SlideEnded:
		if (CurrentState == ERunGameCharacterState::Sliding)
		{
			OutNewState = ERunGameCharacterState::Idle;
			return true;
		}
		break;

	default:
		break;
	}

	return false;
}

void URunGameLocomotionComponent::BindAnimationEvents()
{
	if (!OwnerCharacter || SlideMontageEndedHandle.IsValid())
	{
		return;
	}

	if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
	{
		AnimInstance = Cast<URunGameAnimInstance>(Mesh->GetAnimInstance());
	}

	if (AnimInstance)
	{
		SlideMontageEndedHandle = AnimInstance->OnSlideMontageEnded.AddUObject(
			this,
			&URunGameLocomotionComponent::HandleSlideMontageEnded);
	}
}

void URunGameLocomotionComponent::UnbindAnimationEvents()
{
	if (AnimInstance && SlideMontageEndedHandle.IsValid())
	{
		AnimInstance->OnSlideMontageEnded.Remove(SlideMontageEndedHandle);
		SlideMontageEndedHandle.Reset();
	}
}

void URunGameLocomotionComponent::HandleSlideMontageEnded(UAnimMontage* /*Montage*/, bool /*bInterrupted*/)
{
	PendingSignals.Add(ERunGameLocomotionSignal::SlideEnded);
}

void URunGameLocomotionComponent::HandleMovementModeChanged(
	EMovementMode OldMovementMode,
	EMovementMode NewMovementMode)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (OldMovementMode != MOVE_Falling && NewMovementMode == MOVE_Falling)
	{
		const ERunGameCharacterState State = GetRuntimeCharacterState();
		if (State == ERunGameCharacterState::Idle || State == ERunGameCharacterState::Turning)
		{
			PendingSignals.Add(ERunGameLocomotionSignal::StartedFalling);
			World->GetTimerManager().SetTimer(
				CoyoteTimer,
				this,
				&URunGameLocomotionComponent::OnCoyoteTimeExpired,
				CoyoteTimeDuration,
				false);
		}
	}

	if (OldMovementMode == MOVE_Falling && NewMovementMode != MOVE_Falling)
	{
		World->GetTimerManager().ClearTimer(CoyoteTimer);
	}
}

void URunGameLocomotionComponent::OnCoyoteTimeExpired()
{
	PendingSignals.Add(ERunGameLocomotionSignal::CoyoteExpired);
}

void URunGameLocomotionComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	UWorld* World = GetWorld();

	if (NewState == ERunGameCharacterState::Sliding)
	{
		BindAnimationEvents();
	}

	if (OldState == ERunGameCharacterState::CoyoteTime && World)
	{
		World->GetTimerManager().ClearTimer(CoyoteTimer);
	}

	if (NewState == ERunGameCharacterState::Idle)
	{
		bAirJumpAvailable = true;
		bJumpLaunchPending = false;
		PendingJumpStartState = ERunGameCharacterState::Idle;
	}

	if (NewState == ERunGameCharacterState::Dead)
	{
		bAirJumpAvailable = false;
		bJumpLaunchPending = false;
		PendingJumpStartState = ERunGameCharacterState::Dead;
		PendingSignals.Reset();
		if (World)
		{
			World->GetTimerManager().ClearTimer(CoyoteTimer);
		}
	}
}
