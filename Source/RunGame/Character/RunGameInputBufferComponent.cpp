#include "Character/RunGameInputBufferComponent.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameCameraComponent.h"
#include "Character/Locomotion/RunGameLocomotionComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Skill/SkillComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/World.h"
#include "TimerManager.h"
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
	CacheOwnerComponents();
}

void URunGameInputBufferComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OwnerCharacter)
	{
		OwnerCharacter = nullptr;
	}
	if (SkillComponent)
	{
		SkillComponent = nullptr;
	}
	if (LocomotionComponent)
	{
		LocomotionComponent = nullptr;
	}
	if (CameraComponent)
	{
		CameraComponent = nullptr;
	}

	if (RuntimeState)
	{
		RuntimeState->OnCharacterStateChanged.RemoveDynamic(this, &URunGameInputBufferComponent::OnCharacterStateChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void URunGameInputBufferComponent::BindInput(
	UInputComponent* PlayerInputComponent,
	UInputAction* JumpAction,
	UInputAction* MoveAction,
	UInputAction* SlideAction,
	UInputAction* LookAction,
	UInputAction* MouseLookAction,
	USkillComponent* InSkillComponent)
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	}
	SkillComponent = InSkillComponent;
	CacheOwnerComponents();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogRunGame, Error, TEXT("InputBuffer: Failed to find Enhanced Input component"));
		return;
	}

	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&URunGameInputBufferComponent::HandleJumpStarted);
		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&URunGameInputBufferComponent::HandleJumpCompleted);
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&URunGameInputBufferComponent::HandleMove);
	}

	if (SlideAction)
	{
		EnhancedInputComponent->BindAction(
			SlideAction,
			ETriggerEvent::Started,
			this,
			&URunGameInputBufferComponent::HandleSlideStarted);
	}

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&URunGameInputBufferComponent::HandleLook);
	}

	if (MouseLookAction)
	{
		EnhancedInputComponent->BindAction(
			MouseLookAction,
			ETriggerEvent::Triggered,
			this,
			&URunGameInputBufferComponent::HandleLook);
	}

	if (SkillComponent && SkillComponent->SkillConfig)
	{
		for (const FSkillDefinition& SkillDef : SkillComponent->SkillConfig->Skills)
		{
			if (SkillDef.InputAction && SkillDef.SkillTag.IsValid())
			{
				EnhancedInputComponent->BindAction(
					SkillDef.InputAction,
					ETriggerEvent::Started,
					this,
					&URunGameInputBufferComponent::HandleSkillStarted,
					SkillDef.SkillTag);
			}
		}
	}
}

void URunGameInputBufferComponent::CacheOwnerComponents()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	}

	if (!OwnerCharacter)
	{
		return;
	}

	if (!LocomotionComponent)
	{
		LocomotionComponent = OwnerCharacter->FindComponentByClass<URunGameLocomotionComponent>();
	}

	if (!CameraComponent)
	{
		CameraComponent = OwnerCharacter->FindComponentByClass<URunGameCameraComponent>();
	}
}

// ---- Public API ----

void URunGameInputBufferComponent::BufferInput(ERunGameInputCommand Command)
{
	if (!RuntimeState) return;

	ExpireStaleCommands();

	const ERunGameCharacterState CurrentState = RuntimeState->GetCharacterState();

	// Dead — 丢弃所有输入 Dead — discard all input
	if (CurrentState == ERunGameCharacterState::Dead) return;

	if (ShouldExecuteImmediately(CurrentState, Command) && TryConsumeCommand(Command))
	{
		return;
	}

	if (!ShouldBufferCommand(CurrentState, Command)) return;

	// 最新输入意图覆盖旧缓冲 Last input intent replaces the previous buffered command
	BufferedCommand.Command = Command;
	BufferedCommand.Timestamp = GetWorld()->GetTimeSeconds();

	UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Buffered command %d (state=%d)"), (int32)Command, (int32)CurrentState);
}

void URunGameInputBufferComponent::ClearBuffer()
{
	BufferedCommand = FBufferedCommand();
	UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Buffer cleared"));
}

void URunGameInputBufferComponent::HandleMove(const FInputActionValue& Value)
{
	CacheOwnerComponents();

	if (!LocomotionComponent)
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2D>();
	LocomotionComponent->HandleMoveInput(MovementVector.X);
}

