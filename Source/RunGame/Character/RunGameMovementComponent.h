#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameMovementComponent.generated.h"

class UCurveFloat;
class URunGameTimerSubsystem;

/**
 * 角色运动组件 —— 自治单元，绑 PRS 状态变化自行管理运动物理。
 * Character 通过 FObjectInitializer 强制替换默认 CMC。
 * Custom movement component — self-contained, reacts to state changes autonomously.
 * Character replaces default CMC via FObjectInitializer::SetDefaultSubobjectClass.
 */
UCLASS()
class RUNGAME_API URunGameMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	URunGameMovementComponent();

	/** Float curve that maps elapsed game time to max walk speed */
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Speed")
	TObjectPtr<UCurveFloat> MaxSpeedCurve;

	/** 基础最大行走速度 Base max walk speed — editable in Blueprint */
	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
	float BaseMaxWalkSpeed = 2000.0f;

	/** Duration in seconds to reach target speed via smooth interpolation */
	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
	float SpeedTransitionDuration = 0.5f;

	// -- Speed modifiers --

	/** 添加/更新速度修改器。同标签会覆盖 Add or update a speed modifier. Overwrites if tag exists */
	void AddSpeedModifier(FGameplayTag ModifierTag, float Multiplier);

	/** 移除速度修改器 Remove a speed modifier by tag */
	void RemoveSpeedModifier(FGameplayTag ModifierTag);

	FORCEINLINE float GetCompositeSpeedMultiplier() const { return CachedCompositeSpeedMultiplier; }

	// -- Turn system --

	FORCEINLINE bool IsInTurn() const { return bTurn; }
	FORCEINLINE bool IsInTurnBox() const { return bInTurnBox; }

	/** 应用 90 度转弯旋转，返回 true 表示应忽略横向输入 Apply 90-degree turn rotation, returns true if lateral input should be consumed */
	bool ApplyTurnRotation(float Right);

	FORCEINLINE FRotator GetDesireRotation() const { return DesireRotation; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 事件驱动空中检测：移动模式变为 Falling → 设 CoyoteTime（土狼时间缓冲），定时器到期转 Airborne */
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;

	/** CoyoteTime 持续时间（秒）——走下边缘后仍可跳跃的缓冲窗口 Duration of coyote time window after walking off ledge */
	UPROPERTY(EditAnywhere, Category = "Movement|CoyoteTime")
	float CoyoteTimeDuration = 0.15f;

private:
	/** 响应角色状态变化 React to character state changes */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	// -- Speed --

	float SmoothedMaxWalkSpeed = 2000.0f;
	float DefaultGroundFriction = 8.0f;
	TMap<FGameplayTag, float> SpeedModifiers;
	float CachedCompositeSpeedMultiplier = 1.0f;

	// -- Turn --

	bool bTurn = false;
	bool bInTurnBox = false;
	FRotator DesireRotation = FRotator::ZeroRotator;

	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

	/** CoyoteTime 到期回调——状态仍为 CoyoteTime 则转入 Airborne Coyote time expiry — if still in CoyoteTime, transition to Airborne */
	void OnCoyoteTimeExpired();

	FTimerHandle CoyoteTimer;
};
