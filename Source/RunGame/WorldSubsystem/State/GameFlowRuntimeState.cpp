#include "WorldSubsystem/State/GameFlowRuntimeState.h"
#include "RunGame.h"

void UGameFlowRuntimeState::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGameFlowRuntimeState::Deinitialize()
{
	Super::Deinitialize();
}

void UGameFlowRuntimeState::SetGameState(ERunGameGameState NewState)
{
	if (CurrentGameState == NewState)
	{
		return;
	}

	const ERunGameGameState OldState = CurrentGameState;
	CurrentGameState = NewState;
	OnGameStateChanged.Broadcast(OldState, CurrentGameState);
}

void UGameFlowRuntimeState::SetCountdownSeconds(int32 NewValue)
{
	if (CountdownSeconds != NewValue)
	{
		CountdownSeconds = NewValue;
		OnCountdownUpdated.Broadcast(CountdownSeconds);
	}
}

void UGameFlowRuntimeState::BroadcastCountdownComplete()
{
	OnCountdownComplete.Broadcast();
}

void UGameFlowRuntimeState::SetTotalTimeSeconds(float NewValue)
{
	if (TotalTimeSeconds != NewValue)
	{
		TotalTimeSeconds = NewValue;
		OnTimeChanged.Broadcast(TotalTimeSeconds);
	}
}

void UGameFlowRuntimeState::SetTimerRunning(bool bRunning)
{
	bIsTimerRunning = bRunning;
}
