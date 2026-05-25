#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameAnimationComponent.generated.h"

class UAnimMontage;
class URunGameTimerSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathMontageCompleteSignature);

/**
 * 动画表现组件 —— 自治单元，绑 PRS 状态变化 + HealthComponent 伤害/死亡广播，自行管理蒙太奇。
 * Character 仅绑 OnDeathMontageComplete 以触发溶解 → Destroy。
 * Animation component — self-contained, reacts to state changes and damage/death broadcasts autonomously.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameAnimationComponent();

	/** 滑铲蒙太奇 Slide montage */
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Slide")
	TObjectPtr<UAnimMontage> SlideMontage;

	/** 滑铲根运动缩放 Root motion scale during slide */
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Slide")
	float RootMotionScale = 1.0f;

	/** 蒙太奇播放速率基准 Base play rate for montages */
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Slide")
	float MontagePlayRate = 0.8f;

	/** 死亡蒙太奇映射 Map damage type to death montage */
	UPROPERTY(EditAnywhere, Category = "Animation|Death")
	TMap<FGameplayTag, UAnimMontage*> DeathMontages;

	/** 受击蒙太奇映射 Map damage type to hit reaction montage */
	UPROPERTY(EditAnywhere, Category = "Animation|Hit")
	TMap<FGameplayTag, UAnimMontage*> HitReactionMontages;

	/** 死亡蒙太奇 blend-out 完成时广播。Character 绑它 → StartDissolve Broadcast when death montage completes. Character binds → start dissolve */
	UPROPERTY(BlueprintAssignable, Category = "Animation|Delegates")
	FOnDeathMontageCompleteSignature OnDeathMontageComplete;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** 响应角色状态变化 React to character state changes */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	/** 响应伤害——播放受击蒙太奇 React to damage — play hit reaction */
	UFUNCTION()
	void OnDamageTaken(float Damage, FGameplayTag DamageType, AActor* DamageCauser);

	/** 响应死亡——播放死亡蒙太奇 React to death — play death montage */
	UFUNCTION()
	void OnDeath(FGameplayTag DamageType, AActor* DeathCauser);

	UFUNCTION()
	void OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	void PlaySlideMontage();
	void EndSlide();
	void PlayHitReaction(FGameplayTag DamageType);
	void PlayDeathMontage(FGameplayTag DamageType);

	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

	float BaseMaxWalkSpeed = 1200.0f;
};
