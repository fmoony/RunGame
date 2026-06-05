// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/RunGameSkillExecution.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameMovementComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "RunGame.h"

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

// ===== Unstoppable =====

void USkillExecution_Unstoppable::SetupEffectTags_Implementation(FGameplayTag InSpeedTag, FGameplayTag InDefenseTag)
{
	SpeedTag = InSpeedTag;
	DefenseTag = InDefenseTag;
}

void USkillExecution_Unstoppable::Reset_Implementation()
{
	UE_LOG(LogRunGame, Warning, TEXT("[Unstoppable] Reset SpeedTag:%s DefenseTag:%s"), *SpeedTag.ToString(), *DefenseTag.ToString());

	// 清除 PRS 中残留的效果标签 Clean up leftover effect tags in PRS
	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			if (SpeedTag.IsValid())   PRS->RemoveEffectTag(SpeedTag);
			if (DefenseTag.IsValid()) PRS->RemoveEffectTag(DefenseTag);
		}
	}

	SpeedTag   = FGameplayTag();
	DefenseTag = FGameplayTag();

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
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:Execute SpeedTag:%s DefenseTag:%s Duration:%.1f"),
		*SpeedTag.ToString(), *DefenseTag.ToString(), Duration);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character) return;

	// 速度修改器 — 用 SpeedTag 做 key，归并到 MovementComponent 的复合乘数 Speed modifier — keyed by SpeedTag
	if (SpeedTag.IsValid())
	{
		if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Character))
		{
			if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
			{
				MoveComp->AddSpeedModifier(SpeedTag, SpeedMultiplier);
			}
		}
	}

	// 发布效果 Tag 到 PRS — 各子系统自行响应 Publish effect tags to PRS — subsystems self-react
	if (UPlayerRuntimeState* PRS = Character->GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		if (SpeedTag.IsValid())   PRS->AddEffectTag(SpeedTag);
		if (DefenseTag.IsValid()) PRS->AddEffectTag(DefenseTag);
	}

	// Duration 定时器 → RevertEffect Duration timer → RevertEffect
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
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:RevertEffect SpeedTag:%s"), *SpeedTag.ToString());

	// 第一步：撤速度 + 撤速度 Tag Step 1: remove speed + speed tag
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
		{
			MoveComp->RemoveSpeedModifier(SpeedTag);

			// 撤速度 Tag — 速度开始插值回落 Remove speed tag — speed starts interpolating back
			if (UPlayerRuntimeState* PRS = Instigator->GetWorld()->GetSubsystem<UPlayerRuntimeState>())
			{
				PRS->RemoveEffectTag(SpeedTag);
			}

			// 第二步：等速度插值完成后撤无敌 Tag Step 2: wait for speed interpolation, then remove defense tag
			if (DefenseTag.IsValid())
			{
				if (UWorld* World = Instigator->GetWorld())
				{
					World->GetTimerManager().ClearTimer(RevertTimer);
					const float Delay = MoveComp->SpeedTransitionDuration * 1.8f;
					FTimerDelegate RevertDel = FTimerDelegate::CreateUObject(this, &USkillExecution_Unstoppable::RevertInvincibility, Instigator);
					World->GetTimerManager().SetTimer(RevertTimer, RevertDel, Delay, false);
					UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:DefenseTag removal delayed %.2fs"), Delay);
				}
			}
		}
	}
}

void USkillExecution_Unstoppable::RevertInvincibility(AActor* Instigator)
{
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:RevertInvincibility DefenseTag:%s"), *DefenseTag.ToString());

	if (DefenseTag.IsValid())
	{
		if (UWorld* World = Instigator ? Instigator->GetWorld() : nullptr)
		{
			if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
			{
				PRS->RemoveEffectTag(DefenseTag);
			}
		}
	}
}

void USkillExecution_Unstoppable::Cancel_Implementation(AActor* Instigator)
{
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:Cancel SpeedTag:%s DefenseTag:%s"), *SpeedTag.ToString(), *DefenseTag.ToString());

	// 立即清所有效果 — 死亡/强制中断不等待速度插值 Clear all effects immediately — death/interrupt don't wait for interpolation
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
		{
			MoveComp->RemoveSpeedModifier(SpeedTag);
		}
	}

	if (UWorld* World = Instigator ? Instigator->GetWorld() : nullptr)
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			if (SpeedTag.IsValid())   PRS->RemoveEffectTag(SpeedTag);
			if (DefenseTag.IsValid()) PRS->RemoveEffectTag(DefenseTag);
		}

		if (RevertTimer.IsValid())
		{
			World->GetTimerManager().ClearTimer(RevertTimer);
		}
		RevertTimer.Invalidate();
	}
}
