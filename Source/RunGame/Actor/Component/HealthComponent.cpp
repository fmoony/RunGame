#include "Actor/Component/HealthComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/World.h"
#include "RunGame.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	// 缓存 PRS + 监听效果标签变化 Cache PRS + listen to effect tag changes
	CachedPRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.AddDynamic(this, &UHealthComponent::OnEffectTagChanged);
	}

	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.RemoveDynamic(this, &UHealthComponent::OnEffectTagChanged);
		CachedPRS = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// ---- Health API ----

void UHealthComponent::ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (CurrentHP <= 0.0f || Damage <= 0.0f || IsInvincible())
	{
		return;
	}

	// 护盾先吸收 — 破盾时广播 Shield absorbs first — broadcast on break
	if (ShieldHP > 0.0f)
	{
		if (Damage <= ShieldHP)
		{
			ShieldHP -= Damage;
			OnShieldChanged.Broadcast(ShieldHP);
			return;
		}

		Damage -= ShieldHP;
		ShieldHP = 0.0f;
		OnShieldChanged.Broadcast(0.0f);
		OnShieldBroken.Broadcast();
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

	CurrentHP = 0.0f;
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, -MaxHP);
	OnDeath.Broadcast(DamageType, Killer);
}

float UHealthComponent::GetHealthPercentage() const
{
	return MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f;
}

bool UHealthComponent::IsInvincible() const
{
	return InvincibilityTagCount > 0;
}

bool UHealthComponent::IsTagRelevantToQuery(FGameplayTag Tag, const FGameplayTagQuery& Query) const
{
	if (Query.IsEmpty()) return false;

	FGameplayTagContainer SingleTagContainer;
	SingleTagContainer.AddTag(Tag);
	return SingleTagContainer.MatchesQuery(Query);
}

void UHealthComponent::OnEffectTagChanged(FGameplayTag Tag, bool bAdded)
{
	// ---- 无敌判定 Invincibility check ----
	if (IsTagRelevantToQuery(Tag, InvincibilityTagQuery))
	{
		const int32 OldCount = InvincibilityTagCount;
		InvincibilityTagCount = bAdded ? InvincibilityTagCount + 1 : FMath::Max(InvincibilityTagCount - 1, 0);

		if ((OldCount == 0 && InvincibilityTagCount > 0) || (OldCount > 0 && InvincibilityTagCount == 0))
		{
			OnInvincibilityChanged.Broadcast(InvincibilityTagCount > 0);
		}
	}

	// ---- 护盾判定 Shield check ----
	if (IsTagRelevantToQuery(Tag, ShieldTagQuery))
	{
		if (bAdded)
		{
			ShieldTagCount++;
			if (ShieldTagCount == 1)
			{
				AddShield(DefaultShieldAmount);
			}
		}
		else
		{
			ShieldTagCount = FMath::Max(ShieldTagCount - 1, 0);
			if (ShieldTagCount == 0)
			{
				RemoveShield();
			}
		}
	}
}

void UHealthComponent::Revive(float RestoreHP)
{
	CurrentHP = FMath::Clamp(RestoreHP, 1.0f, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}

// ---- Shield ----

void UHealthComponent::AddShield(float Amount)
{
	ShieldHP += Amount;
	OnShieldChanged.Broadcast(ShieldHP);
}

void UHealthComponent::RemoveShield()
{
	const bool bHadShield = ShieldHP > 0.0f;
	ShieldHP = 0.0f;
	if (bHadShield)
	{
		OnShieldChanged.Broadcast(0.0f);
		OnShieldBroken.Broadcast();
	}
}
