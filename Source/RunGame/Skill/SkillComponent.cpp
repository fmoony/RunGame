#include "Skill/SkillComponent.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Skill/RunGameSkillExecution.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Player/RunGamePlayerState.h"
#include "Engine/World.h"
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

	// 初始化能量 Initialize energy
	CurrentEnergy = FMath::Clamp(InitialEnergy, 0.0f, MaxEnergy);

	// 监听角色状态机：死亡时自行清空冷却和能量 Listen to character state: clear cooldowns and energy on death
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddDynamic(this, &USkillComponent::OnRS_CharacterStateChanged);
	}

	// 订阅 Subsystem 的 OnTimeChanged——由规范时钟驱动能量恢复，消除独立定时器
	// Subscribe to Subsystem's OnTimeChanged — canonical clock drives energy regen, eliminates standalone timer
	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();
	if (TimerSubsystem)
	{
		TimerSubsystem->OnTimeChanged.AddDynamic(this, &USkillComponent::OnTimeChangedCallback);
	}

	// 初始启用能量恢复——与旧行为一致（无条件启动） Start energy regen — matches old behavior (unconditional start)
	SetEnergyRegenActive(true);

	// 广播初始能量状态 Broadcast initial energy
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
			Pair.Value.ExecutionObject = nullptr;
		}

		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->OnCharacterStateChanged.RemoveDynamic(this, &USkillComponent::OnRS_CharacterStateChanged);
		}
	}

	if (TimerSubsystem)
	{
		TimerSubsystem->OnTimeChanged.RemoveDynamic(this, &USkillComponent::OnTimeChangedCallback);
	}
	TimerSubsystem = nullptr;

	SkillStates.Empty();

	Super::EndPlay(EndPlayReason);
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
			FSkillRuntimeState State;
			if (SkillDef.ExecutionClass)
			{
				State.ExecutionObject = NewObject<USkillExecutionBase>(this, SkillDef.ExecutionClass);
			}
			SkillStates.Add(SkillDef.SkillTag, State);
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

	// 能量检查 Energy check
	if (CurrentEnergy < SkillDef->EnergyCost)
	{
		UE_LOG(LogRunGame, Warning, TEXT("USkillComponent::TryActivateSkill failed: Skill '%s' requires %.1f energy but only %.1f available"), *SkillTag.ToString(), SkillDef->EnergyCost, CurrentEnergy);
		return false;
	}

	AActor* const Owner = GetOwner();
	USkillExecutionBase* ExecObj = State->ExecutionObject.Get();
	if (ExecObj)
	{
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

	// 扣减能量 Deduct energy
	CurrentEnergy = FMath::Clamp(CurrentEnergy - SkillDef->EnergyCost, 0.0f, MaxEnergy);
	OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);

	ActiveExecutions.Add(ExecObj);

	if (ExecObj)
	{
		ExecObj->Reset();       // 清零逐次激活状态 Clear per-activation state

		// 传入配置的效果 Tag — 执行对象据此管理 PRS Tag 生命周期
		// Pass effect tags from config — execution object uses these to manage PRS tag lifecycle
		ExecObj->SetupEffectTags(SkillDef->SpeedEffectTag, SkillDef->DefenseEffectTag);

		ExecObj->Execute(Owner, SkillTag);
	}

	// 广播技能激活 Broadcast skill activated
	OnSkillActivated.Broadcast(SkillTag, CooldownDuration);

	if (CooldownDuration <= 0.0f)
	{
		State->bOnCooldown = false;
		OnSkillReady.Broadcast(SkillTag);
	}

	UE_LOG(LogRunGame, Warning, TEXT("USkillComponent: Activated skill '%s' with cooldown %.2f seconds"), *SkillTag.ToString(), CooldownDuration);

	return true;
}

