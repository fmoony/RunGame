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
	/** Handles restart button click, starts a new game via GameMode */
	UFUNCTION()
	void OnRestartButtonClicked();

	/** Handles main menu button click, resets game to main menu */
	UFUNCTION()
	void OnMainMenuButtonClicked();

	/** Retrieves the RunGame GameMode from the current world */
	ARunGameGameMode* GetRunGameGameMode() const;

	/** Retrieves the Timer subsystem from the current world */
	URunGameTimerSubsystem* GetTimerSubsystem() const;

	/** Formats time in seconds to a MM:SS.ms string */
	FString FormatTimeText(float TimeSeconds) const;
};

