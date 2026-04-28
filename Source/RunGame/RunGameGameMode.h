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
	int32 PreAllocateFloorCount = 20;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|FloorSystem", meta = (DisplayName = "Initial Straight Count"))
	int32 InitialStraightFloorCount = 5;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|FloorSystem", meta = (DisplayName = "Initial Random Count"))
	int32 InitialRandomFloorCount = 15;

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

	/** Processes player death -- broadcasts event, sets GameOver state, handles character destruction */
	// Input mode and view target handled by Character::Die()
	UFUNCTION(BlueprintCallable, Category = "RunGame|Death")
	void HandlePlayerDeath(
			ARunGameDeathVolume* DeathVolume,
			ARunGameCharacter* PlayerCharacter,
			bool bImmediate,
			float Delay);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Callback when floor subsystem finishes async loading -- spawns initial floors */
	UFUNCTION()
	void OnFloorSystemReadyCallback();

	/** Reacts to game state changes -- spawns player when entering InGame */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);
};


