#include "Character/Pipeline/RunGameControlPipelineComponent.h"

#include "Character/Input/RunGameInputComponent.h"
#include "Character/Locomotion/Movement/RunGameMovementComponent.h"
#include "Character/Locomotion/RunGameLocomotionComponent.h"
#include "Character/RunGameCharacter.h"
#include "Engine/World.h"
#include "RunGame.h"
#include "Skill/SkillComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"

URunGameControlPipelineComponent::URunGameControlPipelineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URunGameControlPipelineComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheOwnerComponents();

	if (MovementComponent)
	{
		MovementComponent->AddTickPrerequisiteComponent(this);
	}

	if (InputComponent)
	{
		LastJumpReleaseGeneration = InputComponent->GetInputSnapshot().JumpReleaseGeneration;
	}
}

void URunGameControlPipelineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (MovementComponent)
	{
		MovementComponent->RemoveTickPrerequisiteComponent(this);
	}

	OwnerCharacter = nullptr;
	InputComponent = nullptr;
	LocomotionComponent = nullptr;
	MovementComponent = nullptr;
	SkillComponent = nullptr;
	RuntimeState = nullptr;
	Super::EndPlay(EndPlayReason);
}

void URunGameControlPipelineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CacheOwnerComponents();

	if (!InputComponent || !LocomotionComponent || !MovementComponent || !RuntimeState)
	{
		return;
	}

	if (RuntimeState->GetCharacterState() == ERunGameCharacterState::Dead)
	{
		InputComponent->ClearInputState();
		LocomotionComponent->ClearSignals();
		return;
	}

	// 先提交上一物理帧的事实，再使用最新状态评估本帧输入。
	// Commit previous physics facts before evaluating this frame's input against the latest state.
	ProcessLocomotionSignals();

	const float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	InputComponent->ExpireRequests(CurrentTime);
	const FRunGameInputSnapshot Snapshot = InputComponent->GetInputSnapshot();

	ProcessContinuousInput(Snapshot);
	ProcessRequests(Snapshot);
}

void URunGameControlPipelineComponent::CacheOwnerComponents()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	}

	if (!OwnerCharacter)
	{
		return;
	}

	if (!InputComponent)
	{
		InputComponent = OwnerCharacter->FindComponentByClass<URunGameInputComponent>();
	}

	if (!LocomotionComponent)
	{
		LocomotionComponent = OwnerCharacter->GetRunGameLocomotionComponent();
	}

	if (!MovementComponent)
	{
		MovementComponent = OwnerCharacter->GetRunGameMovementComponent();
	}

	if (!SkillComponent)
	{
		SkillComponent = OwnerCharacter->GetSkillComponent();
	}

	if (!RuntimeState && GetWorld())
	{
		RuntimeState = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	}
}

void URunGameControlPipelineComponent::ProcessLocomotionSignals()
{
	ERunGameLocomotionSignal Signal;
	while (LocomotionComponent->DequeueSignal(Signal))
	{
		const ERunGameCharacterState CurrentState = RuntimeState->GetCharacterState();
		ERunGameCharacterState NewState = CurrentState;
		if (!LocomotionComponent->EvaluateSignal(Signal, CurrentState, NewState))
		{
			continue;
		}

		if (Signal == ERunGameLocomotionSignal::SlideEnded)
		{
			MovementComponent->ExecuteSlideEnd();
		}

		RuntimeState->TrySetCharacterState(NewState);
	}
}

void URunGameControlPipelineComponent::ProcessContinuousInput(const FRunGameInputSnapshot& Snapshot)
{
	MovementComponent->ExecuteMoveInput(Snapshot.MoveAxis.X);
	MovementComponent->ExecuteLookInput(Snapshot.LookAxis);

	if (Snapshot.JumpReleaseGeneration != LastJumpReleaseGeneration)
	{
		MovementComponent->ExecuteStopJumping();
		LastJumpReleaseGeneration = Snapshot.JumpReleaseGeneration;
	}
}

void URunGameControlPipelineComponent::ProcessRequests(const FRunGameInputSnapshot& Snapshot)
{
	bool bLocomotionDomainHandled = false;
	bool bSkillDomainHandled = false;

	for (const FRunGameInputRequest& Request : Snapshot.Requests)
	{
		if (Request.Type == ERunGameInputRequestType::Skill)
		{
			if (bSkillDomainHandled || !SkillComponent)
			{
				continue;
			}

			bSkillDomainHandled = true;
			const ERunGameInputRequestResult Result = SkillComponent->TryActivateRequestedSkill(Request.SkillTag);
			if (Result != ERunGameInputRequestResult::Deferred)
			{
				InputComponent->RemoveRequest(Request.RequestId);
			}
			continue;
		}

		if (bLocomotionDomainHandled)
		{
			continue;
		}

		bLocomotionDomainHandled = true;
		const ERunGameInputRequestResult Result = ProcessLocomotionRequest(Request);
		if (Result != ERunGameInputRequestResult::Deferred)
		{
			InputComponent->RemoveRequest(Request.RequestId);
		}
	}
}

ERunGameInputRequestResult URunGameControlPipelineComponent::ProcessLocomotionRequest(const FRunGameInputRequest& Request)
{
	const ERunGameCharacterState PreviousState = RuntimeState->GetCharacterState();

	if (Request.Type == ERunGameInputRequestType::Jump)
	{
		if (!LocomotionComponent->CanExecuteJump())
		{
			return ERunGameInputRequestResult::Deferred;
		}

		if (!MovementComponent->ExecuteJump())
		{
			return ERunGameInputRequestResult::Deferred;
		}

		LocomotionComponent->NotifyJumpRequested(PreviousState);
		if (!RuntimeState->TrySetCharacterState(ERunGameCharacterState::Airborne))
		{
			UE_LOG(LogRunGame, Error, TEXT("Pipeline failed to commit jump state for request %llu"), Request.RequestId);
			return ERunGameInputRequestResult::Rejected;
		}

		return ERunGameInputRequestResult::Applied;
	}

	if (Request.Type == ERunGameInputRequestType::Slide)
	{
		if (!LocomotionComponent->CanExecuteSlide())
		{
			return ERunGameInputRequestResult::Deferred;
		}

		if (!MovementComponent->ExecuteSlide())
		{
			return ERunGameInputRequestResult::Deferred;
		}

		if (!RuntimeState->TrySetCharacterState(ERunGameCharacterState::Sliding))
		{
			MovementComponent->ExecuteSlideEnd();
			UE_LOG(LogRunGame, Error, TEXT("Pipeline failed to commit slide state for request %llu"), Request.RequestId);
			return ERunGameInputRequestResult::Rejected;
		}

		return ERunGameInputRequestResult::Applied;
	}

	return ERunGameInputRequestResult::Rejected;
}
