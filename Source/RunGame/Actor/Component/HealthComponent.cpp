#include "Actor/Component/HealthComponent.h"
#include "Engine/World.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// 初始化生命值为满血 Initialize HP to full
	CurrentHP = MaxHP;

	// 广播初始生命值 Broadcast initial health
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// ---- Health API ----

void UHealthComponent::ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (CurrentHP <= 0.0f || Damage <= 0.0f || bIsInvincible)
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
		OnDeath.Broadcast(DamageType, DamageCauser);
	}
	else
	{
		OnDamageTaken.Broadcast(Damage, DamageType, DamageCauser);
	}
}

void UHealthComponent::Heal(float Amount, AActor* Healer)
{
	if (CurrentHP <= 0.0f || Amount <= 0.0f)
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

void UHealthComponent::ForceKill(FGameplayTag DamageType, AActor* Killer)
{
	if (CurrentHP <= 0.0f)
	{
		return;
	}

	bIsInvincible = false;
	CurrentHP = 0.0f;

	OnHealthChanged.Broadcast(CurrentHP, MaxHP, -MaxHP);
	OnDeath.Broadcast(DamageType, Killer);
}

float UHealthComponent::GetHealthPercentage() const
{
	return MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f;
}

void UHealthComponent::SetInvincible(bool bNewInvincible)
{
	if (bIsInvincible != bNewInvincible)
	{
		bIsInvincible = bNewInvincible;
		OnInvincibilityChanged.Broadcast(bIsInvincible);
	}
}

void UHealthComponent::Revive(float RestoreHP)
{
	CurrentHP = FMath::Clamp(RestoreHP, 1.0f, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}
