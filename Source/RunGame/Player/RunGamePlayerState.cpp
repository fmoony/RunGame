#include "Player/RunGamePlayerState.h"
#include "Game/RunGameGameState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

ARunGamePlayerState::ARunGamePlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARunGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			ScoreTimerHandle,
			this,
			&ARunGamePlayerState::CalculateScoreProcess,
			0.1f,
			true
		);
		World->GetTimerManager().PauseTimer(ScoreTimerHandle);
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
		World->GetTimerManager().ClearTimer(ScoreTimerHandle);

		if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
		{
			GS->OnGameStateChanged.RemoveDynamic(this, &ARunGamePlayerState::OnGameStateChangedCallback);
		}
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
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (bActive)
	{
		World->GetTimerManager().UnPauseTimer(ScoreTimerHandle);
	}
	else
	{
		World->GetTimerManager().PauseTimer(ScoreTimerHandle);
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
