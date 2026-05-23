// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Component/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	bIsDead = false;
}

void UHealthComponent::ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
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

void UHealthComponent::Heal(float Amount, AActor* Healer)
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
	bIsDead = false;
	CurrentHP = FMath::Clamp(RestoreHP, 1.0f, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}
