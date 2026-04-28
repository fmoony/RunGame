// Fill out your copyright notice in the Description page of Project Settings.

#include "RunGameGameState.h"

ARunGameGameState::ARunGameGameState()
{
	CurrentState = ERunGameGameState::MainMenu;
}

void ARunGameGameState::SetGameState(ERunGameGameState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	ERunGameGameState OldState = CurrentState;
	CurrentState = NewState;

	OnGameStateChanged.Broadcast(OldState, CurrentState);

	UE_LOG(LogTemp, Warning, TEXT("RunGameGameState: Game State Changed from %d to %d"), (int32)OldState, (int32)NewState);
}

void ARunGameGameState::SetCountdownSeconds(int32 NewCountdownSeconds)
{
	if (CountdownSeconds != NewCountdownSeconds)
	{
		CountdownSeconds = NewCountdownSeconds;
		OnCountdownUpdated.Broadcast(CountdownSeconds);
		UE_LOG(LogTemp, Warning, TEXT("RunGameGameState: Countdown Seconds Updated: %d"), CountdownSeconds);
	}
}
