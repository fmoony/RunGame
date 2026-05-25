#include "Game/RunGameGameState.h"
#include "RunGame.h"

ARunGameGameState::ARunGameGameState()
{
}

void ARunGameGameState::SetGameState(ERunGameGameState NewState)
{
	if (CurrentGameState == NewState)
	{
		return;
	}

	const ERunGameGameState OldState = CurrentGameState;
	CurrentGameState = NewState;

	OnGameStateChanged.Broadcast(OldState, NewState);

	UE_LOG(LogRunGame, Warning, TEXT("GameState: State changed from %d to %d"), static_cast<int32>(OldState), static_cast<int32>(NewState));
}

void ARunGameGameState::SetCountdownSeconds(int32 NewCountdownSeconds)
{
	if (CountdownSeconds != NewCountdownSeconds)
	{
		CountdownSeconds = NewCountdownSeconds;
		OnCountdownUpdated.Broadcast(CountdownSeconds);
	}
}
