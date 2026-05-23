#include "RunGamePlayerState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "WorldSubsystem/State/GameFlowRuntimeState.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
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

	if (UGameFlowRuntimeState* GRS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		GRS->OnGameStateChanged.AddDynamic(this, &ARunGamePlayerState::OnRS_GameStateChanged);
	}

	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->OnScoreChanged.AddDynamic(this, &ARunGamePlayerState::OnRS_ScoreChanged);
	}
}

void ARunGamePlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ScoreTimerHandle);

		if (UGameFlowRuntimeState* GRS = World->GetSubsystem<UGameFlowRuntimeState>())
		{
			GRS->OnGameStateChanged.RemoveDynamic(this, &ARunGamePlayerState::OnRS_GameStateChanged);
		}

		if (UPlayerRuntimeState* RS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			RS->OnScoreChanged.RemoveDynamic(this, &ARunGamePlayerState::OnRS_ScoreChanged);
		}
	}
	TimerSubsystem = nullptr;
	Super::EndPlay(EndPlayReason);
}

// ---- RS 回调 ----

void ARunGamePlayerState::OnRS_GameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState)
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

void ARunGamePlayerState::OnRS_ScoreChanged(int64 NewScore)
{
	OnScoreChanged.Broadcast(NewScore);
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
	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->AddScore(Value);
	}
}

void ARunGamePlayerState::SetRunGameScore(int64 NewScore)
{
	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->SetRunGameScore(NewScore);
	}
}

int64 ARunGamePlayerState::GetRunGameScore() const
{
	if (const UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		return RS->GetRunGameScore();
	}
	return 0;
}
