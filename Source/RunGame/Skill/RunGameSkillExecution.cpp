// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/RunGameSkillExecution.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameMovementComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "RunGame.h"

PRAGMA_DISABLE_OPTIMIZATION

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
	UE_LOG(LogRunGame, Warning, TEXT("[Unstoppable] Reset"));
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
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:Execute Tag:%s, Duration:%.1f"), *SkillTag.ToString(), Duration);
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
	}

	// 通过 PRS Tag 总线通知无敌 Notify invincibility via PRS tag bus
	if (UPlayerRuntimeState* PRS = Character->GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->AddEffectTag(SkillTag);
		UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:AddEffectTag Tag:%s"), *SkillTag.ToString());
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("USkillExecution_Unstoppable:Failed to get PRS"));
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
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:RevertEffect (timer expired) Tag:%s"), *CachedSkillTag.ToString());

	// 先移除速度——触发插值回落 Remove speed first — triggers interpolation
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
		{
			MoveComp->RemoveSpeedModifier(CachedSkillTag);

			// 等速度回落后再撤 Tag Delay tag removal until speed interpolates back
			if (UWorld* World = Instigator->GetWorld())
			{
				World->GetTimerManager().ClearTimer(RevertTimer);
				const float Delay = MoveComp->SpeedTransitionDuration * 1.8f;
				FTimerDelegate RevertDel = FTimerDelegate::CreateUObject(this, &USkillExecution_Unstoppable::RevertInvincibility, Instigator);
				World->GetTimerManager().SetTimer(RevertTimer, RevertDel, Delay, false);
				UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:Timer Delay:%f"), Delay);
			}
		}
	}
}

void USkillExecution_Unstoppable::Cancel_Implementation(AActor* Instigator)
{
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:Cancel Tag:%s"), *CachedSkillTag.ToString());
	// 移除速度修改器——触发速度插值回落
	// Remove speed modifier — triggers speed interpolation back to normal
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		if (URunGameMovementComponent* MoveComp = RunCharacter->GetRunGameMovementComponent())
		{
			MoveComp->RemoveSpeedModifier(CachedSkillTag);

			// 用实际 SpeedTransitionDuration 重设定时器——运行时值，蓝图覆写自动生效
			// Use actual SpeedTransitionDuration — runtime value, Blueprint overrides are respected
			if (UWorld* World = Instigator->GetWorld())
			{
				World->GetTimerManager().ClearTimer(RevertTimer);
				const float Delay = MoveComp->SpeedTransitionDuration * 1.8f;
				FTimerDelegate RevertDel = FTimerDelegate::CreateUObject(this, &USkillExecution_Unstoppable::RevertInvincibility, Instigator);
				World->GetTimerManager().SetTimer(RevertTimer, RevertDel, Delay, false);
				UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:Timer Delay:%f"), Delay);
			}
			else
			{
				UE_LOG(LogRunGame, Error, TEXT("USkillExecution_Unstoppable:Fail to GetWorld"));
			}
		}
	}
}

void USkillExecution_Unstoppable::RevertInvincibility(AActor* Instigator)
{
	UE_LOG(LogRunGame, Warning, TEXT("USkillExecution_Unstoppable:RevertInvincibility Tag:%s"), *CachedSkillTag.ToString());
	// 速度插值已完成——通过 Tag 总线关闭无敌 Speed interpolation complete — disable invincibility via tag bus
	if (UWorld* World = Instigator ? Instigator->GetWorld() : nullptr)
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->RemoveEffectTag(CachedSkillTag);
		}
	}
}

PRAGMA_ENABLE_OPTIMIZATION
