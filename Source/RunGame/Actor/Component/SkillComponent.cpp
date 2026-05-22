// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Component/SkillComponent.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerState.h"
#include "RunGamePlayerState.h"
#include "RunGame.h"

USkillComponent::USkillComponent()
	: SkillConfig(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeFromConfig();

	// Cache PlayerState for score-based energy regen acceleration
	if (AActor* Owner = GetOwner())
	{
		if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
		{
			CachedPlayerState = PC->GetPlayerState<ARunGamePlayerState>();
		}
	}

	StartEnergyRegen();

	// Broadcast initial energy state so UI initializes correctly
	OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
}

void USkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		for (auto& Pair : SkillStates)
		{
			if (Pair.Value.bOnCooldown)
			{
				TimerManager.ClearTimer(Pair.Value.CooldownTimer);
			}
		}
	}

	StopEnergyRegen();
	SkillStates.Empty();
	CachedPlayerState = nullptr;

	Super::EndPlay(EndPlayReason);
}

void USkillComponent::InitializeFromConfig()
{
	if (!SkillConfig)
	{
		return;
	}

	SkillStates.Reset();

	for (const FSkillDefinition& SkillDef : SkillConfig->Skills)
	{
		if (SkillDef.SkillTag.IsValid())
		{
			SkillStates.Add(SkillDef.SkillTag, FSkillRuntimeState());
		}
	}
}

bool USkillComponent::TryActivateSkill(FGameplayTag SkillTag)
{
	if (!SkillConfig)
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: SkillConfig is not set on %s"), *GetNameSafe(this));
		return false;
	}

	FSkillRuntimeState* State = SkillStates.Find(SkillTag);
	if (!State || State->bOnCooldown)
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: Skill '%s' is not ready (either not found or on cooldown)"), *SkillTag.ToString());
		return false;
	}

	const FSkillDefinition* SkillDef = SkillConfig->FindSkillByTag(SkillTag);
	if (!SkillDef)
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: Skill definition for tag '%s' not found in SkillConfig"), *SkillTag.ToString());
		return false;
	}

	// Check energy
	if (CurrentEnergy < SkillDef->EnergyCost)
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: Skill '%s' requires %.1f energy but only %.1f available"), *SkillTag.ToString(), SkillDef->EnergyCost, CurrentEnergy);
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const float CooldownDuration = SkillDef->CooldownSeconds;

	State->bOnCooldown = true;

	if (CooldownDuration > 0.0f)
	{
		FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &USkillComponent::OnCooldownExpired, SkillTag);
		World->GetTimerManager().SetTimer(State->CooldownTimer, TimerDel, CooldownDuration, false);
	}

	// Consume energy
	CurrentEnergy -= SkillDef->EnergyCost;

	OnSkillActivated.Broadcast(SkillTag, CooldownDuration);
	OnSkillExecuted.Broadcast(SkillTag);
	OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);

	// Handle zero-cooldown skills: fire OnSkillReady immediately after activation
	if (CooldownDuration <= 0.0f)
	{
		State->bOnCooldown = false;
		OnSkillReady.Broadcast(SkillTag);
	}

	UE_LOG(LogRunGame, Warning, TEXT("USkillComponent: Activated skill '%s' with cooldown %.2f seconds"), *SkillTag.ToString(), CooldownDuration);

	return true;
}

void USkillComponent::OnCooldownExpired(FGameplayTag SkillTag)
{
	FSkillRuntimeState* State = SkillStates.Find(SkillTag);
	if (!State)
	{
		return;
	}

	State->bOnCooldown = false;
	State->CooldownTimer.Invalidate();

	OnSkillReady.Broadcast(SkillTag);
}

bool USkillComponent::IsSkillReady(FGameplayTag SkillTag) const
{
	const FSkillRuntimeState* State = SkillStates.Find(SkillTag);
	return State && !State->bOnCooldown;
}

float USkillComponent::GetCooldownRemaining(FGameplayTag SkillTag) const
{
	const FSkillRuntimeState* State = SkillStates.Find(SkillTag);
	if (!State || !State->bOnCooldown)
	{
		return 0.0f;
	}

	if (UWorld* World = GetWorld())
	{
		return World->GetTimerManager().GetTimerRemaining(State->CooldownTimer);
	}

	return 0.0f;
}

FSkillDefinition USkillComponent::GetSkillDefinitionByTag(FGameplayTag SkillTag) const
{
	if (SkillConfig)
	{
		if (const FSkillDefinition* Found = SkillConfig->FindSkillByTag(SkillTag))
		{
			return *Found;
		}
	}
	return FSkillDefinition();
}

TArray<FGameplayTag> USkillComponent::GetSkillTags() const
{
	TArray<FGameplayTag> Tags;
	if (SkillConfig)
	{
		for (const FSkillDefinition& SkillDef : SkillConfig->Skills)
		{
			if (SkillDef.SkillTag.IsValid())
			{
				Tags.Add(SkillDef.SkillTag);
			}
		}
	}
	return Tags;
}

// -- Energy methods --

bool USkillComponent::HasEnoughEnergy(FGameplayTag SkillTag) const
{
	if (!SkillConfig)
	{
		return false;
	}

	const FSkillDefinition* SkillDef = SkillConfig->FindSkillByTag(SkillTag);
	if (!SkillDef)
	{
		return false;
	}

	return CurrentEnergy >= SkillDef->EnergyCost;
}

void USkillComponent::AddEnergy(float Amount)
{
	CurrentEnergy = FMath::Min(MaxEnergy, CurrentEnergy + Amount);
	OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
}

void USkillComponent::AddEnergyRegenModifier(float Delta)
{
	EnergyRegenModifier += Delta;
}

void USkillComponent::SetEnergyRegenMultiplier(float Mult)
{
	EnergyRegenMultiplier = Mult;
}

void USkillComponent::StartEnergyRegen()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			EnergyRegenTimer,
			this,
			&USkillComponent::TickEnergyRegen,
			0.1f,
			true
		);
	}
}

void USkillComponent::StopEnergyRegen()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(EnergyRegenTimer);
	}
}

void USkillComponent::TickEnergyRegen()
{
	if (CurrentEnergy >= MaxEnergy)
	{
		return;
	}

	float ScoreBonus = 0.0f;
	if (CachedPlayerState)
	{
		ScoreBonus = static_cast<float>(CachedPlayerState->GetRunGameScore()) * ScoreRegenMultiplier;
	}

	const float RegenAmount = (BaseEnergyRegenPerSecond + ScoreBonus + EnergyRegenModifier) * EnergyRegenMultiplier * 0.1f;
	CurrentEnergy = FMath::Min(MaxEnergy, CurrentEnergy + RegenAmount);
	OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
}
