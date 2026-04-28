// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Volume/RunGameDeathVolume.h"
#include "RunGameCharacter.h"
#include "RunGameGameMode.h"
#include "Kismet/GameplayStatics.h"

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
	// 通知GameMode玩家死亡
	if(ARunGameGameMode* GameMode = Cast<ARunGameGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		// 通知GameMode处理玩家死亡
		GameMode->HandlePlayerDeath(DeathVolume, PlayerCharacter, bImmediateDeath, DeathDelay);
		bHasTriggeredDeath = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get GameMode in TriggerDeathEvent"));
	}
}

