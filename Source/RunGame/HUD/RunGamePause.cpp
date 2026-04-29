// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/RunGamePause.h"
#include "Components/Button.h"
#include "RunGameGameMode.h"
#include "RunGameGameState.h"
#include "Kismet/GameplayStatics.h"

void URunGamePause::NativeConstruct()
{
	Super::NativeConstruct();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &URunGamePause::OnContinueClicked);
	}
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &URunGamePause::OnRestartClicked);
	}
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &URunGamePause::OnMainMenuClicked);
	}
}

void URunGamePause::OnContinueClicked()
{
	if (ARunGameGameState* GameState = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GameState->SetGameState(ERunGameGameState::CountDown);
	}
}

void URunGamePause::OnRestartClicked()
{
	if (ARunGameGameMode* GameMode = GetRunGameGameMode())
	{
		GameMode->RequestRestartFromPause();
	}
}

void URunGamePause::OnMainMenuClicked()
{
	if (ARunGameGameMode* GameMode = GetRunGameGameMode())
	{
		GameMode->ResetGame();
	}
}

ARunGameGameMode* URunGamePause::GetRunGameGameMode() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		return Cast<ARunGameGameMode>(World->GetAuthGameMode());
	}
	return nullptr;
}
