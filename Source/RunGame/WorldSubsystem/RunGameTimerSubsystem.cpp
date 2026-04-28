// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "RunGameGameState.h"
#include "Engine/World.h"

URunGameTimerSubsystem::URunGameTimerSubsystem()
{
	TotalTimeSeconds = 0.0f;
	bIsTimerRunning = false;
}

void URunGameTimerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URunGameTimerSubsystem::Deinitialize()
{
	StopCountdown();
	StopTimer();

	if (ARunGameGameState* GameState = GetGameState())
	{
		GameState->OnGameStateChanged.RemoveDynamic(this, &URunGameTimerSubsystem::OnGameStateChangedCallback);
	}

	Super::Deinitialize();
}

// ---- 响应式绑定 ----

void URunGameTimerSubsystem::OnWorldBeginPlay(UWorld& World)
{
	Super::OnWorldBeginPlay(World);

	if (ARunGameGameState* GameState = World.GetGameState<ARunGameGameState>())
	{
		GameState->OnGameStateChanged.AddDynamic(this, &URunGameTimerSubsystem::OnGameStateChangedCallback);
		UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Bound to OnGameStateChanged"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameTimerSubsystem: Failed to bind to OnGameStateChanged - no GameState"));
	}
}

// ---- 状态机响应回调 ----

void URunGameTimerSubsystem::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (OldState == NewState)
	{
		return;
	}

	switch (NewState)
	{
	case ERunGameGameState::MainMenu:
		StopTimer();
		break;
	case ERunGameGameState::CountDown:
		StartCountdown();
		break;
	case ERunGameGameState::InGame:
		StartTimer();
		break;
	case ERunGameGameState::Pause:
		StopTimer();
		break;
	case ERunGameGameState::GameOver:
		StopTimer();
		break;
	default:
		break;
	}
}

// ---- Tick ----

void URunGameTimerSubsystem::Tick(float DeltaTime)
{
	UpdateTimer(DeltaTime);
}

TStatId URunGameTimerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMyLevelSubsystem, STATGROUP_Tickables);
}

bool URunGameTimerSubsystem::IsTickable() const
{
	return !HasAnyFlags(RF_ClassDefaultObject);
}

// ---- 内部倒计时 ----

void URunGameTimerSubsystem::StartCountdown()
{
	ARunGameGameState* GameState = GetGameState();
	if (!GameState)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameTimerSubsystem::StartCountdown: No GameState"));
		return;
	}

	const int32 InitialSeconds = GameState->DefaultCountdownSeconds;
	if (InitialSeconds <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Invalid countdown seconds: %d"), InitialSeconds);
		return;
	}

	StopCountdown();

	GameState->SetCountdownSeconds(InitialSeconds);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CountdownTimerHandle, this, &URunGameTimerSubsystem::UpdateCountdown, 1.0f, true);
	}

	UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Countdown started with %d seconds"), InitialSeconds);
}

void URunGameTimerSubsystem::StopCountdown()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
	UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Countdown stopped"));
}

void URunGameTimerSubsystem::UpdateCountdown()
{
	ARunGameGameState* GameState = GetGameState();
	if (!GameState)
	{
		StopCountdown();
		return;
	}

	int32 Current = GameState->GetCountdownSeconds();
	if (Current > 0)
	{
		Current--;
		GameState->SetCountdownSeconds(Current);
		UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Countdown updated: %d"), Current);
	}
	else
	{
		FinishCountdown();
	}
}

void URunGameTimerSubsystem::FinishCountdown()
{
	StopCountdown();

	if (ARunGameGameState* GameState = GetGameState())
	{
		GameState->SetGameState(ERunGameGameState::InGame);
	}

	OnCountdownComplete.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Countdown finished, game started"));
}

// ---- 内部游戏计时 ----

void URunGameTimerSubsystem::StartTimer()
{
	// 正向计时：从 0.0 开始累计，每 Tick 累加 DeltaTime
	TotalTimeSeconds = 0.0f;
	bIsTimerRunning = true;

	OnTimeChanged.Broadcast(TotalTimeSeconds);

	UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Forward timer started from 0.0 seconds"));
}

void URunGameTimerSubsystem::StopTimer()
{
	bIsTimerRunning = false;
	UE_LOG(LogTemp, Warning, TEXT("RunGameTimerSubsystem: Timer stopped at: %f seconds"), TotalTimeSeconds);
}

void URunGameTimerSubsystem::UpdateTimer(float DeltaTime)
{
	if (!bIsTimerRunning)
	{
		return;
	}

	TotalTimeSeconds += DeltaTime;
	OnTimeChanged.Broadcast(TotalTimeSeconds);
}

// ---- 辅助 ----

ARunGameGameState* URunGameTimerSubsystem::GetGameState() const
{
	if (UWorld* World = GetWorld())
	{
		return Cast<ARunGameGameState>(World->GetGameState());
	}
	return nullptr;
}