ERunGameInputRequestResult USkillComponent::TryActivateRequestedSkill(FGameplayTag SkillTag)
{
	if (!SkillTag.IsValid() || !SkillConfig)
	{
		return ERunGameInputRequestResult::Rejected;
	}

	if (UPlayerRuntimeState* RuntimeState = GetWorld() ? GetWorld()->GetSubsystem<UPlayerRuntimeState>() : nullptr)
	{
		const ERunGameCharacterState State = RuntimeState->GetCharacterState();
		if (State == ERunGameCharacterState::Dead)
		{
			return ERunGameInputRequestResult::Rejected;
		}
		if (State == ERunGameCharacterState::Sliding)
		{
			return ERunGameInputRequestResult::Deferred;
		}
	}

	FSkillRuntimeState* State = SkillStates.Find(SkillTag);
	const FSkillDefinition* SkillDef = SkillConfig->FindSkillByTag(SkillTag);
	if (!State || !SkillDef)
	{
		return ERunGameInputRequestResult::Rejected;
	}

	if (State->bOnCooldown || CurrentEnergy < SkillDef->EnergyCost)
	{
		return ERunGameInputRequestResult::Deferred;
	}

	if (USkillExecutionBase* Execution = State->ExecutionObject.Get())
	{
		if (!Execution->CanExecute(GetOwner(), SkillTag))
		{
			return ERunGameInputRequestResult::Deferred;
		}
	}

	return TryActivateSkill(SkillTag)
		? ERunGameInputRequestResult::Applied
		: ERunGameInputRequestResult::Rejected;
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

// -- Energy ----

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
	const float NewValue = FMath::Clamp(CurrentEnergy + Amount, 0.0f, MaxEnergy);
	if (CurrentEnergy != NewValue)
	{
		CurrentEnergy = NewValue;
		OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
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

void USkillComponent::ClearAllCooldowns()
{
	// 取消所有活跃技能执行 Cancel all active skill executions
	for (auto& Exec : ActiveExecutions)
	{
		if (Exec.IsValid())
		{
			Exec->Cancel(GetOwner());
		}
	}
	ActiveExecutions.Empty();

	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		for (auto& Pair : SkillStates)
		{
			if (Pair.Value.bOnCooldown)
			{
				TimerManager.ClearTimer(Pair.Value.CooldownTimer);
				Pair.Value.bOnCooldown = false;

				OnSkillReady.Broadcast(Pair.Key);
			}
		}
	}
}

void USkillComponent::SetEnergyRegenActive(bool bActive)
{
	if (bActive)
	{
		bEnergyRegenActive = true;
		EnergyRegenAccumulator = 0.0f;
		LastEnergyTime = TimerSubsystem ? TimerSubsystem->GetTotalTimeSeconds() : 0.0f;
	}
	else
	{
		bEnergyRegenActive = false;
	}
}

void USkillComponent::OnTimeChangedCallback(float NewTime)
{
	if (!bEnergyRegenActive)
	{
		return;
	}

	const float DeltaTime = NewTime - LastEnergyTime;
	LastEnergyTime = NewTime;

	if (DeltaTime <= 0.0f)
	{
		return;
	}

	EnergyRegenAccumulator += DeltaTime;
	while (EnergyRegenAccumulator >= 0.1f)
	{
		EnergyRegenAccumulator -= 0.1f;

		if (CurrentEnergy >= MaxEnergy)
		{
			break;
		}

		// 跨领域读取：分数在 PlayerState，能量本地管理
		// 使用 sqrt 缩放应对爆炸式分数增长
		// Cross-domain read: score from PlayerState, energy managed locally
		// sqrt scaling for explosive score growth
		int64 Score = 0;
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGamePlayerState* PS = PC->GetPlayerState<ARunGamePlayerState>())
			{
				Score = PS->GetRunGameScore();
			}
		}

		const float ScoreBonus = FMath::Sqrt(FMath::Max(0.0f, static_cast<float>(Score))) * ScoreRegenMultiplier;

		const float RegenAmount = (BaseEnergyRegenPerSecond + ScoreBonus + EnergyRegenModifier) * EnergyRegenMultiplier * 0.1f;

		AddEnergy(RegenAmount);
	}
}

void USkillComponent::OnRS_CharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	if (NewState == ERunGameCharacterState::Dead)
	{
		ClearAllCooldowns();
		CurrentEnergy = 0.0f;
		OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
	}

	// 重生 → 重置能量 Respawning → reset energy to initial
	if (OldState == ERunGameCharacterState::Dead && NewState != ERunGameCharacterState::Dead)
	{
		CurrentEnergy = InitialEnergy;
		OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
	}
}
