#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

/**
 * Native AnimInstance — 缓存层：C++ 预计算所有动画数据，ABP 纯读不计算。
 * 通过 RuntimeState 事件接收受击触发，不直接绑 HealthComponent。
 *
 * Caching layer: all animation data pre-computed in C++, ABP reads only.
 * Hit reaction triggers received via RuntimeState events — no direct HealthComponent binding.
 */
UCLASS()
class RUNGAME_API URunGameAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// ── Hit reaction trigger ──

	/** ABP 读取 → 播放受击动画 → AnimNotify 末尾重置 Read by ABP → play hit reaction → reset by AnimNotify */
	UPROPERTY(BlueprintReadOnly, Category = "Animation|Hit")
	bool bPlayHitReaction = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Hit")
	FGameplayTag HitReactionDamageType;

	/** AnimNotify 调用入口 — 受击动画末尾重置标记 Called by AnimNotify at end of hit reaction */
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void NotifyHitReactionFinished();

protected:
	// ── Cached references ──

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ACharacter> Owner;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComp;

	// ── Locomotion ──

	/** Total velocity magnitude */
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float Speed = 0.0f;

	/** Horizontal velocity magnitude (ignoring Z) */
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float GroundSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	bool bIsMoving = false;

	// ── Character State ──

	UPROPERTY(BlueprintReadOnly, Category = "State")
	ERunGameCharacterState CharacterState = ERunGameCharacterState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsSliding = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsTurning = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead = false;

	// ── Direction ──

	/** Velocity direction angle relative to actor forward (-180..180). 0 = moving straight ahead */
	UPROPERTY(BlueprintReadOnly, Category = "Direction")
	float MoveDirectionAngle = 0.0f;

private:
	/** 绑定 RuntimeState 委托（受击）— 不直接绑 HealthComponent */
	void BindGameplayDelegates();

	UFUNCTION()
	void OnHitReaction(float Damage, FGameplayTag DamageType);
};
