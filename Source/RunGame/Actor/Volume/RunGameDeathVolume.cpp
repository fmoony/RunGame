// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Volume/RunGameDeathVolume.h"
#include "RunGameCharacter.h"
#include "GameplayTagContainer.h"

ARunGameDeathVolume::ARunGameDeathVolume()
{
	bImmediateDeath = true;
	DeathDelay = 0.0f;
	bHasTriggeredDeath = false;
}

void ARunGameDeathVolume::OnPlayerEnter_Implementation(ARunGameCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || bHasTriggeredDeath)
	{
		return;
	}

	bHasTriggeredDeath = true;

	UE_LOG(LogTemp, Error, TEXT("Player entered DeathVolume: %s"), *GetName());
	// 触发死亡事件
	TriggerDeathEvent(this, PlayerCharacter);
}

void ARunGameDeathVolume::TriggerDeathEvent(ARunGameDeathVolume* DeathVolume, ARunGameCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// 直接调用 Character 的 Die，死亡逻辑完全由 Character 自行处理
	PlayerCharacter->Die(FGameplayTag(), DeathDelay);
	bHasTriggeredDeath = false;
}
