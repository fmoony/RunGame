// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "WorldSubsystem/State/CombatRuntimeState.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Skill/RunGameSkillExecution.h"
#include "SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillActivatedSignature, FGameplayTag, SkillTag, float, CooldownDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillReadySignature, FGameplayTag, SkillTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillExecutedSignature, FGameplayTag, SkillTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnergyChangedSignature, float, CurrentEnergy, float, MaxEnergy);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	/** Skill configuration data asset — defines all available skills */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<USkillConfigData> SkillConfig;

	// -- Energy configuration --

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float MaxEnergy = 100.0f;

	/** Energy at game start. 0 = start empty, match MaxEnergy for full bar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float InitialEnergy = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float BaseEnergyRegenPerSecond = 0.333f;

	/** Score bonus multiplier (sqrt-scaled). Bonus = sqrt(Score) * Multiplier. Tune for explosive score growth */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float ScoreRegenMultiplier = 0.005f;

	// -- Delegates --

	/** Broadcast when a skill is activated — UI binds to drive MID cooldown parameters */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillActivatedSignature OnSkillActivated;

	/** Broadcast when a skill's cooldown expires */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillReadySignature OnSkillReady;

	/** Broadcast when a skill is executed — Character binds to implement skill behavior */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillExecutedSignature OnSkillExecuted;

	/** Broadcast when energy changes — UI binds to update energy bar and skill slot availability */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnEnergyChangedSignature OnEnergyChanged;

	/** Attempt to activate a skill by tag. Returns true on success */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool TryActivateSkill(FGameplayTag SkillTag);

	UFUNCTION(BlueprintPure, Category = "Skill")
	bool IsSkillReady(FGameplayTag SkillTag) const;

	/** Seconds remaining on cooldown. 0 if ready */
	UFUNCTION(BlueprintPure, Category = "Skill")
	float GetCooldownRemaining(FGameplayTag SkillTag) const;

	UFUNCTION(BlueprintPure, Category = "Skill")
	int32 GetSkillCount() const { return SkillConfig ? SkillConfig->Skills.Num() : 0; }

	/** Get skill definition by tag. Returns default-constructed struct with invalid tag if not found */
	UFUNCTION(BlueprintPure, Category = "Skill")
	FSkillDefinition GetSkillDefinitionByTag(FGameplayTag SkillTag) const;	

	TArray<FGameplayTag> GetSkillTags() const;

	// -- Energy API --

	UFUNCTION(BlueprintPure, Category = "Skill|Energy")
	float GetCurrentEnergy() const;

	UFUNCTION(BlueprintPure, Category = "Skill|Energy")
	bool HasEnoughEnergy(FGameplayTag SkillTag) const;

	/** Scene prop interface — instantly adds energy (clamped to MaxEnergy) */
	UFUNCTION(BlueprintCallable, Category = "Skill|Energy")
	void AddEnergy(float Amount);

	/** Scene prop interface — adds/subtracts a flat regen rate bonus */
	UFUNCTION(BlueprintCallable, Category = "Skill|Energy")
	void AddEnergyRegenModifier(float Delta);

	/** Scene prop interface — multiplies total regen rate (1.0 = normal) */
	UFUNCTION(BlueprintCallable, Category = "Skill|Energy")
	void SetEnergyRegenMultiplier(float Mult);

	/** Cancel all active cooldowns and clear timers */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ClearAllCooldowns();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitializeFromConfig();

private:
	struct FSkillRuntimeState
	{
		bool bOnCooldown = false;
		FTimerHandle CooldownTimer;
	};

	TMap<FGameplayTag, FSkillRuntimeState> SkillStates;

	/** 当前活跃的技能执行对象——死亡时取消其定时器 Active skill execution tracked for cancellation on death */
	TWeakObjectPtr<USkillExecutionBase> ActiveExecution;

	void OnCooldownExpired(FGameplayTag SkillTag);

	// ---- RS 转发器 ----
	UFUNCTION()
	void OnRS_EnergyChanged(float CurrentEnergy, float InMaxEnergy);

	UFUNCTION()
	void OnRS_SkillActivated(FGameplayTag SkillTag, float CooldownDuration);

	UFUNCTION()
	void OnRS_SkillReady(FGameplayTag SkillTag);

	/** 响应角色状态机：进入 Dead 时自行清空冷却和能量 */
	UFUNCTION()
	void OnRS_CharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	// -- Energy runtime state --

	/** Flat additive bonus from scene props (can be negative) */
	float EnergyRegenModifier = 0.0f;

	/** Global multiplier from scene props (1.0 = normal) */
	float EnergyRegenMultiplier = 1.0f;

	FTimerHandle EnergyRegenTimer;

	void StartEnergyRegen();
	void StopEnergyRegen();
	void TickEnergyRegen();
};
