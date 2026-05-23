// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameType.h"
#include "RunGameTimerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, float, NewTime);

/**
 * 纯粹的计时子系统（引擎）。
 * 数据存储已迁移到 UGameFlowRuntimeState，本类仅保留 Tick 逻辑和倒计时引擎。
 * 委托由 RS 转发器重播以保持向后兼容。
 */
UCLASS()
class RUNGAME_API URunGameTimerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	URunGameTimerSubsystem();

	virtual TStatId GetStatId() const override;

	/** 绑定到 GameFlowRuntimeState 的 OnGameStateChanged 和转发器 */
	virtual void OnWorldBeginPlay(UWorld& World) override;

	UFUNCTION(BlueprintPure, Category = "RunGame|Timer")
	float GetTotalTimeSeconds() const;

	UFUNCTION(BlueprintPure, Category = "RunGame|Timer")
	bool IsTimerRunning() const;

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
	/** 响应 RS 的游戏状态变化，启动/停止计时器 */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	// ---- RS 转发器 ----
	UFUNCTION()
	void OnRS_TimeChanged(float NewTime);

	UFUNCTION()
	void OnRS_CountdownComplete();

	void StartCountdown();
	void StopCountdown();
	void UpdateCountdown();
	void FinishCountdown();
	void StartTimer();
	void StopTimer();
	void UpdateTimer(float DeltaTime);

	bool bIsCountdownActive = false;
	float CountdownTickAccumulator = 0.0f;
	bool bResumingFromPause = false;
};
