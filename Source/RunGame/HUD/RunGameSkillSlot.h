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

	/** Scalar parameter name for activation timestamp (e.g. "CooldownStartTime") */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Slot")
	FName CooldownStartTimeParamName = "CooldownStartTime";

	/** Scalar parameter name for cooldown duration (e.g. "CooldownDuration") */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Slot")
	FName CooldownDurationParamName = "CooldownDuration";

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

	/** Called when the soft-referenced icon texture finishes async loading */
	void OnIconLoaded();
};
