// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/RunGameSkillExecution.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameMovementComponent.h"
#include "Actor/Component/HealthComponent.h"

void USkillExecution_PlayMontageAndImpulse::Execute_Implementation(AActor* Instigator, FGameplayTag SkillTag)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character)
	{
		return;
	}

	if (Montage)
	{
		Character->PlayAnimMontage(Montage);
	}

	if (ImpulseStrength > 0.0f)
	{
		if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
		{
			MoveComp->AddImpulse(Character->GetActorForwardVector() * ImpulseStrength, true);
		}
	}
}

void USkillExecution_Unstoppable::Reset_Implementation()
{
	CachedSkillTag = FGameplayTag();
	if (RevertTimer.IsValid())
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(RevertTimer);
		}
		RevertTimer.Invalidate();
	}
}

void USkillExecution_Unstoppable::Execute_Implementation(AActor* Instigator, FGameplayTag SkillTag)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character)
	{
		return;
	}

	// Speed boost via character's modifier map — persists across Tick curve overwrites
	CachedSkillTag = SkillTag;
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Character))
	{
		if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
			{
				MoveComp->AddSpeedModifier(CachedSkillTag, SpeedMultiplier);
			}

		if (UHealthComponent* HealthComp = RunCharacter->GetHealthComponent())
		{
			HealthComp->SetInvincible(true);
		}
	}

	// Schedule revert after Duration——先清旧定时器防止重复 Execute 泄漏
	// Clear old timer first to prevent leak on double-execute
	if (UWorld* World = Instigator->GetWorld())
	{
		if (RevertTimer.IsValid())
		{
			World->GetTimerManager().ClearTimer(RevertTimer);
		}
		FTimerDelegate RevertDel = FTimerDelegate::CreateUObject(this, &USkillExecution_Unstoppable::RevertEffect, Instigator);
		World->GetTimerManager().SetTimer(RevertTimer, RevertDel, Duration, false);
	}
}

void USkillExecution_Unstoppable::RevertEffect(AActor* Instigator)
{
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
			{
				MoveComp->RemoveSpeedModifier(CachedSkillTag);
			}

		if (UHealthComponent* HealthComp = RunCharacter->GetHealthComponent())
		{
			HealthComp->SetInvincible(false);
		}
	}
}

void USkillExecution_Unstoppable::Cancel_Implementation(AActor* Instigator)
{
	if (UWorld* World = Instigator ? Instigator->GetWorld() : nullptr)
	{
		World->GetTimerManager().ClearTimer(RevertTimer);
	}

	if (Instigator && Instigator->IsValidLowLevel())
	{
		RevertEffect(Instigator);
	}
}
