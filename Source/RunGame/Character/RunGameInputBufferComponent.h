#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunGameInputBufferComponent.generated.h"

class UPlayerRuntimeState;

/** 输入命令类型 — 与 Character 状态分离，仅表达玩家意图 Input command — separated from character state, player intent only */
UENUM()
enum class ERunGameInputCommand : uint8
{
	None,
	Jump,
	Slide,
};

/** 输入命令请求：事件监听者处理后标记 bHandled / Input command request: listeners mark bHandled after consuming it */
USTRUCT()
struct FRunGameInputCommandRequest
{
	GENERATED_BODY()

	UPROPERTY()
	ERunGameInputCommand Command = ERunGameInputCommand::None;

	bool bHandled = false;

	FRunGameInputCommandRequest() = default;

	explicit FRunGameInputCommandRequest(ERunGameInputCommand InCommand)
		: Command(InCommand)
	{
	}

	void MarkHandled()
	{
		bHandled = true;
	}
};

/** 缓冲条目：意图 + 时间戳 + 超时 Buffered entry: intent + timestamp + timeout */
USTRUCT()
struct FBufferedCommand
{
	GENERATED_BODY()

	UPROPERTY()
	ERunGameInputCommand Command = ERunGameInputCommand::None;

	float Timestamp = 0.0f;

	bool IsExpired(float CurrentTime, float Timeout) const
	{
		return (CurrentTime - Timestamp) > Timeout;
	}
};

/**
 * 输入缓冲组件 —— 自治单元
 *
 * Character 仅广播输入意图，组件订阅后决定立即执行或缓存。
 * 组件自行监听 PlayerRuntimeState 状态变化，在状态回到 Idle 时消费缓冲命令，并通过 Character 的命令就绪事件广播。
 * 超时自动丢弃。
 *
 * Input buffer component — self-contained
 * Character only broadcasts input intent; this component decides immediate execution vs buffering.
 * Listens to PlayerRuntimeState, consumes buffered commands when state returns to Idle, then broadcasts ready commands through Character.
 * Stale commands auto-expire.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent)) 
class RUNGAME_API URunGameInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameInputBufferComponent();

	/** Buffered commands expire after this many seconds */
	UPROPERTY(EditAnywhere, Category = "Buffer")
	float BufferTimeout = 0.3f;

	/**
	 * 尝试立即执行输入命令。
	 * 若当前状态不允许 → 入队缓冲（超时后自动丢弃）。
	 * Attempt to execute immediately. If blocked by current state → enqueue (auto-expire).
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void BufferInput(ERunGameInputCommand Command);

	/** 清空所有缓冲 Dead 状态/重置时调用 Clear all buffered commands on death/reset */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void ClearBuffer();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** 响应 Character 输入意图事件 React to Character input-intent events */
	void HandleInputCommandRequested(FRunGameInputCommandRequest& Request);

	/** 当角色状态变化时——尝试消费缓冲 React to state changes — try consuming buffer */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	/** 当前状态是否应立即尝试执行命令 Whether the command should attempt immediate execution in current state */
	bool ShouldExecuteImmediately(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;

	/** 当前状态是否应缓存命令 Whether the command should be buffered in current state */
	bool ShouldBufferCommand(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;

	/** 通过 Character 的事件汇聚点广播命令就绪 Broadcast ready command through Character event hub */
	bool TryDispatchReadyCommand(ERunGameInputCommand Command) const;

	/** 移除过期条目 Remove stale entries */
	void ExpireStaleCommands();

	/** 尝试消费缓冲队列中的第一个有效条目 Try to consume the first valid buffered entry */
	void TryConsumeBuffer();

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;

	UPROPERTY()
	TObjectPtr<class ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TArray<FBufferedCommand> CommandQueue;
};
