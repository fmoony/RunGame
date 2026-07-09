#include "Character/Locomotion/RunGameLocomotionComponent.h"
#include "Character/Animation/RunGameAnimInstance.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameMovementComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

URunGameLocomotionComponent::URunGameLocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameLocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	MovementComponent = OwnerCharacter
		? OwnerCharacter->GetRunGameMovementComponent()
		: nullptr;

	if (MovementComponent && !MovementModeChangedHandle.IsValid())
	{
		MovementModeChangedHandle = MovementComponent->OnRunGameMovementModeChanged.AddUObject(
			this,
			&URunGameLocomotionComponent::HandleMovementModeChanged
		);
	}

	if (UWorld* World = GetWorld())
	{
		if (RuntimeState = World->GetSubsystem<UPlayerRuntimeState>())
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

	OwnerCharacter = nullptr;
	MovementComponent = nullptr;
	AnimInstance = nullptr;
	RuntimeState = nullptr;

	Super::EndPlay(EndPlayReason);
}

ERunGameCharacterState URunGameLocomotionComponent::GetRuntimeCharacterState() const
{
	return RuntimeState
		? RuntimeState->GetCharacterState()
		: ERunGameCharacterState::Idle;
}

bool URunGameLocomotionComponent::TryConsumeInputCommand(ERunGameInputCommand Command)
{
	switch (Command)
	{
	case ERunGameInputCommand::Jump:
		return TryStartJump();

	case ERunGameInputCommand::Slide:
		if (MovementComponent && !MovementComponent->IsFalling())
		{
			return RequestCharacterState(ERunGameCharacterState::Sliding);
		}
		return false;

	default:
		return false;
	}
}

void URunGameLocomotionComponent::HandleJumpInputReleased() const
{
	if (OwnerCharacter)
	{
		OwnerCharacter->StopJumping();
	}
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

void URunGameLocomotionComponent::HandleOwnerJumped()
{
	if (PendingJumpStartState == ERunGameCharacterState::CoyoteTime)
	{
		// 土狼跳仍算一段跳，保留二段跳资格 Coyote jump is still the first jump, keep double-jump available
		bAirJumpAvailable = true;
		RequestCharacterState(ERunGameCharacterState::Airborne);
	}
	else if (PendingJumpStartState == ERunGameCharacterState::Airborne)
	{
		// 只有真正的二段跳才消耗空中跳资格 Only a real double jump consumes air-jump availability
		bAirJumpAvailable = false;
	}
	else if (PendingJumpStartState == ERunGameCharacterState::Idle
		|| PendingJumpStartState == ERunGameCharacterState::Turning)
	{
		// 主动地面跳直接进入 Airborne，并保留二段跳资格 Intentional ground jump enters Airborne and keeps double-jump available
		bAirJumpAvailable = true;
		RequestCharacterState(ERunGameCharacterState::Airborne);
	}

	bJumpLaunchPending = false;
}

void URunGameLocomotionComponent::HandleOwnerLanded(const FHitResult& /*Hit*/)
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

bool URunGameLocomotionComponent::ApplyTurnRotation(float Right) const
{
	return MovementComponent && MovementComponent->ApplyTurnRotation(Right);
}

bool URunGameLocomotionComponent::TryStartJump()
{
	if (!OwnerCharacter || !OwnerCharacter->CanJump())
	{
		return false;
	}

	PendingJumpStartState = GetRuntimeCharacterState();
	bJumpLaunchPending = true;
	OwnerCharacter->Jump();
	return true;
}

bool URunGameLocomotionComponent::RequestCharacterState(ERunGameCharacterState NewState) const
{
	if (!RuntimeState)
	{
		return false;
	}

	if (RuntimeState->GetCharacterState() == NewState)
	{
		return true;
	}

	return RuntimeState->TrySetCharacterState(NewState);
}

bool URunGameLocomotionComponent::ConsumePendingJumpLaunch()
{
	if (!bJumpLaunchPending)
	{
		return false;
	}

	bJumpLaunchPending = false;
	return true;
}

void URunGameLocomotionComponent::BindAnimationEvents()
{
	if (!OwnerCharacter || SlideMontageEndedHandle.IsValid())
	{
		return;
	}

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh)
	{
		return;
	}

	AnimInstance = Cast<URunGameAnimInstance>(Mesh->GetAnimInstance());
	if (AnimInstance)
	{
		SlideMontageEndedHandle = AnimInstance->OnSlideMontageEnded.AddUObject(
			this,
			&URunGameLocomotionComponent::HandleSlideMontageEnded
		);
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
	if (GetRuntimeCharacterState() == ERunGameCharacterState::Sliding)
	{
		RequestCharacterState(ERunGameCharacterState::Idle);
	}
}

void URunGameLocomotionComponent::HandleMovementModeChanged(EMovementMode OldMovementMode, EMovementMode NewMovementMode)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (OldMovementMode != MOVE_Falling && NewMovementMode == MOVE_Falling)
	{
		const ERunGameCharacterState CurrentState = GetRuntimeCharacterState();
		if (CurrentState == ERunGameCharacterState::Idle || CurrentState == ERunGameCharacterState::Turning)
		{
			if (ConsumePendingJumpLaunch())
			{
				// 主动跳跃进入空中，不开启土狼时间 Intentional jump enters Airborne without arming coyote time
				RequestCharacterState(ERunGameCharacterState::Airborne);
				World->GetTimerManager().ClearTimer(CoyoteTimer);
				return;
			}

			RequestCharacterState(ERunGameCharacterState::CoyoteTime);

			// 走出边缘时开启土狼时间窗口 Walking off ledge arms the coyote time window
			World->GetTimerManager().SetTimer(
				CoyoteTimer,
				this,
				&URunGameLocomotionComponent::OnCoyoteTimeExpired,
				CoyoteTimeDuration,
				false
			);
		}
	}

	if (OldMovementMode == MOVE_Falling && NewMovementMode != MOVE_Falling)
	{
		World->GetTimerManager().ClearTimer(CoyoteTimer);
	}
}

void URunGameLocomotionComponent::OnCoyoteTimeExpired()
{
	if (GetRuntimeCharacterState() == ERunGameCharacterState::CoyoteTime)
	{
		RequestCharacterState(ERunGameCharacterState::Airborne);
	}
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

		if (World)
		{
			World->GetTimerManager().ClearTimer(CoyoteTimer);
		}
	}
}
