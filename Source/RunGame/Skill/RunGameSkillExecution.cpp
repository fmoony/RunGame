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
	// 先移除速度修改器——触发射 MovementComponent 开始速度插值回落
	// Remove speed modifier first — triggers MovementComponent speed interpolation back to normal
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
		{
			MoveComp->RemoveSpeedModifier(CachedSkillTag);

			// 清除旧定时器，用 SpeedTransitionDuration 重设——等速度插值完成后再关无敌
			// Clear old timer, re-arm with SpeedTransitionDuration — wait for speed interpolation to finish
			if (UWorld* World = Instigator->GetWorld())
			{
				World->GetTimerManager().ClearTimer(RevertTimer);
				const float InterpTime = MoveComp->SpeedTransitionDuration;
				FTimerDelegate RevertDel = FTimerDelegate::CreateUObject(this, &USkillExecution_Unstoppable::RevertInvincibility, Instigator);
				World->GetTimerManager().SetTimer(RevertTimer, RevertDel, InterpTime, false);
			}
		}
	}
}

void USkillExecution_Unstoppable::RevertInvincibility(AActor* Instigator)
{
	// 速度插值已完成——安全关闭无敌 Speed interpolation complete — safe to disable invincibility
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		if (UHealthComponent* HealthComp = RunCharacter->GetHealthComponent())
		{
			HealthComp->SetInvincible(false);
		}
	}
}
