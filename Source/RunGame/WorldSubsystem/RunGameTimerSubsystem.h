// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameType.h"
#include "Tickable.h"
#include "RunGameTimerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, float, NewTime);

class ARunGameGameState;

/**
 * 纯粹的计时子系统
 * 倒计时：从 DefaultCountdownSeconds 倒数至 0，每秒 Tick 一次。
 * 正向计时：从 0.0 向上累计，每帧 Tick。
 * 不持有配置数据或分数逻辑，启停完全由 GameState 状态机驱动。
 */
UCLASS()
class RUNGAME_API URunGameTimerSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	URunGameTimerSubsystem();

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
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;

private:
	// 响应式入口：由 GameState::OnGameStateChanged 触发
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	// 内部倒计时辅助函数
	void StartCountdown();
	void StopCountdown();
	void UpdateCountdown();
	void FinishCountdown();

	// 内部计时辅助函数
	void StartTimer();
	void StopTimer();
	void UpdateTimer(float DeltaTime);

	ARunGameGameState* GetGameState() const;

	FTimerHandle CountdownTimerHandle;

	// 正向计时累计秒数（从 0.0 开始，每帧累加 DeltaTime）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Timer", meta = (AllowPrivateAccess = "true"))
	float TotalTimeSeconds;

	bool bIsTimerRunning;
};
