// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Skill/RunGameSkillExecution.h"
#include "SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillActivatedSignature, FGameplayTag, SkillTag, float, CooldownDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillReadySignature, FGameplayTag, SkillTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillExecutedSignature, FGameplayTag, SkillTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnergyChangedSignature, float, CurrentEnergy, float, MaxEnergy);

USTRUCT()
struct FSkillRuntimeState
{
	GENERATED_BODY()

	bool bOnCooldown = false;
	FTimerHandle CooldownTimer;

	/** 缓存复用的执行对象——初始化时创建，激活时复用 Cached execution object — created once, reused on activation */
	UPROPERTY()
	TObjectPtr<USkillExecutionBase> ExecutionObject;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	/** 技能配置数据资产——定义所有可用技能 Skill configuration data asset — defines all available skills */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<USkillConfigData> SkillConfig;

	// -- Energy configuration --

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float MaxEnergy = 100.0f;

	/** 游戏开始时的能量值。0 = 从空开始 Energy at game start. 0 = start empty, match MaxEnergy for full bar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float InitialEnergy = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float BaseEnergyRegenPerSecond = 0.333f;

	/** 分数奖励乘数（经 sqrt 缩放）。Bonus = sqrt(分数) × 乘数 Score bonus multiplier (sqrt-scaled). Bonus = sqrt(Score) × Multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float ScoreRegenMultiplier = 0.005f;

	// -- Delegates --

	/** 技能激活时广播——UI 绑定以驱动 MID 冷却参数 Broadcast when a skill is activated — UI binds to drive MID cooldown parameters */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillActivatedSignature OnSkillActivated;

	/** 技能冷却到期时广播 Broadcast when a skill's cooldown expires */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillReadySignature OnSkillReady;

	/** 技能执行时广播——Character 绑定以实现技能行为 Broadcast when a skill is executed — Character binds to implement skill behavior */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillExecutedSignature OnSkillExecuted;

	/** 能量变化时广播——UI 绑定以更新能量条 Broadcast when energy changes — UI binds to update energy bar */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnEnergyChangedSignature OnEnergyChanged;

	/** 尝试激活技能，成功返回 true Attempt to activate a skill by tag. Returns true on success */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool TryActivateSkill(FGameplayTag SkillTag);

	UFUNCTION(BlueprintPure, Category = "Skill")
	bool IsSkillReady(FGameplayTag SkillTag) const;

	/** 剩余冷却时间（秒）。0 = 就绪 Seconds remaining on cooldown. 0 if ready */
	UFUNCTION(BlueprintPure, Category = "Skill")
	float GetCooldownRemaining(FGameplayTag SkillTag) const;

	UFUNCTION(BlueprintPure, Category = "Skill")
	int32 GetSkillCount() const { return SkillConfig ? SkillConfig->Skills.Num() : 0; }

	/** 按标签获取技能定义。未找到返回默认构造结构体 Get skill definition by tag. Returns default-constructed struct with invalid tag if not found */
	UFUNCTION(BlueprintPure, Category = "Skill")
	FSkillDefinition GetSkillDefinitionByTag(FGameplayTag SkillTag) const;

	TArray<FGameplayTag> GetSkillTags() const;

	// -- Energy API --

	UFUNCTION(BlueprintPure, Category = "Skill|Energy")
	float GetCurrentEnergy() const { return CurrentEnergy; }

	UFUNCTION(BlueprintPure, Category = "Skill|Energy")
	bool HasEnoughEnergy(FGameplayTag SkillTag) const;

	/** 场景道具接口——立即增加能量（钳制至 MaxEnergy） Scene prop interface — instantly adds energy (clamped to MaxEnergy) */
	UFUNCTION(BlueprintCallable, Category = "Skill|Energy")
	void AddEnergy(float Amount);

	/** 场景道具接口——加减固定回复速率奖励 Scene prop interface — adds/subtracts a flat regen rate bonus */
	UFUNCTION(BlueprintCallable, Category = "Skill|Energy")
	void AddEnergyRegenModifier(float Delta);

	/** 场景道具接口——倍乘总回复速率（1.0 = 正常） Scene prop interface — multiplies total regen rate (1.0 = normal) */
	UFUNCTION(BlueprintCallable, Category = "Skill|Energy")
	void SetEnergyRegenMultiplier(float Mult);

	/** 取消所有活跃冷却并清除定时器 Cancel all active cooldowns and clear timers */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ClearAllCooldowns();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitializeFromConfig();

private:
	TMap<FGameplayTag, FSkillRuntimeState> SkillStates;

	/** 当前活跃的技能执行对象——死亡时取消其定时器 Active skill execution tracked for cancellation on death */
	TWeakObjectPtr<USkillExecutionBase> ActiveExecution;

	void OnCooldownExpired(FGameplayTag SkillTag);

	/** 响应角色状态机：进入 Dead 时自行清空冷却和能量 React to character state: clear cooldowns and energy on death */
	UFUNCTION()
	void OnRS_CharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	// -- Energy runtime state --

	float CurrentEnergy = 0.0f;

	/** 来自场景道具的固定加减奖励 Flat additive bonus from scene props (can be negative) */
	float EnergyRegenModifier = 0.0f;

	/** 来自场景道具的全局乘数（1.0 = 正常） Global multiplier from scene props (1.0 = normal) */
	float EnergyRegenMultiplier = 1.0f;

	FTimerHandle EnergyRegenTimer;

	void StartEnergyRegen();
	void StopEnergyRegen();
	void TickEnergyRegen();
};
