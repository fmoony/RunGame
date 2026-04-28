// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RunGameType.h"
#include "RunGameGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChangedSignature, ERunGameGameState, OldGameState, ERunGameGameState, NewGameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdatedSignature, int32, CountdownSeconds);

/**
 * 全游戏唯一数据中心 (Single Source of Truth)
 * 状态、倒计时的秒数及可配置默认值均由 GameState 集中管理。
 * 游戏内计时为正向累计（从 0 向上递增），由 TimerSubsystem 驱动。
 */
UCLASS()
class RUNGAME_API ARunGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ARunGameGameState();

	// ---- 游戏状态控制 ----
	/** Changes the current game state and broadcasts OnGameStateChanged to all listeners */
	UFUNCTION(BlueprintCallable, Category = "RunGame|State")
	void SetGameState(ERunGameGameState NewState);

	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	ERunGameGameState GetCurrentState() const { return CurrentState; }

	// ---- 倒计时数据 (单一数据源) ----

	/** Sets the countdown seconds value and broadcasts OnCountdownUpdated if changed */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Countdown")
	void SetCountdownSeconds(int32 NewCountdownSeconds);

	UFUNCTION(BlueprintCallable, Category = "RunGame|Countdown")
	int32 GetCountdownSeconds() const { return CountdownSeconds; }

	// ---- 可配置默认值 (供 Subsystem 响应式读取) ----

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Config")
	int32 DefaultCountdownSeconds = 3;

	// 正向计时的时间上限（0.0 表示无上限，仅正向累计）
	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Config")
	float DefaultGameTotalTime = 0.0f;

	// ---- 委托 ----

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Events")
	FOnGameStateChangedSignature OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Events")
	FOnCountdownUpdatedSignature OnCountdownUpdated;

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Death")
	FOnCharacterDeathDelegate OnCharacterDeath;

private:
	UPROPERTY(VisibleAnywhere, Category = "RunGame|State")
	ERunGameGameState CurrentState;

	UPROPERTY(VisibleAnywhere, Category = "RunGame|State")
	int32 CountdownSeconds;
};
