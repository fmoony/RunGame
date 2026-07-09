#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameAnimInstance.generated.h"

class ACharacter;
class ARunGameCharacter;
class UCharacterMovementComponent;
class UAnimMontage;
class UPlayerRuntimeState;
class URunGameTimerSubsystem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FRunGameSlideMontageEnded, UAnimMontage*, bool);

/**
 * Native AnimInstance — 统一管理所有动画：数据缓存 + 蒙太奇播放。
 * AnimInstance — unified animation: data cache + montage playback.
 * 删除 AnimationComponent，全部逻辑集中于此。
 */
UCLASS()
class RUNGAME_API URunGameAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/** 滑铲蒙太奇结束事件，Locomotion 决定是否结束滑铲 Slide montage end event; Locomotion decides whether to finish slide */
	FRunGameSlideMontageEnded OnSlideMontageEnded;

	// ── Config: Slide ──

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Slide")
	TObjectPtr<UAnimMontage> SlideMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Slide")
	float RootMotionScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Slide")
	float MontagePlayRate = 0.8f;

	// ── Config: Death ──

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Death")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> DeathMontages;

	// ── Config: Hit ──

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Hit")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> HitReactionMontages;

	// ── Locomotion (ABP 纯读) ──

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float GroundSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "Direction")
	float MoveDirectionAngle = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	ERunGameCharacterState CharacterState = ERunGameCharacterState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsSliding = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsCoyoteTime = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsTurning = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead = false;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ACharacter> Owner;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComp;

private:
	// ── Init ──

	void BindGameplayDelegates();
	void CacheBaseSpeed();

	// ── State reactions ──

	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	// ── Slide ──

	void PlaySlideMontage();
	void EndSlide();

	UFUNCTION()
	void OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	// ── Death ──

	void PlayDeathMontage(FGameplayTag DamageType);

	UFUNCTION()
	void OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	// ── Hit ──

	UFUNCTION()
	void OnHitReaction(float Damage, FGameplayTag DamageType);

	UFUNCTION()
	void OnCharacterDied(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter);

	UPROPERTY(Transient)
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

	/** 缓存的 PRS —— 避免每帧 GetSubsystem 查找 */
	TWeakObjectPtr<UPlayerRuntimeState> CachedPRS;

	float BaseMaxWalkSpeed = 1200.0f;
};
