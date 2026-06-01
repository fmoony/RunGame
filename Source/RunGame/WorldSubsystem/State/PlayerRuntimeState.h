#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "PlayerRuntimeState.generated.h"

class ARunGameCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPR_OnCharacterStateChanged, ERunGameCharacterState, OldState, ERunGameCharacterState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPR_OnCharacterDied, FGameplayTag, DamageType, ARunGameCharacter*, DeadCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPR_OnHitReaction, float, Damage, FGameplayTag, DamageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPR_OnDeathAnimationFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPR_OnEffectTagChanged, FGameplayTag, Tag, bool, bAdded);

/**
 * 角色状态机——纯事件总线。
 * 管理 CurrentCharacterState 的转移校验和广播，所有受限于此状态机的系统直接绑定 OnCharacterStateChanged 自行响应。
 * Character state machine — pure event bus.
 * Manages CurrentCharacterState transitions and broadcasts. All constrained systems bind directly and self-respond.
 */
UCLASS()
class RUNGAME_API UPlayerRuntimeState : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// ---- Character State ----

	UFUNCTION(BlueprintCallable, Category = "Player|Character")
	void SetCharacterState(ERunGameCharacterState NewState);

	UFUNCTION(BlueprintPure, Category = "Player|Character")
	ERunGameCharacterState GetCharacterState() const { return CurrentCharacterState; }

	UFUNCTION(BlueprintPure, Category = "Player|Character")
	bool IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnCharacterStateChanged OnCharacterStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnCharacterDied OnCharacterDied;

	// ---- Animation Events ----

	/** Character 桥接：HealthComponent 伤害 → RuntimeState 广播 → AnimInstance 设触发变量 */
	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnHitReaction OnHitReaction;

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void TriggerHitReaction(float Damage, FGameplayTag DamageType);

	/** AnimNotify 桥接：死亡动画末尾 → RuntimeState 广播 → Character 溶解 */
	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnDeathAnimationFinished OnDeathAnimationFinished;

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void NotifyDeathAnimationFinished();

	// ---- Effect Tags (事件总线) ----

	/** 活跃效果标签容器——增删时广播 OnEffectTagsChanged Active effect tags — broadcast OnEffectTagsChanged on add/remove */
	UPROPERTY(BlueprintReadOnly, Category = "Player|Effects")
	FGameplayTagContainer ActiveEffectTags;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnEffectTagChanged OnEffectTagChanged;

	UFUNCTION(BlueprintCallable, Category = "Player|Effects")
	void AddEffectTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Player|Effects")
	void RemoveEffectTag(FGameplayTag Tag);

	/** 重置为新游戏——清除角色状态和效果标签 Reset for new game: clear character state and effect tags */
	void ResetForNewGame();

	// ---- Lifecycle ----

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	ERunGameCharacterState CurrentCharacterState = ERunGameCharacterState::Idle;
};
