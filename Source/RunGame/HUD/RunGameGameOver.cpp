// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/RunGameGameOver.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RunGameGameMode.h"
#include "RunGamePlayerState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"

void URunGameGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	// 绑定按钮点击事件
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &URunGameGameOver::OnRestartButtonClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &URunGameGameOver::OnMainMenuButtonClicked);
	}

	// 显示时自动更新分数和时间
	UpdateScoreAndTime();
}

void URunGameGameOver::UpdateScoreAndTime()
{
	// 从 PlayerState 获取分数
	int32 CurrentScore = 0;
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ARunGamePlayerState* PlayerState = Cast<ARunGamePlayerState>(PC->GetPlayerState<APlayerState>()))
		{
			CurrentScore = PlayerState->GetRunGameScore();
		}
	}

	// 从 TimerSubsystem 获取时长
	float CurrentTime = 0.0f;
	if (URunGameTimerSubsystem* TimerSubsystem = GetTimerSubsystem())
	{
		CurrentTime = TimerSubsystem->GetTotalTimeSeconds();
	}

	// 更新分数文本
	if (ScoreText)
	{
		FString ScoreString = FString::Printf(TEXT("Score: %09d"), CurrentScore);
		ScoreText->SetText(FText::FromString(ScoreString));
	}

	// 更新时长文本（分:秒格式）
	if (TimeText)
	{
		FString TimeString = FString::Printf(TEXT("Time: %s"), *FormatTimeText(CurrentTime));
		TimeText->SetText(FText::FromString(TimeString));
	}
}

void URunGameGameOver::OnRestartButtonClicked()
{
	if (ARunGameGameMode* GameMode = GetRunGameGameMode())
	{
		GameMode->StartNewGame();
	}
}

void URunGameGameOver::OnMainMenuButtonClicked()
{
	if (ARunGameGameMode* GameMode = GetRunGameGameMode())
	{
		GameMode->ResetGame();
	}
}

ARunGameGameMode* URunGameGameOver::GetRunGameGameMode() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		return Cast<ARunGameGameMode>(World->GetAuthGameMode());
	}
	return nullptr;
}

URunGameTimerSubsystem* URunGameGameOver::GetTimerSubsystem() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		return World->GetSubsystem<URunGameTimerSubsystem>();
	}
	return nullptr;
}

FString URunGameGameOver::FormatTimeText(float TimeSeconds) const
{
	// 格式化时间显示为 MM:SS.ms 格式
	int32 Minutes = FMath::FloorToInt(TimeSeconds / 60.0f);
	int32 Seconds = FMath::FloorToInt(TimeSeconds) % 60;
	int32 Milliseconds = FMath::FloorToInt((TimeSeconds - FMath::FloorToInt(TimeSeconds)) * 1000.0f);

	return FString::Printf(TEXT("%02d:%02d.%03d"), Minutes, Seconds, Milliseconds);
}