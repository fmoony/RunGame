// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, CurrentHP, float, MaxHP, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageTaken, float, Damage, FGameplayTag, DamageType, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, FGameplayTag, DamageType, AActor*, DeathCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvincibilityChangedSignature, bool, bNewInvincible);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	/** 配置属性 —— BeginPlay 时同步到 CombatRuntimeState */
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

	/** Subtract HP and broadcast OnHealthChanged. If HP <= 0, broadcasts OnDeath */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser);

	/** Restore HP up to MaxHP and broadcast OnHealthChanged. No effect if dead */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float Amount, AActor* Healer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetCurrentHP() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetInvincible(bool bNewInvincible);

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsInvincible() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Revive(float RestoreHP);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnRS_HealthChanged(float CurrentHP, float InMaxHP, float InDelta);

	UFUNCTION()
	void OnRS_DamageTaken(float Damage, FGameplayTag DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnRS_Death(FGameplayTag DamageType, AActor* DeathCauser);

	UFUNCTION()
	void OnRS_InvincibilityChanged(bool bNewInvincible);
};
