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
 */
UCLASS()
class RUNGAME_API ARunGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARunGamePlayerState();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 外部控制算分启停（供 GameState 状态机调用）
	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void SetScoringActive(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void AddScore(int64 Value);

	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void SetRunGameScore(int64 NewScore);

	UFUNCTION(BlueprintPure, Category = "RunGame|Score")
	int64 GetRunGameScore() const { return RunGameScore; }

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Score")
	FOnScoreChangedDelegate OnScoreChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Score", meta = (AllowPrivateAccess = "true"))
	int64 RunGameScore;

private:
	// 0.1s 循环定时器回调
	UFUNCTION()
	void CalculateScoreProcess();

	// 响应游戏状态变化，启停算分定时器
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	FTimerHandle ScoreTimerHandle;

	UPROPERTY()
	URunGameTimerSubsystem* TimerSubsystem;
};
