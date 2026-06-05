// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "RunGameSkillExecution.generated.h"

class UHealthComponent;

/**
 * Base class for skill execution behavior.
 *
 * Each skill can reference a subclass (Blueprint or C++) that defines:
 * - CanExecute: additional constraints beyond cooldown / energy (e.g., target in range, specific state)
 * - Execute: the actual gameplay effect (montage, projectile, buff, impulse, etc.)
 *
 * Created once per-activation by SkillComponent::TryActivateSkill.
 * The object is transient — created on demand, GC'd after use.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class RUNGAME_API USkillExecutionBase : public UObject
{
	GENERATED_BODY()

public:
	/** Check additional constraints before skill fires. Return false to block activation */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Execution")
	bool CanExecute(AActor* Instigator, FGameplayTag SkillTag) const;

	/** Execute the skill's gameplay effect. Called after all validation passes */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Execution")
	void Execute(AActor* Instigator, FGameplayTag SkillTag);

	/** Cancel active effect — called on character death or forced interrupt */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Execution")
	void Cancel(AActor* Instigator);

	/** 每次 Execute 前调用——子类覆写以清零逐次激活的成员变量（如 CachedSkillTag、定时器句柄）
	 *  Called before every Execute — subclasses override to clear per-activation state (CachedSkillTag, timer handles, etc.) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Execution")
	void Reset();

	/** SkillComponent 在 Execute 前传入技能配置的效果 Tag — 子类覆写保存用于后续撤 Tag
	 *  SkillComponent passes effect tags from skill config before Execute — subclasses override to cache for later removal */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Execution")
	void SetupEffectTags(FGameplayTag InSpeedTag, FGameplayTag InDefenseTag);

protected:
	bool CanExecute_Implementation(AActor* Instigator, FGameplayTag SkillTag) const { return true; }
	virtual void Execute_Implementation(AActor* Instigator, FGameplayTag SkillTag) {}
	virtual void Cancel_Implementation(AActor* Instigator) {}
	virtual void Reset_Implementation() {}
	virtual void SetupEffectTags_Implementation(FGameplayTag InSpeedTag, FGameplayTag InDefenseTag) {}
};


/**
 * Concrete execution class that plays a montage and applies a forward impulse.
 * Migrates the old SkillMontages / SkillImpulseStrengths TMap from ARunGameCharacter.
 */
UCLASS(Blueprintable, BlueprintType)
class RUNGAME_API USkillExecution_PlayMontageAndImpulse : public USkillExecutionBase
{
	GENERATED_BODY()

public:
	/** Animation montage played on skill execution */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	TObjectPtr<UAnimMontage> Montage;

	/** Forward impulse strength applied to the instigator */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	float ImpulseStrength = 0.0f;

	virtual void Execute_Implementation(AActor* Instigator, FGameplayTag SkillTag) override;
};


/**
 * Unstoppable skill — speed boost + invincibility for a fixed duration.
 * On execute: saves current speed, applies multiplier, enables invincibility.
 * After Duration seconds: restores speed and disables invincibility.
 */
UCLASS(Blueprintable, BlueprintType)
class RUNGAME_API USkillExecution_Unstoppable : public USkillExecutionBase
{
	GENERATED_BODY()

public:
	/** Speed multiplier applied during the effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	float SpeedMultiplier = 1.5f;

	/** Duration of the effect in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	float Duration = 3.0f;

	virtual void Execute_Implementation(AActor* Instigator, FGameplayTag SkillTag) override;
	virtual void Cancel_Implementation(AActor* Instigator) override;
	virtual void Reset_Implementation() override;
	virtual void SetupEffectTags_Implementation(FGameplayTag InSpeedTag, FGameplayTag InDefenseTag) override;

private:
	/** 加速效果 Tag — 由 SkillComponent 从技能配置传入 Speed effect tag — passed from skill config by SkillComponent */
	FGameplayTag SpeedTag;

	/** 无敌效果 Tag — 由 SkillComponent 从技能配置传入 Defense effect tag — passed from skill config by SkillComponent */
	FGameplayTag DefenseTag;

	FTimerHandle RevertTimer;

	/** Duration 到期 → 撤速度+速度 Tag Duration expiry → remove speed + speed tag */
	void RevertEffect(AActor* Instigator);

	/** 速度插值完成 → 撤无敌 Tag Speed interpolation complete → remove defense tag */
	void RevertInvincibility(AActor* Instigator);
};


/**
 * Shield skill — 一次性护盾，吸收伤害直至耗尽 One-time shield, absorbs damage until depleted.
 * 护盾存在期间 OnShieldBroken 触发时自动清 ShieldTag。
 * Shield tag auto-removed when shield breaks via OnShieldBroken.
 */
UCLASS(Blueprintable, BlueprintType)
class RUNGAME_API USkillExecution_Shield : public USkillExecutionBase
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(AActor* Instigator, FGameplayTag SkillTag) override;
	virtual void Cancel_Implementation(AActor* Instigator) override;
	virtual void Reset_Implementation() override;
	virtual void SetupEffectTags_Implementation(FGameplayTag InSpeedTag, FGameplayTag InDefenseTag) override;

private:
	/** 护盾 Tag — 由 SkillComponent 从 DefenseEffectTag 传入，匹配 HealthComponent 的 ShieldTagQuery Shield tag — passed from DefenseEffectTag, matches HealthComponent ShieldTagQuery */
	FGameplayTag ShieldTag;

	/** 破盾回调 — 自动撤 Tag Auto-remove tag when shield breaks */
	UFUNCTION()
	void OnShieldBrokenCallback();

	UPROPERTY()
	TObjectPtr<UHealthComponent> BoundHealthComponent;
};