void URunGameInputBufferComponent::HandleLook(const FInputActionValue& Value)
{
	CacheOwnerComponents();

	if (!CameraComponent)
	{
		return;
	}

	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	CameraComponent->HandleLookInput(LookAxisVector.X, LookAxisVector.Y);
}

void URunGameInputBufferComponent::HandleJumpStarted()
{
	BufferInput(ERunGameInputCommand::Jump);
}

void URunGameInputBufferComponent::HandleJumpCompleted()
{
	CacheOwnerComponents();

	if (LocomotionComponent)
	{
		LocomotionComponent->HandleJumpInputReleased();
	}
}

void URunGameInputBufferComponent::HandleSlideStarted()
{
	BufferInput(ERunGameInputCommand::Slide);
}

void URunGameInputBufferComponent::HandleSkillStarted(FGameplayTag SkillTag)
{
	if (!RuntimeState)
	{
		RuntimeState = GetWorld() ? GetWorld()->GetSubsystem<UPlayerRuntimeState>() : nullptr;
	}

	if (!RuntimeState || !SkillComponent)
	{
		return;
	}

	const ERunGameCharacterState State = RuntimeState->GetCharacterState();
	if (State == ERunGameCharacterState::Dead || State == ERunGameCharacterState::Sliding)
	{
		return;
	}

	SkillComponent->TryActivateSkill(SkillTag);
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

	// 状态窗口变化时尝试消费缓冲 State-window changes try consuming buffered commands
	if (UWorld* World = GetWorld())
	{
		FTimerDelegate ConsumeDelegate;
		ConsumeDelegate.BindUObject(this, &URunGameInputBufferComponent::TryConsumeBuffer);

		// 状态代理同帧完成后再消费，避免先于 Movement 收尾导致 CanJump 误失败
		// Defer consumption until state delegates finish so Movement cleanup can run before CanJump checks
		World->GetTimerManager().SetTimerForNextTick(ConsumeDelegate);
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
		// 空中跳跃即时消费失败后转为落地跳缓冲；滑铲中跳跃等待滑铲结束 Airborne jump falls back to landing buffer; sliding jump waits for slide end
		return CurrentState == ERunGameCharacterState::Airborne
			|| CurrentState == ERunGameCharacterState::Sliding;

	default:
		return false;
	}
}

bool URunGameInputBufferComponent::CanAttemptBufferedConsume(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const
{
	switch (Command)
	{
	case ERunGameInputCommand::Jump:
		// 缓冲后的跳跃只等待下一次地面/土狼窗口，不反复尝试空中二段跳 Buffered jump waits for the next grounded/coyote window, not repeated air-jump attempts
		return CurrentState == ERunGameCharacterState::Idle
			|| CurrentState == ERunGameCharacterState::Turning
			|| CurrentState == ERunGameCharacterState::CoyoteTime;

	case ERunGameInputCommand::Slide:
		return CurrentState == ERunGameCharacterState::Idle;

	default:
		return false;
	}
}

bool URunGameInputBufferComponent::TryConsumeCommand(ERunGameInputCommand Command)
{
	CacheOwnerComponents();

	return LocomotionComponent && LocomotionComponent->TryConsumeInputCommand(Command);
}

void URunGameInputBufferComponent::ExpireStaleCommands()
{
	const float Now = GetWorld()->GetTimeSeconds();

	if (BufferedCommand.Command != ERunGameInputCommand::None && BufferedCommand.IsExpired(Now, BufferTimeout))
	{
		UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Expired command %d (age=%.2fs)"), (int32)BufferedCommand.Command, Now - BufferedCommand.Timestamp);
		BufferedCommand = FBufferedCommand();
	}
}

void URunGameInputBufferComponent::TryConsumeBuffer()
{
	ExpireStaleCommands();

	if (BufferedCommand.Command == ERunGameInputCommand::None) return;

	// 状态变化只尝试最新输入意图 State change only attempts the latest input intent
	const ERunGameCharacterState CurrentState = RuntimeState
		? RuntimeState->GetCharacterState()
		: ERunGameCharacterState::Idle;

	if (!CanAttemptBufferedConsume(CurrentState, BufferedCommand.Command))
	{
		return;
	}

	if (TryConsumeCommand(BufferedCommand.Command))
	{
		UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Consumed buffered command %d"), (int32)BufferedCommand.Command);
		BufferedCommand = FBufferedCommand();
	}
	else
	{
		UE_LOG(LogRunGame, Warning, TEXT("InputBuffer: Buffered command %d is still blocked in state %d"), (int32)BufferedCommand.Command, (int32)CurrentState);
	}
}
