// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/RunGameSkillExecution.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "RunGameCharacter.h"
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
		RunCharacter->AddSpeedModifier(CachedSkillTag, SpeedMultiplier);

		if (UHealthComponent* HealthComp = RunCharacter->GetHealthComponent())
		{
			HealthComp->SetInvincible(true);
		}
	}

	// Schedule revert after Duration
	if (UWorld* World = Instigator->GetWorld())
	{
		FTimerDelegate RevertDel = FTimerDelegate::CreateUObject(this, &USkillExecution_Unstoppable::RevertEffect, Instigator);
		World->GetTimerManager().SetTimer(RevertTimer, RevertDel, Duration, false);
	}
}

void USkillExecution_Unstoppable::RevertEffect(AActor* Instigator)
{
	if (ARunGameCharacter* RunCharacter = Cast<ARunGameCharacter>(Instigator))
	{
		RunCharacter->RemoveSpeedModifier(CachedSkillTag);

		if (UHealthComponent* HealthComp = RunCharacter->GetHealthComponent())
		{
			HealthComp->SetInvincible(false);
		}
	}
}
