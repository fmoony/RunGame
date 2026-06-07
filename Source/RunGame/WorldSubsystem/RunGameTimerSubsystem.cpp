#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Game/RunGameGameState.h"
#include "Engine/World.h"
#include "RunGame.h"

URunGameTimerSubsystem::URunGameTimerSubsystem()
{
}

TStatId URunGameTimerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URunGameTimerSubsystem, STATGROUP_Tickables);
}

void URunGameTimerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URunGameTimerSubsystem::Deinitialize()
{
	StopCountdown();
	StopTimer();

	if (UWorld* World = GetWorld())
	{
		if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
		{
			GS->OnGameStateChanged.RemoveDynamic(this, &URunGameTimerSubsystem::OnGameStateChangedCallback);
		}
	}

	Super::Deinitialize();
}

// ---- 响应式绑定 Reactive Binding ----

void URunGameTimerSubsystem::OnWorldBeginPlay(UWorld& World)
{
	Super::OnWorldBeginPlay(World);

	if (ARunGameGameState* GS = World.GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &URunGameTimerSubsystem::OnGameStateChangedCallback);
		UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Bound to GameState::OnGameStateChanged"));
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameTimerSubsystem: Failed to get GameState"));
	}
}

// ---- 状态机响应回调 State Machine Callback ----

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
		bResumingFromPause = false;
		break;
	case ERunGameGameState::CountDown:
		StartCountdown();
		break;
	case ERunGameGameState::InGame:
		if (OldState == ERunGameGameState::Pause || bResumingFromPause)
		{
			bIsTimerRunning = true;
			bResumingFromPause = false;
		}
		else
		{
			StartTimer();
		}
		break;
	case ERunGameGameState::Pause:
		StopTimer();
		if (OldState == ERunGameGameState::InGame)
		{
			bResumingFromPause = true;
		}
		break;
	case ERunGameGameState::GameOver:
		StopTimer();
		bResumingFromPause = false;
		break;
	default:
		break;
	}
}

// ---- Tick ----

void URunGameTimerSubsystem::Tick(float DeltaTime)
{
	UpdateTimer(DeltaTime);

	if (bIsCountdownActive)
	{
		CountdownTickAccumulator += DeltaTime;
		while (CountdownTickAccumulator >= 1.0f)
		{
			CountdownTickAccumulator -= 1.0f;
			UpdateCountdown();
		}
	}
}

// ---- 内部倒计时 Internal Countdown ----

void URunGameTimerSubsystem::StartCountdown()
{
	ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>();
	if (!GS)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameTimerSubsystem::StartCountdown: No GameState"));
		return;
	}

	const int32 InitialSeconds = GS->DefaultCountdownSeconds;
	if (InitialSeconds <= 0)
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Invalid countdown seconds: %d"), InitialSeconds);
		return;
	}

	StopCountdown();

	GS->SetCountdownSeconds(InitialSeconds);

	bIsCountdownActive = true;
	CountdownTickAccumulator = 0.0f;

	UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Countdown started with %d seconds"), InitialSeconds);
}

void URunGameTimerSubsystem::StopCountdown()
{
	bIsCountdownActive = false;
	CountdownTickAccumulator = 0.0f;
	UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Countdown stopped"));
}

void URunGameTimerSubsystem::UpdateCountdown()
{
	ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>();
	if (!GS)
	{
		StopCountdown();
		return;
	}

	int32 Current = GS->GetCountdownSeconds();
	if (Current > 0)
	{
		Current--;
		GS->SetCountdownSeconds(Current);
		UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Countdown updated: %d"), Current);
	}
	else
	{
		FinishCountdown();
	}
}

void URunGameTimerSubsystem::FinishCountdown()
{
	StopCountdown();

	// 先解冻引擎 — HUD 切到 InGame 时世界已在运行
	// Unfreeze engine first — world is already running when HUD switches to InGame
	OnCountdownComplete.Broadcast();

	ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>();
	if (GS)
	{
		GS->SetGameState(ERunGameGameState::InGame);
	}
}

// ---- 内部游戏计时 Internal Game Timer ----

void URunGameTimerSubsystem::StartTimer()
{
	TotalTimeSeconds = 0.0f;
	bIsTimerRunning = true;

	// 即使初始值为 0，也确保消费者收到重置 Ensure consumers receive reset even when value is 0
	OnTimeChanged.Broadcast(0.0f);

	UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Forward timer started from 0.0 seconds"));
}

void URunGameTimerSubsystem::StopTimer()
{
	bIsTimerRunning = false;
	UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Timer stopped"));
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
