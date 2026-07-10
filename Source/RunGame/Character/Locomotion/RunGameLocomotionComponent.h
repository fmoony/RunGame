#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Input/RunGameInputTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunGameType.h"
#include "RunGameLocomotionComponent.generated.h"

class ARunGameCharacter;
class UAnimMontage;
class UPlayerRuntimeState;
class URunGameAnimInstance;
class URunGameInputContextComponent;
class URunGameMovementComponent;
struct FHitResult;

/**
 * 跑酷运动规则组件，承接跳跃/滑铲/转向等玩法层运动规则
 * Runner locomotion rules component, owning gameplay-level jump/slide/turn rules.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameLocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameLocomotionComponent();

	/** 获取当前角色语义状态 Get current semantic character state */
	ERunGameCharacterState GetRuntimeCharacterState() const;

	/** 获取缓存的运动组件 Get cached movement component */
	FORCEINLINE URunGameMovementComponent* GetRunGameMovementComponent() const { return MovementComponent; }

	/** 消费输入命令 Consume an input command */
	bool TryConsumeInputCommand(ERunGameInputCommand Command);

	/** 处理跳跃按键释放 Handle jump input release */
	void HandleJumpInputReleased() const;

	/** 处理横向移动输入，包含转向盒规则 Handle lateral move input, including turn-box rules */
	void HandleMoveInput(float Right) const;

	/** 判断当前状态是否允许跳跃 Check whether the current state allows jump */
	bool CanStartJump(bool bDefaultCanJump) const;

	/** 响应 Character 起跳回调 React to Character jump callback */
	void HandleOwnerJumped();

	/** 响应 Character 落地回调 React to Character landed callback */
	void HandleOwnerLanded(const FHitResult& Hit);

	/** 应用转向规则，返回是否阻止横向输入 Apply turn rule and return whether lateral input should be blocked */
	bool ApplyTurnRotation(float Right) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** CoyoteTime 持续时间（秒），走下边缘后仍可跳跃的缓冲窗口 Duration of coyote time window after walking off ledge */
	UPROPERTY(EditAnywhere, Category = "Movement|CoyoteTime")
	float CoyoteTimeDuration = 0.15f;

private:
	/** 尝试启动跳跃，成功时记录跳跃上下文 Try to start jump and record jump context on success */
	bool TryStartJump();

	/** 请求 RuntimeState 切换角色状态 Request RuntimeState character state transition */
	bool RequestCharacterState(ERunGameCharacterState NewState) const;

	/** 消费主动跳跃标记，区分主动起跳和走出边缘 Consume intentional jump marker to distinguish jump from ledge fall */
	bool ConsumePendingJumpLaunch();

	/** 绑定动画事件，由 Locomotion 接管滑铲结束决策 Bind animation events so Locomotion owns slide-end decisions */
	void BindAnimationEvents();

	/** 解绑动画事件 Unbind animation events */
	void UnbindAnimationEvents();

	/** 响应滑铲蒙太奇结束 React to slide montage end */
	void HandleSlideMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** 处理 MovementMode 变化 Handle movement mode changes */
	void HandleMovementModeChanged(EMovementMode OldMovementMode, EMovementMode NewMovementMode);

	/** CoyoteTime 到期回调，状态仍为 CoyoteTime 则转入 Airborne Coyote time expiry callback */
	void OnCoyoteTimeExpired();

	/** 响应角色状态变化 React to character state changes */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	void BindInputContext();
	void UnbindInputContext();
	void HandleMoveInputChanged(const FVector2D& MoveAxis);
	void HandleJumpReleasedFromInputContext();
	void HandleInputCommandBuffered(ERunGameInputCommand Command);
	void TryConsumeInputContextBuffer();
	bool ShouldExecuteImmediately(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;
	bool ShouldBufferCommand(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;
	bool CanAttemptBufferedConsume(ERunGameCharacterState CurrentState, ERunGameInputCommand Command) const;

	UPROPERTY()
	TObjectPtr<ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<URunGameMovementComponent> MovementComponent;

	UPROPERTY()
	TObjectPtr<URunGameAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;

	UPROPERTY()
	TObjectPtr<URunGameInputContextComponent> InputContext;

	FDelegateHandle MovementModeChangedHandle;
	FDelegateHandle SlideMontageEndedHandle;
	FDelegateHandle MoveInputChangedHandle;
	FDelegateHandle CommandBufferedHandle;
	FDelegateHandle JumpReleasedHandle;
	FTimerHandle CoyoteTimer;

	bool bAirJumpAvailable = true;
	bool bJumpLaunchPending = false;
	ERunGameCharacterState PendingJumpStartState = ERunGameCharacterState::Idle;
};
