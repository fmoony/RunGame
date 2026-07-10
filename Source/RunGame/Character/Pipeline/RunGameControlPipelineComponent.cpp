#include "Character/Pipeline/RunGameControlPipelineComponent.h"

#include "Character/Camera/RunGameCameraComponent.h"
#include "Character/Input/RunGameInputContextComponent.h"
#include "Character/Locomotion/RunGameLocomotionComponent.h"
#include "Character/RunGameCharacter.h"
#include "GameplayTagContainer.h"
#include "Skill/SkillComponent.h"
#include "TimerManager.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"

URunGameControlPipelineComponent::URunGameControlPipelineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameControlPipelineComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheOwnerComponents();

	if (RuntimeState)
	{
		RuntimeState->OnCharacterStateChanged.AddDynamic(this, &URunGameControlPipelineComponent::OnCharacterStateChanged);
	}
}

void URunGameControlPipelineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (RuntimeState)
	{
		RuntimeState->OnCharacterStateChanged.RemoveDynamic(this, &URunGameControlPipelineComponent::OnCharacterStateChanged);
	}

	OwnerCharacter = nullptr;
	InputContext = nullptr;
	LocomotionComponent = nullptr;
	CameraComponent = nullptr;
	SkillComponent = nullptr;
	RuntimeState = nullptr;

	Super::EndPlay(EndPlayReason);
}

void URunGameControlPipelineComponent::ProcessInputFrame()
{
	CacheOwnerComponents();

	if (!InputContext)
	{
		return;
	}

	const float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	const FRunGameInputFrame Frame = InputContext->ConsumeFrame(CurrentTime);

	const ERunGameCharacterState CurrentState = RuntimeState
		? RuntimeState->GetCharacterState()
		: ERunGameCharacterState::Idle;

	if (CurrentState == ERunGameCharacterState::Dead)
	{
		InputContext->ClearInputContext();
		return;
	}

	if (Frame.bHasLookInput && CameraComponent)
	{
		CameraComponent->HandleLookInput(Frame.LookAxis.X, Frame.LookAxis.Y);
	}

	if (Frame.bHasMoveInput && LocomotionComponent)
	{
		LocomotionComponent->HandleMoveInput(Frame.MoveAxis.X);
	}

	if (Frame.bJumpReleased && LocomotionComponent)
	{
		LocomotionComponent->HandleJumpInputReleased();
	}

	if (Frame.bHasCommand && LocomotionComponent)
	{
		LocomotionComponent->HandleInputContextCommand(InputContext, Frame.Command);
	}

	if (Frame.bHasSkillRequest && SkillComponent)
	{
		SkillComponent->TryActivateRequestedSkill(Frame.SkillTag);
	}
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

	if (!InputContext)
	{
		InputContext = OwnerCharacter->FindComponentByClass<URunGameInputContextComponent>();
	}

	if (!LocomotionComponent)
	{
		LocomotionComponent = OwnerCharacter->FindComponentByClass<URunGameLocomotionComponent>();
	}

	if (!CameraComponent)
	{
		CameraComponent = OwnerCharacter->FindComponentByClass<URunGameCameraComponent>();
	}

	if (!SkillComponent)
	{
		SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	}

	if (!RuntimeState && GetWorld())
	{
		RuntimeState = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	}
}

void URunGameControlPipelineComponent::OnCharacterStateChanged(ERunGameCharacterState /*OldState*/, ERunGameCharacterState NewState)
{
	CacheOwnerComponents();

	if (!InputContext)
	{
		return;
	}

	if (NewState == ERunGameCharacterState::Dead)
	{
		InputContext->ClearInputContext();
		return;
	}

	if (UWorld* World = GetWorld())
	{
		FTimerDelegate ConsumeDelegate;
		ConsumeDelegate.BindUObject(this, &URunGameControlPipelineComponent::ProcessBufferedCommand);
		World->GetTimerManager().SetTimerForNextTick(ConsumeDelegate);
	}
}

void URunGameControlPipelineComponent::ProcessBufferedCommand()
{
	CacheOwnerComponents();

	if (LocomotionComponent && InputContext)
	{
		LocomotionComponent->TryConsumeInputContextBuffer(InputContext);
	}
}
