// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/RunGameHUD.h"
#include "RunGame.h"
#include "HUD/RunGameMainMenu.h"
#include "RunGameType.h"
#include "Blueprint/UserWidget.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "Game/RunGameGameState.h"

void ARunGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if(ARunGameGameState* CurrentGameState = GetWorld()->GetGameState<ARunGameGameState>())
	{
		CurrentGameState->OnGameStateChanged.AddDynamic(this, &ARunGameHUD::OnGameStateChangedCallback);

		UpdateUIForState(CurrentGameState->GetCurrentState());
	}
	else
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameHUD: Failed to get current game state in BeginPlay"));
	}
}

void ARunGameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ARunGameGameState* CurrentGameState = GetWorld()->GetGameState<ARunGameGameState>())
	{
		CurrentGameState->OnGameStateChanged.RemoveDynamic(this,&ARunGameHUD::OnGameStateChangedCallback);
	}
	Super::EndPlay(EndPlayReason);
}

void ARunGameHUD::ShowUI(TSubclassOf<UUserWidget> UIClass)
{
	if(UIClass != nullptr)
	{
		// 优化：如果要显示的UI就是当前正在显示的，什么都不做
		if (CurrentActiveWidget && CurrentActiveWidget->GetClass() == UIClass)
		{
			UE_LOG(LogRunGame, Warning, TEXT("RunGameHUD: Widget of class %s is already displayed"), *UIClass->GetName());
			return;
		}

		HideUI(CurrentActiveWidget ? CurrentActiveWidget->GetClass() : nullptr);

		// 2. 创建新的UI
		CurrentActiveWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), UIClass);

		if (CurrentActiveWidget)
		{
			// 3. 添加到视口
			CurrentActiveWidget->AddToViewport();
			UE_LOG(LogRunGame, Warning, TEXT("RunGameHUD: Displayed widget of class %s"), *UIClass->GetName());
		}
		else
		{
			UE_LOG(LogRunGame, Warning, TEXT("RunGameHUD: Failed to create widget of class %s"), *UIClass->GetName());
		}
	}
}

void ARunGameHUD::HideUI(TSubclassOf<UUserWidget> UIClass)
{
	if(UIClass != nullptr)
	{
		if (CurrentActiveWidget && CurrentActiveWidget->GetClass() == UIClass)
		{
			CurrentActiveWidget->RemoveFromParent();
			CurrentActiveWidget = nullptr;
			UE_LOG(LogRunGame, Warning, TEXT("RunGameHUD: Removed widget of class %s from viewport"), *UIClass->GetName());
		}
		else
		{
			UE_LOG(LogRunGame, Warning, TEXT("RunGameHUD: No widget of class %s is currently displayed"), *UIClass->GetName());
		}
	}
}

void ARunGameHUD::UpdateUIForState(ERunGameGameState NewState)
{
	// 1. 从映射表中查找当前状态对应的UI类
	TSubclassOf<UUserWidget>* FoundUIClass = CurrentUIMap.Find(NewState);

	if (FoundUIClass && *FoundUIClass)
	{
		// 2. 如果找到了对应的UI，显示它
		ShowUI(*FoundUIClass);
	}
}

void ARunGameHUD::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	UpdateUIForState(NewState);
}
