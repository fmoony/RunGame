#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "CombatRuntimeState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCR_OnHealthChanged, float, CurrentHP, float, MaxHP, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCR_OnDamageTaken, float, Damage, FGameplayTag, DamageType, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCR_OnDeath, FGameplayTag, DamageType, AActor*, DeathCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCR_OnInvincibilityChanged, bool, bNewInvincible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCR_OnSkillActivated, FGameplayTag, SkillTag, float, CooldownDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCR_OnSkillReady, FGameplayTag, SkillTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCR_OnSkillExecuted, FGameplayTag, SkillTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCR_OnEnergyChanged, float, CurrentEnergy, float, MaxEnergy);

/**
 * 战斗运行时状态 —— 生命值、无敌、能量、技能广播。
 * 通过 GetWorld()->GetSubsystem<UCombatRuntimeState>() 访问。
 */
UCLASS()
class RUNGAME_API UCombatRuntimeState : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// ---- Health ----

	void ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser);
	void Heal(float Amount, AActor* Healer);
	void Revive(float RestoreHP);

	UFUNCTION(BlueprintCallable, Category = "Combat|Health")
	void SetInvincible(bool bNewInvincible);

	UFUNCTION(BlueprintPure, Category = "Combat|Health")
	float GetCurrentHP() const { return CurrentHP; }

	void SetMaxHP(float InMaxHP) { MaxHP = InMaxHP; }
	UFUNCTION(BlueprintPure, Category = "Combat|Health")
	float GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintPure, Category = "Combat|Health")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Combat|Health")
	bool IsInvincible() const { return bIsInvincible; }

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnDamageTaken OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnInvincibilityChanged OnInvincibilityChanged;

	// ---- Skill / Energy ----

	void SetEnergy(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Combat|Skill")
	void AddEnergy(float Amount);

	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetCurrentEnergy() const { return CurrentEnergy; }

	void SetMaxEnergy(float InMaxEnergy) { MaxEnergy = InMaxEnergy; }
	UFUNCTION(BlueprintPure, Category = "Combat|Skill")
	float GetMaxEnergy() const { return MaxEnergy; }

	UFUNCTION(BlueprintCallable, Category = "Combat|Skill")
	void BroadcastSkillActivated(FGameplayTag SkillTag, float CooldownDuration);

	UFUNCTION(BlueprintCallable, Category = "Combat|Skill")
	void BroadcastSkillReady(FGameplayTag SkillTag);

	UFUNCTION(BlueprintCallable, Category = "Combat|Skill")
	void BroadcastSkillExecuted(FGameplayTag SkillTag);

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnSkillActivated OnSkillActivated;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnSkillReady OnSkillReady;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnSkillExecuted OnSkillExecuted;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FCR_OnEnergyChanged OnEnergyChanged;

	// ---- Lifecycle ----

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	float CurrentHP = 100.0f;
	float MaxHP = 100.0f;
	bool bIsDead = false;
	bool bIsInvincible = false;
	float CurrentEnergy = 100.0f;
	float MaxEnergy = 100.0f;
};
