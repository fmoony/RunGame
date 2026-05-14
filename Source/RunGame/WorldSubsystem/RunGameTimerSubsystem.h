// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameType.h"
#include "RunGameTimerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, float, NewTime);

class ARunGameGameState;

/**
 * 纯粹的计时子系统
 * 倒计时：从 DefaultCountdownSeconds 倒数至 0，每秒 Tick 一次。
 * 正向计时：从 0.0 向上累计，每帧 Tick。
 * 不持有配置数据或分数逻辑，启停完全由 GameState 状态机驱动。
 * 世界暂停时仍可正常计时。
 */
UCLASS()
class RUNGAME_API URunGameTimerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Constructs the timer subsystem with zeroed time and stopped state */
	URunGameTimerSubsystem();

	virtual TStatId GetStatId() const override;

	/** Binds to the game state's state-change event on world begin play */
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

	/** Stops all timers and unbinds from game state events on teardown */
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	/** Allow ticking when the world is paused */
	virtual bool IsTickableWhenPaused() const override { return true; }

private:
	/** Reactively starts/stops timers based on the new game state */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	/** Starts the 1Hz countdown timer using the configured duration */
	void StartCountdown();

	/** Stops and clears the countdown timer handle */
	void StopCountdown();

	/** Decrements countdown each tick; finishes at zero */
	void UpdateCountdown();

	/** Stops countdown, transitions to InGame, broadcasts completion */
	void FinishCountdown();

	/** Resets forward timer to zero and begins accumulating */
	void StartTimer();

	/** Pauses forward timer accumulation */
	void StopTimer();

	/** Accumulates delta time and broadcasts updated value */
	void UpdateTimer(float DeltaTime);

	/** Returns the RunGame game state cast from the world's game state */
	ARunGameGameState* GetGameState() const;

	/** Accumulated forward time in seconds, incremented each tick */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Timer", meta = (AllowPrivateAccess = "true"))
	float TotalTimeSeconds;

	/** Whether the 1Hz countdown is currently active */
	bool bIsCountdownActive = false;

	/** Accumulates delta time to fire countdown ticks at 1Hz intervals */
	float CountdownTickAccumulator = 0.0f;

	bool bIsTimerRunning;

	bool bResumingFromPause = false;
};
