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

DECLARE_DELEGATE_OneParam(FOnInputCommandConsumed, ERunGameInputCommand);

/**
 * 输入缓冲组件 —— 自治单元
 *
 * Character 仅绑 EnhancedInput → 调用 BufferInput。
 * 组件自行监听 PlayerRuntimeState 状态变化，在状态回到 Idle 时消费缓冲命令。
 * 超时自动丢弃。
 *
 * Input buffer component — self-contained
 * Character only binds EnhancedInput → BufferInput.
 * Listens to PlayerRuntimeState, consumes buffered commands when state returns to Idle.
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

	/** Fired when a buffered command is consumed. Character binds → DoJump / StartSlide */
	FOnInputCommandConsumed OnInputCommandConsumed;

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
	/** 当角色状态变化时——尝试消费缓冲 React to state changes — try consuming buffer */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	/** 移除过期条目 Remove stale entries */
	void ExpireStaleCommands();

	/** 尝试消费缓冲队列中的第一个有效条目 Try to consume the first valid buffered entry */
	void TryConsumeBuffer();

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;

	UPROPERTY()
	TArray<FBufferedCommand> CommandQueue;
};
