#include "WorldSubsystem/State/CombatRuntimeState.h"
#include "RunGame.h"

void UCombatRuntimeState::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCombatRuntimeState::Deinitialize()
{
	Super::Deinitialize();
}

// ---- Health ----

void UCombatRuntimeState::ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (bIsDead || Damage <= 0.0f || bIsInvincible)
	{
		return;
	}

	const float OldHP = CurrentHP;
	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.0f, MaxHP);
	const float Delta = CurrentHP - OldHP;

	if (Delta != 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHP, MaxHP, Delta);
	}

	if (CurrentHP <= 0.0f)
	{
		bIsDead = true;
		OnDeath.Broadcast(DamageType, DamageCauser);
	}
	else
	{
		OnDamageTaken.Broadcast(Damage, DamageType, DamageCauser);
	}
}

void UCombatRuntimeState::Heal(float Amount, AActor* Healer)
{
	if (bIsDead || Amount <= 0.0f)
	{
		return;
	}

	const float OldHP = CurrentHP;
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, MaxHP);
	const float Delta = CurrentHP - OldHP;

	if (Delta != 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHP, MaxHP, Delta);
	}
}

void UCombatRuntimeState::Revive(float RestoreHP)
{
	bIsDead = false;
	CurrentHP = FMath::Clamp(RestoreHP, 1.0f, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}

void UCombatRuntimeState::SetInvincible(bool bNewInvincible)
{
	if (bIsInvincible != bNewInvincible)
	{
		bIsInvincible = bNewInvincible;
		OnInvincibilityChanged.Broadcast(bIsInvincible);
	}
}

float UCombatRuntimeState::GetHealthPercentage() const
{
	return MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f;
}

// ---- Skill / Energy ----

void UCombatRuntimeState::SetEnergy(float NewValue)
{
	NewValue = FMath::Clamp(NewValue, 0.0f, MaxEnergy);
	if (CurrentEnergy != NewValue)
	{
		CurrentEnergy = NewValue;
		OnEnergyChanged.Broadcast(CurrentEnergy, MaxEnergy);
	}
}

void UCombatRuntimeState::AddEnergy(float Amount)
{
	SetEnergy(CurrentEnergy + Amount);
}

void UCombatRuntimeState::BroadcastSkillActivated(FGameplayTag SkillTag, float CooldownDuration)
{
	OnSkillActivated.Broadcast(SkillTag, CooldownDuration);
}

void UCombatRuntimeState::BroadcastSkillReady(FGameplayTag SkillTag)
{
	OnSkillReady.Broadcast(SkillTag);
}

void UCombatRuntimeState::BroadcastSkillExecuted(FGameplayTag SkillTag)
{
	OnSkillExecuted.Broadcast(SkillTag);
}
