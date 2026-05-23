#include "Actor/Component/SkillComponent.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Skill/RunGameSkillExecution.h"
#include "WorldSubsystem/State/CombatRuntimeState.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/World.h"
#include "TimerManager.h"
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

	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		// 同步配置
		RS->SetMaxEnergy(MaxEnergy);
		RS->SetEnergy(MaxEnergy);

		// 绑定转发器
		RS->OnEnergyChanged.AddDynamic(this, &USkillComponent::OnRS_EnergyChanged);
		RS->OnSkillActivated.AddDynamic(this, &USkillComponent::OnRS_SkillActivated);
		RS->OnSkillReady.AddDynamic(this, &USkillComponent::OnRS_SkillReady);
	}

	StartEnergyRegen();

	// 广播初始能量状态
	OnEnergyChanged.Broadcast(MaxEnergy, MaxEnergy);
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

		if (UCombatRuntimeState* RS = World->GetSubsystem<UCombatRuntimeState>())
		{
			RS->OnEnergyChanged.RemoveDynamic(this, &USkillComponent::OnRS_EnergyChanged);
			RS->OnSkillActivated.RemoveDynamic(this, &USkillComponent::OnRS_SkillActivated);
			RS->OnSkillReady.RemoveDynamic(this, &USkillComponent::OnRS_SkillReady);
		}
	}

	StopEnergyRegen();
	SkillStates.Empty();

	Super::EndPlay(EndPlayReason);
}

// ---- RS 转发器 ----

void USkillComponent::OnRS_EnergyChanged(float CurrentEnergy, float InMaxEnergy)
{
	OnEnergyChanged.Broadcast(CurrentEnergy, InMaxEnergy);
}

void USkillComponent::OnRS_SkillActivated(FGameplayTag SkillTag, float CooldownDuration)
{
	OnSkillActivated.Broadcast(SkillTag, CooldownDuration);
}

void USkillComponent::OnRS_SkillReady(FGameplayTag SkillTag)
{
	OnSkillReady.Broadcast(SkillTag);
}

// ---- Init ----

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

// ---- Skill Activation ----

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
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: Skill '%s' is not ready"), *SkillTag.ToString());
		return false;
	}

	const FSkillDefinition* SkillDef = SkillConfig->FindSkillByTag(SkillTag);
	if (!SkillDef)
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: Skill definition for tag '%s' not found"), *SkillTag.ToString());
		return false;
	}

	UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>();
	if (!RS)
	{
		return false;
	}

	const float CurrentEnergy = RS->GetCurrentEnergy();
	if (CurrentEnergy < SkillDef->EnergyCost)
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: Skill '%s' requires %.1f energy but only %.1f available"), *SkillTag.ToString(), SkillDef->EnergyCost, CurrentEnergy);
		return false;
	}

	AActor* const Owner = GetOwner();
	USkillExecutionBase* ExecObj = nullptr;
	if (SkillDef->ExecutionClass)
	{
		ExecObj = NewObject<USkillExecutionBase>(GetTransientPackage(), SkillDef->ExecutionClass);
		if (!ExecObj->CanExecute(Owner, SkillTag))
		{
			return false;
		}
	}
	else
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill: Skill '%s' has no ExecutionClass"), *SkillTag.ToString());
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

	// 扣减能量并写入 RS
	RS->SetEnergy(CurrentEnergy - SkillDef->EnergyCost);

	if (ExecObj)
	{
		ExecObj->Execute(Owner, SkillTag);
	}

	RS->BroadcastSkillActivated(SkillTag, CooldownDuration);

	if (CooldownDuration <= 0.0f)
	{
		State->bOnCooldown = false;
		RS->BroadcastSkillReady(SkillTag);
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

	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->BroadcastSkillReady(SkillTag);
	}
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

// -- Energy ----

float USkillComponent::GetCurrentEnergy() const
{
	if (const UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		return RS->GetCurrentEnergy();
	}
	return 0.0f;
}

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

	return GetCurrentEnergy() >= SkillDef->EnergyCost;
}

void USkillComponent::AddEnergy(float Amount)
{
	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->AddEnergy(Amount);
	}
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
	UCombatRuntimeState* CRS = GetWorld()->GetSubsystem<UCombatRuntimeState>();
	UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!CRS || !PRS)
	{
		return;
	}

	const float CurrentEnergy = CRS->GetCurrentEnergy();
	if (CurrentEnergy >= MaxEnergy)
	{
		return;
	}

	// 跨领域读取：分数在 PlayerRuntimeState，能量在 CombatRuntimeState
	const float ScoreBonus = static_cast<float>(PRS->GetRunGameScore()) * ScoreRegenMultiplier;

	const float RegenAmount = (BaseEnergyRegenPerSecond + ScoreBonus + EnergyRegenModifier) * EnergyRegenMultiplier * 0.1f;
	CRS->AddEnergy(RegenAmount);
}
