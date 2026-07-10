#include "Character/Input/RunGameInputContextComponent.h"

#include "Engine/World.h"

URunGameInputContextComponent::URunGameInputContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameInputContextComponent::SetMoveAxis(const FVector2D& InMoveAxis)
{
	MoveAxis = InMoveAxis;
	OnMoveInputChanged.Broadcast(MoveAxis);
}

void URunGameInputContextComponent::SetLookAxis(const FVector2D& InLookAxis)
{
	LookAxis = InLookAxis;
	OnLookInputChanged.Broadcast(LookAxis);
}

void URunGameInputContextComponent::BufferCommand(ERunGameInputCommand Command)
{
	LatestCommand.Command = Command;
	LatestCommand.Timestamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	OnCommandBuffered.Broadcast(Command);
}

bool URunGameInputContextComponent::ConsumeLatestCommand(ERunGameInputCommand ExpectedCommand)
{
	if (LatestCommand.Command != ExpectedCommand)
	{
		return false;
	}

	ClearLatestCommand();
	return true;
}

void URunGameInputContextComponent::ClearLatestCommand()
{
	LatestCommand = FRunGameBufferedInputCommand();
}

void URunGameInputContextComponent::ExpireLatestCommand(float CurrentTime)
{
	if (LatestCommand.Command != ERunGameInputCommand::None
		&& LatestCommand.IsExpired(CurrentTime, CommandBufferTimeout))
	{
		ClearLatestCommand();
	}
}

void URunGameInputContextComponent::NotifyJumpReleased()
{
	bJumpReleasedThisFrame = true;
	OnJumpReleased.Broadcast();
}

void URunGameInputContextComponent::RequestSkill(FGameplayTag SkillTag)
{
	RequestedSkillTag = SkillTag;
	OnSkillRequested.Broadcast(SkillTag);
}

void URunGameInputContextComponent::ClearInputContext()
{
	MoveAxis = FVector2D::ZeroVector;
	LookAxis = FVector2D::ZeroVector;
	ClearLatestCommand();
	RequestedSkillTag = FGameplayTag();
	bJumpReleasedThisFrame = false;
}
