// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RunGameInteractiveVolume.h"
#include "RunGameDeathVolume.generated.h"

/**
 * 死亡体积 - 当玩家进入时触发游戏结束
 * 用于检测玩家掉落到地图下方
 */
UCLASS()
class RUNGAME_API ARunGameDeathVolume : public ARunGameInteractiveVolume
{
	GENERATED_BODY()

public:
	/** Constructs the death volume with default immediate-death settings */
	ARunGameDeathVolume();

	/** Triggers death when a player enters the volume, with duplicate-prevention */
	virtual void OnPlayerEnter_Implementation(ARunGameCharacter* PlayerCharacter) override;

	// 蓝图可配置属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RunGame|Death")
	bool bImmediateDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RunGame|Death")
	float DeathDelay;

	/** Notifies the game mode to handle player death with configured delay settings */
	void TriggerDeathEvent(ARunGameDeathVolume* DeathVolume, ARunGameCharacter* PlayerCharacter);

private:
	// 防止重复触发的标志
	bool bHasTriggeredDeath;
};