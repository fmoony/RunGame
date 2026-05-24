// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Volume/RunGameDeathVolume.h"
#include "RunGameCharacter.h"
#include "Actor/Component/HealthComponent.h"
#include "Interfaces/Damagable.h"
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

	// 死亡体积强制击杀：无视无敌，直接置零 HP 并广播死亡
	// DeathVolume force kill: bypass invincibility, directly zero HP and broadcast death
	if (UHealthComponent* HC = PlayerCharacter->GetHealthComponent())
	{
		HC->ForceKill(FGameplayTag(), DeathVolume);
	}
	bHasTriggeredDeath = false;
}
