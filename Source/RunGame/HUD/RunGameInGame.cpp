// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/RunGameInGame.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "RunGamePlayerState.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"

URunGameInGame::URunGameInGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ScoreText(nullptr)
	, TimerText(nullptr)
	, TimerSubsystem(nullptr)
{
}

void URunGameInGame::NativeConstruct()
{
	Super::NativeConstruct();

	// 获取TimerSubsystem引用
	if (UWorld* World = GetWorld())
	{
		TimerSubsystem = World->GetSubsystem<URunGameTimerSubsystem>();
	}

	// 检查UI组件是否正确绑定
	if (!ScoreText)
	{
		UE_LOG(LogTemp, Error, TEXT("ScoreText is not bound in RunGameInGame widget!"));
	}
	else
	{
		// 初始化分数显示
		ScoreText->SetText(FText::FromString(TEXT("Score: 000000000")));
	}

	if (!TimerText)
	{
		UE_LOG(LogTemp, Error, TEXT("TimerText is not bound in RunGameInGame widget!"));
	}
	else
	{
		// 初始化计时显示
		TimerText->SetText(FText::FromString(TEXT("Time: 0.000s")));
	}

	// 绑定委托回调
	if (TimerSubsystem)
	{
		// 绑定时间变更委托
		TimerSubsystem->OnTimeChanged.AddDynamic(this, &URunGameInGame::OnTimerUpdated);

		// 获取PlayerState并绑定分数变更委托
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGamePlayerState* PlayerState = Cast<ARunGamePlayerState>(PC->GetPlayerState<APlayerState>()))
			{
				PlayerState->OnScoreChanged.AddDynamic(this, &URunGameInGame::OnScoreUpdated);
			
				// 立即更新UI显示当前值
				OnScoreUpdated(PlayerState->GetRunGameScore());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get PlayerState for score binding"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get PlayerController for score binding"));
		}

		OnTimerUpdated(TimerSubsystem->GetTotalTimeSeconds());

		UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Successfully bound to TimerSubsystem events"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get TimerSubsystem!"));
	}
}

void URunGameInGame::NativeDestruct()
{
	// 解绑委托回调，防止内存泄漏
	if (TimerSubsystem)
	{
		TimerSubsystem->OnTimeChanged.RemoveDynamic(this, &URunGameInGame::OnTimerUpdated);
	
		// 解绑PlayerState的分数委托
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGamePlayerState* PlayerState = Cast<ARunGamePlayerState>(PC->GetPlayerState<APlayerState>()))
			{
				PlayerState->OnScoreChanged.RemoveDynamic(this, &URunGameInGame::OnScoreUpdated);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Successfully unbound from TimerSubsystem events"));
	}

	Super::NativeDestruct();
}

void URunGameInGame::OnScoreUpdated(int64 NewScore)
{
	// 更新分数文本显示
	if (ScoreText)
	{
		FString ScoreString = FString::Printf(TEXT("Score: %09d"), NewScore);
		ScoreText->SetText(FText::FromString(ScoreString));
		//UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Score updated to %d"), NewScore);
	}
}

void URunGameInGame::OnTimerUpdated(float NewTime)
{
	// 更新计时文本显示
	if (TimerText)
	{
		FString TimeString = FString::Printf(TEXT("Time: %s"), *FormatTimeText(NewTime));
		TimerText->SetText(FText::FromString(TimeString));
		//UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Time updated to %f seconds"), NewTime);
	}
}

FString URunGameInGame::FormatTimeText(float TimeSeconds) const
{
	// 格式化时间显示为 MM:SS.ms 格式
	int32 Minutes = FMath::FloorToInt(TimeSeconds / 60.0f);
	int32 Seconds = FMath::FloorToInt(TimeSeconds) % 60;
	int32 Milliseconds = FMath::FloorToInt((TimeSeconds - FMath::FloorToInt(TimeSeconds)) * 1000.0f);

	return FString::Printf(TEXT("%02d:%02d.%03d"), Minutes, Seconds, Milliseconds);
}
