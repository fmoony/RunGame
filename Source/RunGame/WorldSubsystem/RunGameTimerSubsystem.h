// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameType.h"
#include "RunGameTimerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, float, NewTime);

/**
 * 纯计时子系统 —— Tick 驱动的倒计时引擎和正向计时器。
 * 直接持有计时数据，绑定 GameState 委托响应状态变化。
 * Pure timer subsystem — Tick-driven countdown engine and forward timer.
 * Holds timer data directly, binds GameState delegates reactively.
 */
UCLASS()
class RUNGAME_API URunGameTimerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	URunGameTimerSubsystem();

	virtual TStatId GetStatId() const override;

	/** 绑定 GameState 的 OnGameStateChanged 以响应状态变化 Bind to GameState's OnGameStateChanged for reactive timing */
	virtual void OnWorldBeginPlay(UWorld& World) override;

	UFUNCTION(BlueprintPure, Category = "RunGame|Timer")
	float GetTotalTimeSeconds() const { return TotalTimeSeconds; }

	UFUNCTION(BlueprintPure, Category = "RunGame|Timer")
	bool IsTimerRunning() const { return bIsTimerRunning; }

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Timer")
	FOnCountdownCompleteDelegate OnCountdownComplete;

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Timer")
	FOnTimeChangedDelegate OnTimeChanged;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickableWhenPaused() const override { return true; }

private:
	/** 响应 GameState 的游戏状态变化，启动/停止计时器 React to GameState changes: start/stop timers */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	void StartCountdown();
	void StopCountdown();
	void UpdateCountdown();
	void FinishCountdown();
	void StartTimer();
	void StopTimer();
	void UpdateTimer(float DeltaTime);

	float TotalTimeSeconds = 0.0f;
	bool bIsTimerRunning = false;
	bool bIsCountdownActive = false;
	float CountdownTickAccumulator = 0.0f;
	bool bResumingFromPause = false;
};
