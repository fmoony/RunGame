// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunGameType.h"
#include "Actor/Volume/RunGameDeathVolume.h"
#include "RunGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeathDelegate, ARunGameCharacter*, PlayerCharacter);

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract) 
class ARunGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
		UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<APawn> GameCharacterClass;

	// 地板蓝图类配置（用于初始化 FloorSubsystem）
	UPROPERTY(EditDefaultsOnly, Category = "RunGame|FloorSystem", meta = (DisplayName = "Straight Floor Classes"))
	TArray<TSoftClassPtr<AActor>> StraightFloorClasses;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|FloorSystem", meta = (DisplayName = "Turn Floor Classes"))
	TArray<TSoftClassPtr<AActor>> TurnFloorClasses;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|FloorSystem", meta = (DisplayName = "Pre-Allocate Count"))
	int32 PreAllocateFloorCount = 10;

	// 游戏初始时间设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RunGame|Game")
	float GameTotalTime;

	// 玩家死亡相关
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Death")
	FOnPlayerDeathDelegate OnPlayerDeath;

	// 玩家死亡延迟处理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RunGame|Death")
	float PlayerDeathDelay;

	/** Constructor */
	ARunGameGameMode();

	UFUNCTION(BlueprintCallable, Category = "RunGame|Flow")
	void StartGameCountDown(int32 CountdownSeconds = 3);

	// 生成玩家
	UFUNCTION()
	void SpawnPlayer();

	// 游戏计时相关函数
	UFUNCTION(BlueprintCallable, Category = "RunGame|Game")
	void InitializeGameTimer();

	// 完全重置游戏到初始状态（回到主菜单）
	UFUNCTION(BlueprintCallable, Category = "RunGame|Game")
	void ResetGame();

	// 重新开始游戏（绕过主菜单，直接进入倒计时）
	UFUNCTION(BlueprintCallable, Category = "RunGame|Game")
	void StartNewGame();

	// 玩家死亡处理函数 - 游戏状态转换和角色销毁
	//（鼠标/输入模式由 Character::Die() 自行处理）
	UFUNCTION(BlueprintCallable, Category = "RunGame|Death")
	void HandlePlayerDeath(ARunGameDeathVolume* DeathVolume, ARunGameCharacter* PlayerCharacter, bool bImmediate, float Delay);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnFloorSystemReadyCallback();
};


