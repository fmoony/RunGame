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

	// 死亡体积强制击杀：无视无敌，走伤害接口统一链路
	if (UHealthComponent* HC = PlayerCharacter->GetHealthComponent())
	{
		HC->SetInvincible(false);
	}
	IDamagable::Execute_OnTakeDamage(PlayerCharacter, 99999.0f, FGameplayTag(), DeathVolume);
	bHasTriggeredDeath = false;
}
