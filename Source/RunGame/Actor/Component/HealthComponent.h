// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, CurrentHP, float, MaxHP, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageTaken, float, Damage, FGameplayTag, DamageType, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, FGameplayTag, DamageType, AActor*, DeathCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvincibilityChangedSignature, bool, bNewInvincible);

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

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetInvincible(bool bNewInvincible);

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsInvincible() const { return bIsInvincible; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Revive(float RestoreHP);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	float CurrentHP = 0.0f;
	bool bIsInvincible = false;
};
