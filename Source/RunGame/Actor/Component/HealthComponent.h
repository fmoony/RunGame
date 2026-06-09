// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "HealthComponent.generated.h"

class UPlayerRuntimeState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, CurrentHP, float, MaxHP, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageTaken, float, Damage, FGameplayTag, DamageType, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, FGameplayTag, DamageType, AActor*, DeathCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvincibilityChangedSignature, bool, bNewInvincible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldChangedSignature, float, CurrentShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldBrokenSignature);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	/** 最大生命值 Maximum health — set in Blueprint or C++ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 1))
	float MaxHP = 100.0f;

	/** Broadcast when HP changes — positive Delta = healing, negative = damage */
	UPROPERTY(BlueprintAssignable, Category = "Health|Delegates")
	FOnHealthChanged OnHealthChanged;

	/** Broadcast when damage is applied and HP is still above 0 — for hit reactions. Mutually exclusive with OnDeath */
	UPROPERTY(BlueprintAssignable, Category = "Health|Delegates")
	FOnDamageTaken OnDamageTaken;

	/** Broadcast when HP reaches 0 */
	UPROPERTY(BlueprintAssignable, Category = "Health|Delegates")
	FOnDeath OnDeath;

	/** Broadcast when invincibility state changes */
	UPROPERTY(BlueprintAssignable, Category = "Health|Delegates")
	FOnInvincibilityChangedSignature OnInvincibilityChanged;

	/** 扣除生命值并广播。如 HP <= 0 则广播 OnDeath。无敌状态下无效 Subtract HP and broadcast. If HP <= 0, broadcasts OnDeath. No-op while invincible */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser);

	/** 恢复生命值至多到 MaxHP 并广播。死亡后无效 Restore HP up to MaxHP and broadcast. No-op if dead */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float Amount, AActor* Healer);

	/** 强制击杀——无视无敌、直接致死 Force kill — ignores invincibility, sets HP=0, broadcasts OnDeath */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ForceKill(FGameplayTag DamageType, AActor* Killer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetCurrentHP() const { return CurrentHP; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetHealthPercentage() const;

	/** HP <= 0 即判定死亡——单一数据源，无需与 PlayerRuntimeState 同步
	 *  HP <= 0 means dead — single source of truth, no sync with PlayerRuntimeState needed */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	bool IsDead() const { return CurrentHP <= 0.0f; }

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsInvincible() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Revive(float RestoreHP);

	// ---- 标签查询 Tag queries ----

	/** 匹配后即视为无敌的标签查询 Tags matching this query grant invincibility */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Tags")
	FGameplayTagQuery InvincibilityTagQuery;

	/** 匹配后即激活护盾的标签查询 Tags matching this query activate shield */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Tags")
	FGameplayTagQuery ShieldTagQuery;

	/** 护盾激活时的默认吸收量 Default shield absorption amount when shield tag becomes active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield")
	float DefaultShieldAmount = 50.0f;

	// ---- 护盾 Shield ----

	/** 添加护盾 — 伤害优先扣盾再扣血 Add shield — damage absorbs shield before HP */
	UFUNCTION(BlueprintCallable, Category = "Health|Shield")
	void AddShield(float Amount);

	/** 移除护盾 — 清空剩余盾量 Remove shield — clear remaining shield */
	UFUNCTION(BlueprintCallable, Category = "Health|Shield")
	void RemoveShield();

	UFUNCTION(BlueprintPure, Category = "Health|Shield")
	float GetShieldHP() const { return ShieldHP; }

	/** 护盾值变化时广播 Broadcast when shield amount changes */
	UPROPERTY(BlueprintAssignable, Category = "Health|Delegates")
	FOnShieldChangedSignature OnShieldChanged;

	/** 护盾被击破时广播 Broadcast when shield breaks */
	UPROPERTY(BlueprintAssignable, Category = "Health|Delegates")
	FOnShieldBrokenSignature OnShieldBroken;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnEffectTagChanged(FGameplayTag Tag, bool bAdded);

	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	bool IsTagRelevantToQuery(FGameplayTag Tag, const FGameplayTagQuery& Query) const;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> CachedPRS;

	float CurrentHP = 0.0f;
	float ShieldHP = 0.0f;
	int32 InvincibilityTagCount = 0;
	int32 ShieldTagCount = 0;
};
