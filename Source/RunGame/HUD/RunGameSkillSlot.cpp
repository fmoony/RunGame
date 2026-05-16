// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/RunGameSkillSlot.h"
#include "Actor/Component/SkillComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Skill/RunGameSkillConfigData.h"

URunGameSkillSlot::URunGameSkillSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, SkillIcon(nullptr)
	, CooldownOverlay(nullptr)
	, KeyHintText(nullptr)
	, CooldownBaseMaterial(nullptr)
	, CooldownMID(nullptr)
{
}

void URunGameSkillSlot::SetupSlot(const FSkillDefinition& SkillDef, FGameplayTag InSkillTag, USkillComponent* InComp)
{
	SkillTag = InSkillTag;
	CachedSkillComponent = InComp;

	if (KeyHintText)
	{
		KeyHintText->SetText(SkillDef.KeyHint);
	}

	// Async-load the icon texture from soft reference
	PendingIcon = SkillDef.SkillIcon;
	if (UTexture2D* CachedIcon = PendingIcon.Get())
	{
		SkillIcon->SetBrushFromTexture(CachedIcon);
	}
	else if (!PendingIcon.IsNull())
	{
		FSoftObjectPath IconPath = PendingIcon.ToSoftObjectPath();
		IconLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
			IconPath,
			FStreamableDelegate::CreateUObject(this, &URunGameSkillSlot::OnIconLoaded)
		);
	}

	// Create MID from base material for cooldown radial fill
	if (CooldownBaseMaterial)
	{
		CooldownMID = UMaterialInstanceDynamic::Create(CooldownBaseMaterial, this);

		if (CooldownOverlay)
		{
			CooldownOverlay->SetBrushFromMaterial(CooldownMID);
			CooldownOverlay->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Bind to SkillComponent delegates
	if (USkillComponent* Comp = CachedSkillComponent.Get())
	{
		Comp->OnSkillActivated.AddDynamic(this, &URunGameSkillSlot::OnSkillActivated_Callback);
		Comp->OnSkillReady.AddDynamic(this, &URunGameSkillSlot::OnSkillReady_Callback);
	}
}

void URunGameSkillSlot::NativeDestruct()
{
	if (USkillComponent* Comp = CachedSkillComponent.Get())
	{
		Comp->OnSkillActivated.RemoveDynamic(this, &URunGameSkillSlot::OnSkillActivated_Callback);
		Comp->OnSkillReady.RemoveDynamic(this, &URunGameSkillSlot::OnSkillReady_Callback);
	}

	if (IconLoadHandle.IsValid())
	{
		IconLoadHandle->CancelHandle();
		IconLoadHandle.Reset();
	}

	CachedSkillComponent.Reset();
	CooldownMID = nullptr;

	Super::NativeDestruct();
}

void URunGameSkillSlot::OnSkillActivated_Callback(FGameplayTag ActivatedTag, float CooldownDuration)
{
	if (ActivatedTag != SkillTag)
	{
		return;
	}

	if (CooldownMID && CooldownOverlay)
	{
		CooldownMID->SetScalarParameterValue(CooldownStartTimeParamName, GetWorld()->GetTimeSeconds());
		CooldownMID->SetScalarParameterValue(CooldownDurationParamName, CooldownDuration);
		CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void URunGameSkillSlot::OnSkillReady_Callback(FGameplayTag ReadyTag)
{
	if (ReadyTag != SkillTag)
	{
		return;
	}

	if (CooldownOverlay)
	{
		CooldownOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void URunGameSkillSlot::OnIconLoaded()
{
	IconLoadHandle.Reset();

	if (SkillIcon)
	{
		if (UTexture2D* LoadedTex = PendingIcon.Get())
		{
			SkillIcon->SetBrushFromTexture(LoadedTex);
		}
	}
}
