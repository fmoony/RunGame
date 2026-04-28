// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameInGame.generated.h"

class URunGameTimerSubsystem;
class UTextBlock;

/**
 * 游戏中UI - 显示实时分数和计时
 */
UCLASS()
class RUNGAME_API URunGameInGame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 构造函数
	URunGameInGame(const FObjectInitializer& ObjectInitializer);

protected:
	// Native生命周期函数
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	// UI组件 - 使用BindWidget元数据绑定到蓝图
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	// Subsystem引用
	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

	// 委托回调函数
	UFUNCTION()
	void OnScoreUpdated(int64 NewScore);

	UFUNCTION()
	void OnTimerUpdated(float NewTime);

	// 辅助函数：格式化时间显示
	FString FormatTimeText(float TimeSeconds) const;
};
