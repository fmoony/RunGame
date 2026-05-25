// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RunGameMainMenu.h"
#include "Game/RunGameGameState.h"
#include "Game/RunGameGameMode.h"
#include <Kismet/GameplayStatics.h>

void URunGameMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &URunGameMainMenu::OnStartClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StartButton is not bound!"));
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &URunGameMainMenu::OnQuitClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuitButton is not bound!"));
	}
}


void URunGameMainMenu::StartGame()
{
	//FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	//if (CurrentLevelName == "")
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Current level name is empty!"));
	//	return;
	//}
	//UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));

	if(ARunGameGameMode* GameMode = GetWorld()->GetAuthGameMode<ARunGameGameMode>())
	{
		GameMode->StartGameCountDown();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get GameMode in StartGame"));
	}
}

void URunGameMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void URunGameMainMenu::OnStartClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStartClicked"));
	StartGame();
}

void URunGameMainMenu::OnQuitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnQuitClicked"));
	QuitGame();
}
