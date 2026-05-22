// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/RunGameSkillExecution.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

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
