// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RunGameType.h"
#include "RunGamePlayerState.generated.h"

class URunGameTimerSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedDelegate, int64, NewScore);

/**
 * 分数由 0.1s 后台定时器驱动，杜绝 Tick 开销。
 * 倍率使用三次多项式 (Cubic Polynomial) 平滑爆发，避免指数溢出。
 * 分数存储和运算全部使用 int64，安全承载游戏后期极高分数。
 * Score driven by 0.1s background timer. Cubic polynomial multiplier for smooth growth. All int64.
 */
UCLASS()
class RUNGAME_API ARunGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARunGamePlayerState();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** 启用/禁用分数计算定时器 Enables or disables the score calculation timer */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void SetScoringActive(bool bActive);

	/** 累加分数并广播变化 Adds the given value to the player's score and broadcasts the change */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void AddScore(int64 Value);

	/** 设置绝对分数值，有变化时广播 Sets the player's score to an absolute value and broadcasts if changed */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void SetRunGameScore(int64 NewScore);

	UFUNCTION(BlueprintPure, Category = "RunGame|Score")
	int64 GetRunGameScore() const { return RunGameScore; }

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Score")
	FOnScoreChangedDelegate OnScoreChanged;

private:
	UFUNCTION()
	void CalculateScoreProcess();

	/** 由 TimerSubsystem::OnTimeChanged 驱动——替代独立 0.1s 定时器 Driven by TimerSubsystem::OnTimeChanged — replaces standalone 0.1s timer */
	UFUNCTION()
	void OnTimeChangedCallback(float NewTime);

	/** 响应 GameState 状态变化：清零分数、启停计分 React to GameState changes: reset score, toggle scoring */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	UPROPERTY()
	URunGameTimerSubsystem* TimerSubsystem;

	int64 RunGameScore = 0;
	bool bScoringActive = false;
	float ScoreTickAccumulator = 0.0f;
	float LastScoreTime = 0.0f;
};
