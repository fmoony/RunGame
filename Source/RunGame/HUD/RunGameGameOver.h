// Fill out your Copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameGameOver.generated.h"

class UButton;
class UTextBlock;
class ARunGameGameMode;
class URunGameTimerSubsystem;

/**
 * 游戏结束界面
 * 通过 GameState 自动触发显示，从 PlayerState 和 TimerSubsystem 获取数据
 */
UCLASS()
class RUNGAME_API URunGameGameOver : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 更新分数和时长的显示（从 PlayerState 和 TimerSubsystem 获取数据） */
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void UpdateScoreAndTime();

protected:
	virtual void NativeConstruct() override;

	/** UI 控件绑定 - 需要在蓝图子控件中命名一致 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

private:
	/** 按钮点击回调 */
	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();

	/** 获取 GameMode 辅助函数 */
	ARunGameGameMode* GetRunGameGameMode() const;

	/** 获取 TimerSubsystem 辅助函数 */
	URunGameTimerSubsystem* GetTimerSubsystem() const;

	FString FormatTimeText(float TimeSeconds) const;
};

