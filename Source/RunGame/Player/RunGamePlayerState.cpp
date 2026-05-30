#include "Player/RunGamePlayerState.h"
#include "Game/RunGameGameState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Engine/World.h"

ARunGamePlayerState::ARunGamePlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARunGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();

	// 订阅 Subsystem 的 OnTimeChanged——由规范时钟驱动计分，消除独立定时器
	// Subscribe to Subsystem's OnTimeChanged — canonical clock drives scoring, eliminates standalone timer
	if (TimerSubsystem)
	{
		TimerSubsystem->OnTimeChanged.AddDynamic(this, &ARunGamePlayerState::OnTimeChangedCallback);
	}

	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &ARunGamePlayerState::OnGameStateChangedCallback);
	}
}

void ARunGamePlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
		{
			GS->OnGameStateChanged.RemoveDynamic(this, &ARunGamePlayerState::OnGameStateChangedCallback);
		}
	}

	if (TimerSubsystem)
	{
		TimerSubsystem->OnTimeChanged.RemoveDynamic(this, &ARunGamePlayerState::OnTimeChangedCallback);
	}
	TimerSubsystem = nullptr;
	Super::EndPlay(EndPlayReason);
}

// ---- GameState 响应 GameState Callback ----

void ARunGamePlayerState::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (OldState == NewState)
	{
		return;
	}

	if (NewState == ERunGameGameState::CountDown || NewState == ERunGameGameState::MainMenu)
	{
		SetRunGameScore(0);
	}

	if (NewState == ERunGameGameState::InGame)
	{
		SetScoringActive(true);
	}
	else
	{
		SetScoringActive(false);
	}
}

// ---- Scoring ----

void ARunGamePlayerState::SetScoringActive(bool bActive)
{
	if (bActive)
	{
		bScoringActive = true;
		ScoreTickAccumulator = 0.0f;
		LastScoreTime = TimerSubsystem ? TimerSubsystem->GetTotalTimeSeconds() : 0.0f;
	}
	else
	{
		bScoringActive = false;
	}
}

void ARunGamePlayerState::OnTimeChangedCallback(float NewTime)
{
	if (!bScoringActive)
	{
		return;
	}

	const float DeltaTime = NewTime - LastScoreTime;
	LastScoreTime = NewTime;

	if (DeltaTime <= 0.0f)
	{
		return;
	}

	ScoreTickAccumulator += DeltaTime;
	while (ScoreTickAccumulator >= 0.1f)
	{
		ScoreTickAccumulator -= 0.1f;
		CalculateScoreProcess();
	}
}

void ARunGamePlayerState::CalculateScoreProcess()
{
	if (!TimerSubsystem || !TimerSubsystem->IsTimerRunning())
	{
		return;
	}

	const float Elapsed = TimerSubsystem->GetTotalTimeSeconds();
	const float MinutesPassed = Elapsed / 60.0f;

	const float Multiplier = 1.0f + FMath::Pow(MinutesPassed, 2.0f) + 0.5f * FMath::Pow(MinutesPassed, 3.0f);

	const int64 ScoreToAdd = FMath::FloorToInt64(10LL * Multiplier);

	AddScore(ScoreToAdd);
}

void ARunGamePlayerState::AddScore(int64 Value)
{
	if (Value != 0)
	{
		RunGameScore += Value;
		OnScoreChanged.Broadcast(RunGameScore);
	}
}

void ARunGamePlayerState::SetRunGameScore(int64 NewScore)
{
	if (RunGameScore != NewScore)
	{
		RunGameScore = NewScore;
		OnScoreChanged.Broadcast(RunGameScore);
	}
}
