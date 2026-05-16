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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	/** Skill configuration data asset — defines all available skills */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<USkillConfigData> SkillConfig;

	/** Broadcast when a skill is activated — UI binds to drive MID cooldown parameters */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillActivatedSignature OnSkillActivated;

	/** Broadcast when a skill's cooldown expires */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillReadySignature OnSkillReady;

	/** Broadcast when a skill is executed — Character binds to implement skill behavior */
	UPROPERTY(BlueprintAssignable, Category = "Skill|Delegates")
	FOnSkillExecutedSignature OnSkillExecuted;

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
};
