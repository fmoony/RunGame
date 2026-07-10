#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameInputBufferComponent.generated.h"

class UPlayerRuntimeState;
class UInputAction;
class UInputComponent;
class USkillComponent;
class URunGameCameraComponent;
class URunGameLocomotionComponent;
struct FInputActionValue;

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

/**
 * 输入缓冲组件 —— 自治单元
 *
 * Character 仅转发 EnhancedInput，组件决定立即执行或缓存。
 * 组件自行监听 PlayerRuntimeState 状态变化，在状态回到 Idle 时尝试消费缓冲命令。
 * 超时自动丢弃。
 *
 * Input buffer component — self-contained
 * Character only forwards EnhancedInput; this component decides immediate execution vs buffering.
 * Listens to PlayerRuntimeState and tries to consume buffered commands when state returns to Idle.
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

	/** 绑定角色输入动作，把 EnhancedInput 入口收束到输入组件 Bind character input actions and keep EnhancedInput entrypoints inside this component */
	void BindInput(
		UInputComponent* PlayerInputComponent,
		UInputAction* JumpAction,
		UInputAction* MoveAction,
		UInputAction* SlideAction,
		UInputAction* LookAction,
		UInputAction* MouseLookAction,
		USkillComponent* InSkillComponent);

	/**
	 * 尝试立即执行输入命令。
	 * 若当前状态不允许 → 入队缓冲（超时后自动丢弃）。
	 * Attempt to execute immediately. If blocked by current state -> cache as latest intent (auto-expire).
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
	// 缓存同角色上的输入目标组件 Cache input target components on the owner character
	void CacheOwnerComponents();

	// 处理移动输入并转发给 Locomotion 规则域 Handle movement input and forward to the Locomotion rules domain
	void HandleMove(const FInputActionValue& Value);

	// 处理视角输入并转发给 Camera 域 Handle look input and forward to the Camera domain
	void HandleLook(const FInputActionValue& Value);

	// 处理跳跃按下并进入缓冲/消费链路 Handle jump press through the buffer/consume path
	void HandleJumpStarted();

	// 处理跳跃释放并转发给运动规则组件 Handle jump release and forward to locomotion rules
	void HandleJumpCompleted();

	// 处理滑铲按下并进入缓冲/消费链路 Handle slide press through the buffer/consume path
	void HandleSlideStarted();

	// 处理技能输入并按当前角色状态过滤 Handle skill input and filter by current character state
	void HandleSkillStarted(FGameplayTag SkillTag);

	/** 当角色状态变化时——尝试消费缓冲 React to state changes — try consuming buffer */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	/** 当前状态是否应立即尝试执行命令 Whether the command should attempt immediate execution in current state */
	bool ShouldExecuteImmediately(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;

	/** 当前状态是否应缓存命令 Whether the command should be buffered in current state */
	bool ShouldBufferCommand(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;

	/** 缓冲命令在当前状态是否应尝试消费 Whether a buffered command should attempt consumption in the current state */
	bool CanAttemptBufferedConsume(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;

	/** 请求 Locomotion 消费输入命令 Request Locomotion to consume an input command */
	bool TryConsumeCommand(ERunGameInputCommand Command);

	/** 移除过期条目 Remove stale entries */
	void ExpireStaleCommands();

	/** 尝试消费最新缓冲命令 Try to consume the latest buffered command */
	void TryConsumeBuffer();

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;

	UPROPERTY()
	TObjectPtr<class ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<USkillComponent> SkillComponent;

	UPROPERTY()
	TObjectPtr<URunGameLocomotionComponent> LocomotionComponent;

	UPROPERTY()
	TObjectPtr<URunGameCameraComponent> CameraComponent;

	UPROPERTY()
	FBufferedCommand BufferedCommand;
};
