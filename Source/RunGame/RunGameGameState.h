// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RunGameType.h"
#include "RunGameGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChangedSignature, ERunGameGameState, OldGameState, ERunGameGameState, NewGameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdatedSignature, int32, CountdownSeconds);

/**
 * 游戏状态薄包装 —— 数据存储已迁移到 UGameFlowRuntimeState。
 * 保留委托作为向后兼容的转发层，消费者无需改动。
 */
UCLASS()
class RUNGAME_API ARunGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ARunGameGameState();

	// ---- 游戏状态控制 ----
	/** 将状态写入 GameFlowRuntimeState（RS 负责广播） */
	UFUNCTION(BlueprintCallable, Category = "RunGame|State")
	void SetGameState(ERunGameGameState NewState);

	/** 从 GameFlowRuntimeState 读取当前状态 */
	UFUNCTION(BlueprintPure, Category = "RunGame|State")
	ERunGameGameState GetCurrentState() const;

	// ---- 倒计时数据 ----

	UFUNCTION(BlueprintCallable, Category = "RunGame|Countdown")
	void SetCountdownSeconds(int32 NewCountdownSeconds);

	UFUNCTION(BlueprintCallable, Category = "RunGame|Countdown")
	int32 GetCountdownSeconds() const;

	// ---- 可配置默认值 (同步到 GameFlowRuntimeState) ----

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Config")
	int32 DefaultCountdownSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "RunGame|Config")
	float DefaultGameTotalTime = 0.0f;

	// ---- 委托 (向后兼容，由 RS 转发) ----

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Events")
	FOnGameStateChangedSignature OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Events")
	FOnCountdownUpdatedSignature OnCountdownUpdated;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// ---- RS 转发器回调 ----

	UFUNCTION()
	void OnRS_GameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	UFUNCTION()
	void OnRS_CountdownUpdated(int32 CountdownSeconds);

	UFUNCTION()
	void OnRS_CountdownComplete();
};
