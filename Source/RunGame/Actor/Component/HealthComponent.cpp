#include "Actor/Component/HealthComponent.h"
#include "WorldSubsystem/State/CombatRuntimeState.h"
#include "Engine/World.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->SetMaxHP(MaxHP);
		RS->Revive(MaxHP);

		RS->OnHealthChanged.AddDynamic(this, &UHealthComponent::OnRS_HealthChanged);
		RS->OnDamageTaken.AddDynamic(this, &UHealthComponent::OnRS_DamageTaken);
		RS->OnDeath.AddDynamic(this, &UHealthComponent::OnRS_Death);
		RS->OnInvincibilityChanged.AddDynamic(this, &UHealthComponent::OnRS_InvincibilityChanged);
	}
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->OnHealthChanged.RemoveDynamic(this, &UHealthComponent::OnRS_HealthChanged);
		RS->OnDamageTaken.RemoveDynamic(this, &UHealthComponent::OnRS_DamageTaken);
		RS->OnDeath.RemoveDynamic(this, &UHealthComponent::OnRS_Death);
		RS->OnInvincibilityChanged.RemoveDynamic(this, &UHealthComponent::OnRS_InvincibilityChanged);
	}

	Super::EndPlay(EndPlayReason);
}

// ---- RS 转发器 ----

void UHealthComponent::OnRS_HealthChanged(float CurrentHP, float InMaxHP, float InDelta)
{
	OnHealthChanged.Broadcast(CurrentHP, InMaxHP, InDelta);
}

void UHealthComponent::OnRS_DamageTaken(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	OnDamageTaken.Broadcast(Damage, DamageType, DamageCauser);
}

void UHealthComponent::OnRS_Death(FGameplayTag DamageType, AActor* DeathCauser)
{
	OnDeath.Broadcast(DamageType, DeathCauser);
}

void UHealthComponent::OnRS_InvincibilityChanged(bool bNewInvincible)
{
	OnInvincibilityChanged.Broadcast(bNewInvincible);
}

// ---- Health API ----

void UHealthComponent::ApplyDamage(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->ApplyDamage(Damage, DamageType, DamageCauser);
	}
}

void UHealthComponent::Heal(float Amount, AActor* Healer)
{
	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->Heal(Amount, Healer);
	}
}

float UHealthComponent::GetCurrentHP() const
{
	if (const UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		return RS->GetCurrentHP();
	}
	return 0.0f;
}

float UHealthComponent::GetHealthPercentage() const
{
	if (const UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		return RS->GetHealthPercentage();
	}
	return 0.0f;
}

bool UHealthComponent::IsDead() const
{
	if (const UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		return RS->IsDead();
	}
	return false;
}

void UHealthComponent::SetInvincible(bool bNewInvincible)
{
	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->SetInvincible(bNewInvincible);
	}
}

bool UHealthComponent::IsInvincible() const
{
	if (const UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		return RS->IsInvincible();
	}
	return false;
}

void UHealthComponent::Revive(float RestoreHP)
{
	if (UCombatRuntimeState* RS = GetWorld()->GetSubsystem<UCombatRuntimeState>())
	{
		RS->Revive(RestoreHP);
	}
}
