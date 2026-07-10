#include "Character/Input/RunGameInputContextComponent.h"

#include "Engine/World.h"

URunGameInputContextComponent::URunGameInputContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameInputContextComponent::SetMoveAxis(const FVector2D& InMoveAxis)
{
	MoveAxis = InMoveAxis;
	bMoveInputDirty = true;
}

void URunGameInputContextComponent::SetLookAxis(const FVector2D& InLookAxis)
{
	LookAxis = InLookAxis;
	bLookInputDirty = true;
}

void URunGameInputContextComponent::BufferCommand(ERunGameInputCommand Command)
{
	LatestCommand.Command = Command;
	LatestCommand.Timestamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	bCommandBufferedThisFrame = true;
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
}

void URunGameInputContextComponent::RequestSkill(FGameplayTag SkillTag)
{
	RequestedSkillTag = SkillTag;
	bSkillRequestedThisFrame = true;
}

FRunGameInputFrame URunGameInputContextComponent::ConsumeFrame(float CurrentTime)
{
	ExpireLatestCommand(CurrentTime);

	FRunGameInputFrame Frame;

	Frame.MoveAxis = MoveAxis;
	Frame.bHasMoveInput = bMoveInputDirty;
	bMoveInputDirty = false;

	Frame.LookAxis = LookAxis;
	Frame.bHasLookInput = bLookInputDirty;
	bLookInputDirty = false;

	Frame.Command = LatestCommand.Command;
	Frame.bHasCommand = bCommandBufferedThisFrame && LatestCommand.Command != ERunGameInputCommand::None;
	bCommandBufferedThisFrame = false;

	Frame.bJumpReleased = bJumpReleasedThisFrame;
	bJumpReleasedThisFrame = false;

	Frame.SkillTag = RequestedSkillTag;
	Frame.bHasSkillRequest = bSkillRequestedThisFrame && RequestedSkillTag.IsValid();
	RequestedSkillTag = FGameplayTag();
	bSkillRequestedThisFrame = false;

	return Frame;
}

void URunGameInputContextComponent::ClearInputContext()
{
	MoveAxis = FVector2D::ZeroVector;
	LookAxis = FVector2D::ZeroVector;
	ClearLatestCommand();
	RequestedSkillTag = FGameplayTag();
	bMoveInputDirty = false;
	bLookInputDirty = false;
	bCommandBufferedThisFrame = false;
	bJumpReleasedThisFrame = false;
	bSkillRequestedThisFrame = false;
}
