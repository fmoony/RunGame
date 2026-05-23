#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameType.h"
#include "GameFlowRuntimeState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGF_OnGameStateChanged, ERunGameGameState, OldGameState, ERunGameGameState, NewGameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGF_OnCountdownUpdated, int32, CountdownSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGF_OnCountdownComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGF_OnTimeChanged, float, NewTime);

/**
 * 游戏流程运行时状态 —— 游戏状态、倒计时、正向计时器。
 * 通过 GetWorld()->GetSubsystem<UGameFlowRuntimeState>() 访问。
 */
UCLASS()
class RUNGAME_API UGameFlowRuntimeState : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// ---- Game State ----

	UFUNCTION(BlueprintCallable, Category = "GameFlow|State")
	void SetGameState(ERunGameGameState NewState);

	UFUNCTION(BlueprintPure, Category = "GameFlow|State")
	ERunGameGameState GetGameState() const { return CurrentGameState; }

	UPROPERTY(BlueprintAssignable, Category = "GameFlow|Events")
	FGF_OnGameStateChanged OnGameStateChanged;

	UFUNCTION(BlueprintCallable, Category = "GameFlow|Countdown")
	void SetCountdownSeconds(int32 NewValue);

	UFUNCTION(BlueprintPure, Category = "GameFlow|Countdown")
	int32 GetCountdownSeconds() const { return CountdownSeconds; }

	UPROPERTY(BlueprintAssignable, Category = "GameFlow|Events")
	FGF_OnCountdownUpdated OnCountdownUpdated;

	UPROPERTY(BlueprintAssignable, Category = "GameFlow|Events")
	FGF_OnCountdownComplete OnCountdownComplete;

	void BroadcastCountdownComplete();

	UPROPERTY(BlueprintReadWrite, Category = "GameFlow|Config")
	float DefaultCountdownSeconds = 3.0f;

	UPROPERTY(BlueprintReadWrite, Category = "GameFlow|Config")
	float DefaultGameTotalTime = 0.0f;

	// ---- Timer ----

	UFUNCTION(BlueprintCallable, Category = "GameFlow|Timer")
	void SetTotalTimeSeconds(float NewValue);

	UFUNCTION(BlueprintPure, Category = "GameFlow|Timer")
	float GetTotalTimeSeconds() const { return TotalTimeSeconds; }

	UFUNCTION(BlueprintCallable, Category = "GameFlow|Timer")
	void SetTimerRunning(bool bRunning);

	UFUNCTION(BlueprintPure, Category = "GameFlow|Timer")
	bool IsTimerRunning() const { return bIsTimerRunning; }

	UPROPERTY(BlueprintAssignable, Category = "GameFlow|Events")
	FGF_OnTimeChanged OnTimeChanged;

	// ---- Lifecycle ----

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	ERunGameGameState CurrentGameState = ERunGameGameState::MainMenu;
	int32 CountdownSeconds = 0;
	float TotalTimeSeconds = 0.0f;
	bool bIsTimerRunning = false;
};
