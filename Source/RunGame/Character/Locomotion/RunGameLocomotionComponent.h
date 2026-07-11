#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RunGameType.h"
#include "RunGameLocomotionComponent.generated.h"

class ARunGameCharacter;
class UAnimMontage;
class UPlayerRuntimeState;
class URunGameAnimInstance;
class URunGameMovementComponent;
struct FHitResult;

/** 移动领域向控制管线上报的简单信号 / Simple locomotion signal reported to the control pipeline */
enum class ERunGameLocomotionSignal : uint8
{
	Landed,
	StartedFalling,
	CoyoteExpired,
	SlideEnded,
};

/**
 * 跑酷移动规则组件，负责跳跃、滑铲、土狼时间和二段跳判定。
 * Runner locomotion rules component owning jump, slide, coyote-time, and air-jump rules.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameLocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameLocomotionComponent();

	/** 获取当前角色语义状态 / Get current semantic character state */
	ERunGameCharacterState GetRuntimeCharacterState() const;

	/** 获取缓存的运动组件 / Get the cached movement component */
	FORCEINLINE URunGameMovementComponent* GetRunGameMovementComponent() const { return MovementComponent; }

	/** 判断当前领域状态是否允许执行跳跃请求 / Check whether a jump request is currently allowed */
	bool CanExecuteJump() const;

	/** 判断当前领域状态是否允许执行滑铲请求 / Check whether a slide request is currently allowed */
	bool CanExecuteSlide() const;

	/** 记录已经交给 CMC 的跳跃请求 / Record a jump request submitted to CMC */
	void NotifyJumpRequested(ERunGameCharacterState PreviousState);

	/** 在 CMC 确认起跳后提交二段跳领域数据 / Commit air-jump data after CMC confirms launch */
	void HandleOwnerJumped();

	/** 判断 Character 默认规则之外是否仍允许跳跃 / Extend Character jump rules with coyote and air-jump state */
	bool CanStartJump(bool bDefaultCanJump) const;

	/** 将落地回调转换为待处理信号 / Convert a landed callback into a pending signal */
	void HandleOwnerLanded(const FHitResult& Hit);

	/** 取出一个待处理移动信号 / Pop one pending locomotion signal */
	bool DequeueSignal(ERunGameLocomotionSignal& OutSignal);

	/** 清空未处理信号 / Clear pending locomotion signals */
	void ClearSignals();

	/** 根据最终状态评估信号对应的语义状态变化 / Evaluate a signal against the latest semantic state */
	bool EvaluateSignal(
		ERunGameLocomotionSignal Signal,
		ERunGameCharacterState CurrentState,
		ERunGameCharacterState& OutNewState) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** 土狼时间持续时间 / Duration of the coyote-time window */
	UPROPERTY(EditAnywhere, Category = "Movement|CoyoteTime")
	float CoyoteTimeDuration = 0.15f;

private:
	// 绑定滑铲动画结束事件 / Bind the slide animation end event
	void BindAnimationEvents();

	// 解绑滑铲动画结束事件 / Unbind the slide animation end event
	void UnbindAnimationEvents();

	// 将滑铲动画结束转换为信号 / Convert slide montage completion into a signal
	void HandleSlideMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 将 CMC 模式变化转换为移动信号 / Convert CMC movement-mode changes into locomotion signals
	void HandleMovementModeChanged(EMovementMode OldMovementMode, EMovementMode NewMovementMode);

	// 将土狼计时结束转换为信号 / Convert coyote timer expiry into a signal
	void OnCoyoteTimeExpired();

	/** 响应最终角色状态并维护领域数据 / React to committed character state and maintain domain data */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UPROPERTY()
	TObjectPtr<ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<URunGameMovementComponent> MovementComponent;

	UPROPERTY()
	TObjectPtr<URunGameAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;

	FDelegateHandle MovementModeChangedHandle;
	FDelegateHandle SlideMontageEndedHandle;
	FTimerHandle CoyoteTimer;
	TArray<ERunGameLocomotionSignal> PendingSignals;
	bool bAirJumpAvailable = true;
	bool bJumpLaunchPending = false;
	ERunGameCharacterState PendingJumpStartState = ERunGameCharacterState::Idle;
};
