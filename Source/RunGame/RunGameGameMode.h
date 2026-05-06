// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunGameType.h"
#include "DataAssets/FloorConfigData.h"
#include "RunGameGameMode.generated.h"

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

	// 地板配置数据资产（数据驱动的生成配置）
	UPROPERTY(EditDefaultsOnly, Category = "RunGame|FloorSystem", meta = (DisplayName = "Floor Config"))
	TObjectPtr<UFloorConfigData> FloorConfig;

	// 游戏初始时间设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RunGame|Game")
	float GameTotalTime;

	// 玩家死亡延迟处理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RunGame|Death")
	float PlayerDeathDelay;

	/** Constructor */
	ARunGameGameMode();

	/** Initiates the game countdown sequence and sets game state to CountDown */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Flow")
	void StartGameCountDown(int32 CountdownSeconds = 3);

	/** Spawns and possesses the player character at the chosen player start */
	UFUNCTION()
	void SpawnPlayer();

	/** Deprecated -- timer now starts reactively via GameState state changes */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Game")
	void InitializeGameTimer();

	/** Fully resets the game to initial state and returns to main menu */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Game")
	void ResetGame();

	/** Restarts the game directly, bypassing the main menu */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Game")
	void StartNewGame();

	/* Request from Pause UI */
	void RequestRestartFromPause();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Callback when floor subsystem finishes async loading -- spawns initial floors */
	UFUNCTION()
	void OnFloorSystemReadyCallback();

	/** Reacts to game state changes -- spawns player when entering InGame */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

private:
	bool bResumingFromPause = false;
};
