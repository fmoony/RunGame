#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "WorldSubsystem/State/GameFlowRuntimeState.h"
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

	if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		RS->OnGameStateChanged.RemoveDynamic(this, &URunGameTimerSubsystem::OnGameStateChangedCallback);
		RS->OnTimeChanged.RemoveDynamic(this, &URunGameTimerSubsystem::OnRS_TimeChanged);
		RS->OnCountdownComplete.RemoveDynamic(this, &URunGameTimerSubsystem::OnRS_CountdownComplete);
	}

	Super::Deinitialize();
}

float URunGameTimerSubsystem::GetTotalTimeSeconds() const
{
	if (const UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		return RS->GetTotalTimeSeconds();
	}
	return 0.0f;
}

bool URunGameTimerSubsystem::IsTimerRunning() const
{
	if (const UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		return RS->IsTimerRunning();
	}
	return false;
}

// ---- 响应式绑定 ----

void URunGameTimerSubsystem::OnWorldBeginPlay(UWorld& World)
{
	Super::OnWorldBeginPlay(World);

	if (UGameFlowRuntimeState* RS = World.GetSubsystem<UGameFlowRuntimeState>())
	{
		// 状态机响应
		RS->OnGameStateChanged.AddDynamic(this, &URunGameTimerSubsystem::OnGameStateChangedCallback);
		// 委托转发
		RS->OnTimeChanged.AddDynamic(this, &URunGameTimerSubsystem::OnRS_TimeChanged);
		RS->OnCountdownComplete.AddDynamic(this, &URunGameTimerSubsystem::OnRS_CountdownComplete);
		UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Bound to GameFlowRuntimeState"));
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameTimerSubsystem: Failed to get GameFlowRuntimeState"));
	}
}

// ---- RS 转发器 ----

void URunGameTimerSubsystem::OnRS_TimeChanged(float NewTime)
{
	OnTimeChanged.Broadcast(NewTime);
}

void URunGameTimerSubsystem::OnRS_CountdownComplete()
{
	OnCountdownComplete.Broadcast();
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
		bResumingFromPause = false;
		break;
	case ERunGameGameState::CountDown:
		StartCountdown();
		break;
	case ERunGameGameState::InGame:
		if (OldState == ERunGameGameState::Pause || bResumingFromPause)
		{
			if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
			{
				RS->SetTimerRunning(true);
			}
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

// ---- 内部倒计时 ----

void URunGameTimerSubsystem::StartCountdown()
{
	UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>();
	if (!RS)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameTimerSubsystem::StartCountdown: No GameFlowRuntimeState"));
		return;
	}

	const int32 InitialSeconds = FMath::RoundToInt(RS->DefaultCountdownSeconds);
	if (InitialSeconds <= 0)
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Invalid countdown seconds: %d"), InitialSeconds);
		return;
	}

	StopCountdown();

	RS->SetCountdownSeconds(InitialSeconds);

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
	UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>();
	if (!RS)
	{
		StopCountdown();
		return;
	}

	int32 Current = RS->GetCountdownSeconds();
	if (Current > 0)
	{
		Current--;
		RS->SetCountdownSeconds(Current);
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

	if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		RS->SetGameState(ERunGameGameState::InGame);
		RS->BroadcastCountdownComplete();
	}
}

// ---- 内部游戏计时 ----

void URunGameTimerSubsystem::StartTimer()
{
	UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>();
	if (!RS)
	{
		return;
	}

	RS->SetTotalTimeSeconds(0.0f);
	RS->SetTimerRunning(true);

	// 即使 RS 因为 guard 没有广播（值为 0.0），也确保消费者收到重置
	OnTimeChanged.Broadcast(0.0f);

	UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Forward timer started from 0.0 seconds"));
}

void URunGameTimerSubsystem::StopTimer()
{
	if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		RS->SetTimerRunning(false);
	}
	UE_LOG(LogRunGame, Warning, TEXT("RunGameTimerSubsystem: Timer stopped"));
}

void URunGameTimerSubsystem::UpdateTimer(float DeltaTime)
{
	UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>();
	if (!RS || !RS->IsTimerRunning())
	{
		return;
	}

	RS->SetTotalTimeSeconds(RS->GetTotalTimeSeconds() + DeltaTime);
}
