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

	/** Enables or disables the score calculation timer */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void SetScoringActive(bool bActive);

	/** Adds the given value to the player's score and broadcasts the change */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Score")
	void AddScore(int64 Value);

	/** Sets the player's score to an absolute value and broadcasts if changed */
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
	/** Periodic score calculation callback -- computes cubic polynomial multiplier */
	UFUNCTION()
	void CalculateScoreProcess();

	/** Reactively starts or stops score calculation based on game state */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	FTimerHandle ScoreTimerHandle;

	UPROPERTY()
	URunGameTimerSubsystem* TimerSubsystem;
};
