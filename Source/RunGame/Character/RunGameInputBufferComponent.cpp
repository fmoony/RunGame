#include "Character/RunGameInputBufferComponent.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameMovementComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/World.h"
#include "RunGame.h"

URunGameInputBufferComponent::URunGameInputBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameInputBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	RuntimeState = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (RuntimeState)
	{
		RuntimeState->OnCharacterStateChanged.AddDynamic(this, &URunGameInputBufferComponent::OnCharacterStateChanged);
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("InputBuffer: Failed to get PlayerRuntimeState"));
	}

	OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
}

void URunGameInputBufferComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OwnerCharacter)
	{
		OwnerCharacter = nullptr;
	}

	if (RuntimeState)
	{
		RuntimeState->OnCharacterStateChanged.RemoveDynamic(this, &URunGameInputBufferComponent::OnCharacterStateChanged);
	}

	Super::EndPlay(EndPlayReason);
}

// ---- Public API ----

void URunGameInputBufferComponent::BufferInput(ERunGameInputCommand Command)
{
	if (!RuntimeState) return;

	const ERunGameCharacterState CurrentState = RuntimeState->GetCharacterState();

	// Dead — 丢弃所有输入 Dead — discard all input
	if (CurrentState == ERunGameCharacterState::Dead) return;

	if (ShouldExecuteImmediately(CurrentState, Command) && TryConsumeCommand(Command))
	{
		return;
	}

	if (!ShouldBufferCommand(CurrentState, Command)) return;

	// 去重：同类型命令只保留最新一个 Deduplicate: only keep latest of same command type
	for (int32 i = CommandQueue.Num() - 1; i >= 0; --i)
	{
		if (CommandQueue[i].Command == Command)
		{
			CommandQueue.RemoveAt(i);
		}
	}

	FBufferedCommand& Entry = CommandQueue.AddDefaulted_GetRef();
	Entry.Command = Command;
	Entry.Timestamp = GetWorld()->GetTimeSeconds();

	UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Buffered command %d (state=%d)"), (int32)Command, (int32)CurrentState);
}

void URunGameInputBufferComponent::ClearBuffer()
{
	CommandQueue.Empty();
	UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Buffer cleared"));
}

// ---- State reaction ----

void URunGameInputBufferComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	// Dead → 清空缓冲 Dead → clear everything
	if (NewState == ERunGameCharacterState::Dead)
	{
		ClearBuffer();
		return;
	}

	// 回到 Idle → 尝试消费缓冲 Back to Idle → try consuming buffered commands
	if (NewState == ERunGameCharacterState::Idle)
	{
		TryConsumeBuffer();
	}
}

// ---- Internal ----

bool URunGameInputBufferComponent::ShouldExecuteImmediately(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const
{
	switch (Command)
	{
	case ERunGameInputCommand::Jump:
		// Jump 先尝试即时执行；MovementComponent 决定一段跳、土狼跳或二段跳是否真正可用
		// Jump first attempts immediate execution; MovementComponent decides ground, coyote, or double-jump validity
		return CurrentState == ERunGameCharacterState::Idle
			|| CurrentState == ERunGameCharacterState::Turning
			|| CurrentState == ERunGameCharacterState::CoyoteTime
			|| CurrentState == ERunGameCharacterState::Airborne;

	case ERunGameInputCommand::Slide:
		return CurrentState == ERunGameCharacterState::Idle;

	default:
		return false;
	}
}

bool URunGameInputBufferComponent::ShouldBufferCommand(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const
{
	switch (Command)
	{
	case ERunGameInputCommand::Slide:
		// 空中按滑铲 → 缓冲，落地触发 Airborne/CoyoteTime + Slide → buffer, consume on land
		return CurrentState == ERunGameCharacterState::Airborne
			|| CurrentState == ERunGameCharacterState::CoyoteTime;

	case ERunGameInputCommand::Jump:
		// 滑铲中按跳 → 缓冲，滑铲结束后触发 Sliding + Jump → buffer, consume after slide
		return CurrentState == ERunGameCharacterState::Sliding;

	default:
		return false;
	}
}

bool URunGameInputBufferComponent::TryConsumeCommand(ERunGameInputCommand Command) const
{
	if (!OwnerCharacter) return false;

	URunGameMovementComponent* MoveComp = OwnerCharacter->GetRunGameMovementComponent();
	return MoveComp && MoveComp->TryConsumeInputCommand(Command);
}

void URunGameInputBufferComponent::ExpireStaleCommands()
{
	const float Now = GetWorld()->GetTimeSeconds();

	CommandQueue.RemoveAll([this, Now](const FBufferedCommand& Cmd)
	{
		if (Cmd.IsExpired(Now, BufferTimeout))
		{
			UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Expired command %d (age=%.2fs)"), (int32)Cmd.Command, Now - Cmd.Timestamp);
			return true;
		}
		return false;
	});
}

void URunGameInputBufferComponent::TryConsumeBuffer()
{
	ExpireStaleCommands();

	if (CommandQueue.Num() == 0) return;

	// FIFO: 消费最早缓冲的命令 FIFO: consume oldest buffered command
	const FBufferedCommand Cmd = CommandQueue[0];
	if (TryConsumeCommand(Cmd.Command))
	{
		CommandQueue.RemoveAt(0);
		UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Consumed buffered command %d"), (int32)Cmd.Command);
	}
	else
	{
		UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Buffered command %d is still blocked"), (int32)Cmd.Command);
	}
}
