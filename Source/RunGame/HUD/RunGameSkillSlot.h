// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "RunGameSkillSlot.generated.h"

class UImage;
class UTextBlock;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class USkillComponent;
struct FSkillDefinition;
struct FStreamableHandle;

/**
 * Single skill slot widget — displays icon, key hint, and material-driven cooldown overlay.
 * Created dynamically by URunGameInGame from USkillConfigData.
 */
UCLASS()
class RUNGAME_API URunGameSkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	URunGameSkillSlot(const FObjectInitializer& ObjectInitializer);

	/** Initialize this slot from a skill definition and bind to the component delegates */
	void SetupSlot(const FSkillDefinition& SkillDef, FGameplayTag InSkillTag, USkillComponent* InComp);

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SkillIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CooldownOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyHintText;

	/** Base material for cooldown radial fill. MID is created from this in SetupSlot */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Slot")
	TObjectPtr<UMaterialInterface> CooldownBaseMaterial;

	/** Scalar parameter name for cooldown fill percent (1.0 = full cooldown, 0.0 = ready). Passed directly to MID */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Slot")
	FName CooldownPercentParamName = "CooldownPercent";

private:
	FGameplayTag SkillTag;
	TWeakObjectPtr<USkillComponent> CachedSkillComponent;
	TObjectPtr<UMaterialInstanceDynamic> CooldownMID;

	/** Soft reference kept for resolution after async load completes */
	TSoftObjectPtr<UTexture2D> PendingIcon;

	TSharedPtr<FStreamableHandle> IconLoadHandle;

	UFUNCTION()
	void OnSkillActivated_Callback(FGameplayTag ActivatedTag, float CooldownDuration);

	UFUNCTION()
	void OnSkillReady_Callback(FGameplayTag ReadyTag);

	/** Per-slot timer callback — reads GetCooldownRemaining from component, writes to MID. Runs at 0.05s */
	UFUNCTION()
	void UpdateCooldownDisplay();

	/** Called when the soft-referenced icon texture finishes async loading */
	void OnIconLoaded();

	FTimerHandle CooldownDisplayTimer;

	float CachedCooldownDuration = 0.0f;
};
