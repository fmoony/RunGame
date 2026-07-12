#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameMovementComponent.generated.h"

class UCurveFloat;
class URunGameTimerSubsystem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FRunGameMovementModeChanged, EMovementMode, EMovementMode);
DECLARE_MULTICAST_DELEGATE_TwoParams(FRunGameRunDirectionChanged, float, float);

/**
 * 角色运动物理组件，负责 CMC 层速度、摩擦、转向和 MovementMode 事件
 * Character movement physics component owning CMC-level speed, friction, turning, and MovementMode events.
 */
UCLASS()
class RUNGAME_API URunGameMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	URunGameMovementComponent();

	/** MovementMode 变化事件，供 Locomotion 规则层响应 MovementMode change event for locomotion rules */
	FRunGameMovementModeChanged OnRunGameMovementModeChanged;

	/** 跑酷前进方向变化事件 / Run direction changed event */
	FRunGameRunDirectionChanged OnRunDirectionChanged;

	/** 运行时间到最大行走速度的曲线 Curve that maps elapsed game time to max walk speed */
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Speed")
	TObjectPtr<UCurveFloat> MaxSpeedCurve;

	/** 基础最大行走速度 Base max walk speed */
	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
	float BaseMaxWalkSpeed = 2000.0f;

	/** 平滑到目标速度的时长 Duration to reach target speed via interpolation */
	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
	float SpeedTransitionDuration = 0.5f;

	/** 添加或更新速度修正器 Add or update a speed modifier */
	void AddSpeedModifier(FGameplayTag ModifierTag, float Multiplier);

	/** 移除速度修正器 Remove a speed modifier by tag */
	void RemoveSpeedModifier(FGameplayTag ModifierTag);

	FORCEINLINE float GetCompositeSpeedMultiplier() const { return CachedCompositeSpeedMultiplier; }
	FORCEINLINE bool IsInTurn() const { return bTurn; }
	FORCEINLINE bool IsInTurnBox() const { return bInTurnBox; }
	FORCEINLINE FRotator GetDesireRotation() const { return DesireRotation; }

	/** 执行横向移动输入 / Execute lateral movement input */
	void ExecuteMoveInput(float Right);

	/** 向 Character 提交跳跃命令 / Submit a jump command to the Character */
	bool ExecuteJump();

	/** 向 Character 提交停止跳跃命令 / Submit a stop-jumping command to the Character */
	void ExecuteStopJumping();

	/** 启动滑铲物理设置 / Apply slide movement settings */
	bool ExecuteSlide();

	/** 结束滑铲物理设置 / Restore movement settings after a slide */
	void ExecuteSlideEnd();

	/** 应用 90 度转向旋转，返回是否应阻止横向输入 Apply 90-degree turn rotation and return whether lateral input should be blocked */
	bool ApplyTurnRotation(float Right);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 广播 MovementMode 变化 Broadcast MovementMode changes */
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;

private:
	/** 响应角色状态变化 React to character state changes */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	float SmoothedMaxWalkSpeed = 2000.0f;
	float DefaultGroundFriction = 8.0f;
	TMap<FGameplayTag, float> SpeedModifiers;
	float CachedCompositeSpeedMultiplier = 1.0f;

	bool bTurn = false;
	bool bInTurnBox = false;
	FRotator DesireRotation = FRotator::ZeroRotator;

	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;
};
