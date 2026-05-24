// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RunGameType.h"
#include "RunGameGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChangedSignature, ERunGameGameState, OldGameState, ERunGameGameState, NewGameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdatedSignature, int32, CountdownSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownCompleteSignature);

/**
 * 游戏状态权威数据源 —— 直接持有状态数据和委托，不再经过 RuntimeState 中转
 * Authoritative source for game state data — holds state and delegates directly
 */
UCLASS()
class RUNGAME_API ARunGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ARunGameGameState();

	// ---- 游戏状态控制 Game State Control ----

	UFUNCTION(BlueprintCallable, Category = "RunGame|State")
	void SetGameState(ERunGameGameState NewState);

	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	ERunGameGameState GetCurrentState() const { return CurrentGameState; }

	// ---- 倒计时 Countdown ----

	UFUNCTION(BlueprintCallable, Category = "RunGame|Countdown")
	void SetCountdownSeconds(int32 NewCountdownSeconds);

	UFUNCTION(BlueprintCallable, Category = "RunGame|Countdown")
	int32 GetCountdownSeconds() const { return CountdownSeconds; }

	// ---- 可配置默认值 Configurable Defaults ----

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Config")
	int32 DefaultCountdownSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Config")
	float DefaultGameTotalTime = 0.0f;

	// ---- 委托 Delegates ----

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Events")
	FOnGameStateChangedSignature OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Events")
	FOnCountdownUpdatedSignature OnCountdownUpdated;

	/** 倒计时完成时广播（由 TimerSubsystem 触发） Broadcast when countdown completes (triggered by TimerSubsystem) */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Events")
	FOnCountdownCompleteSignature OnCountdownComplete;

private:
	ERunGameGameState CurrentGameState = ERunGameGameState::MainMenu;
	int32 CountdownSeconds = 0;
};
