// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Skill/RunGameSkillConfigData.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float BaseEnergyRegenPerSecond = 1.0f;

	/** Bonus regen per score point. Final regen = (Base + Score * Multiplier + Modifier) * RegenMultiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Energy")
	float ScoreRegenMultiplier = 0.001f;

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

	void OnCooldownExpired(FGameplayTag SkillTag);

	// ---- RS 转发器 ----
	UFUNCTION()
	void OnRS_EnergyChanged(float CurrentEnergy, float InMaxEnergy);

	UFUNCTION()
	void OnRS_SkillActivated(FGameplayTag SkillTag, float CooldownDuration);

	UFUNCTION()
	void OnRS_SkillReady(FGameplayTag SkillTag);

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
