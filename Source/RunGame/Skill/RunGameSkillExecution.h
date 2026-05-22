// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "RunGameSkillExecution.generated.h"

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

protected:
	bool CanExecute_Implementation(AActor* Instigator, FGameplayTag SkillTag) const { return true; }
	virtual void Execute_Implementation(AActor* Instigator, FGameplayTag SkillTag) {}
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
