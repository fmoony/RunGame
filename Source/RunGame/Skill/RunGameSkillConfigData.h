// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RunGameSkillConfigData.generated.h"

class UInputAction;

/**
 * Defines a single skill: its identity, visuals, cooldown, and input binding.
 */
USTRUCT(BlueprintType)
struct RUNGAME_API FSkillDefinition
{
	GENERATED_BODY()

	/** Unique identifier for this skill — used for activation, dispatch, and delegate routing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FGameplayTag SkillTag;

	/** Display name shown in tooltips or UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FText SkillName;

	/** Icon texture — soft reference, async-loaded by the skill slot widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSoftObjectPtr<UTexture2D> SkillIcon;

	/** Keyboard hint text shown on the skill slot (e.g. "Q", "E") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FText KeyHint;

	/** Cooldown duration in seconds. 0 = no cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CooldownSeconds = 5.0f;

	/** Input action bound dynamically by the character in SetupPlayerInputComponent */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Input")
	TObjectPtr<UInputAction> InputAction;
};

/**
 * Data asset that defines all available skills.
 * Follows the UFloorConfigData pattern — a single asset holds the full skill table.
 */
UCLASS(BlueprintType)
class RUNGAME_API USkillConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TArray<FSkillDefinition> Skills;

	/** Find a skill definition by tag. Returns nullptr if not found */
	const FSkillDefinition* FindSkillByTag(FGameplayTag Tag) const
	{
		for (const FSkillDefinition& Skill : Skills)
		{
			if (Skill.SkillTag == Tag)
			{
				return &Skill;
			}
		}
		return nullptr;
	}

	/** Find the index of a skill by tag. Returns INDEX_NONE if not found */
	int32 FindSkillIndexByTag(FGameplayTag Tag) const
	{
		for (int32 i = 0; i < Skills.Num(); ++i)
		{
			if (Skills[i].SkillTag == Tag)
			{
				return i;
			}
		}
		return INDEX_NONE;
	}
};
