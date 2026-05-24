#include "Actor/Component/HealthComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
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

	// 监听角色状态机，死亡时自行清除无敌 Listen to character state: clear invincibility on death
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddDynamic(this, &UHealthComponent::OnRS_CharacterStateChanged);
	}

	// 广播初始生命值 Broadcast initial health
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->OnCharacterStateChanged.RemoveDynamic(this, &UHealthComponent::OnRS_CharacterStateChanged);
		}
	}

	Super::EndPlay(EndPlayReason);
}

// ---- Health API ----

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

void UHealthComponent::ForceKill(FGameplayTag DamageType, AActor* Killer)
{
	if (bIsDead)
	{
		return;
	}

	bIsInvincible = false;
	CurrentHP = 0.0f;
	bIsDead = true;

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
	bIsDead = false;
	CurrentHP = FMath::Clamp(RestoreHP, 1.0f, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP, MaxHP, CurrentHP);
}

void UHealthComponent::OnRS_CharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	if (NewState == ERunGameCharacterState::Dead)
	{
		SetInvincible(false);
	}
}
