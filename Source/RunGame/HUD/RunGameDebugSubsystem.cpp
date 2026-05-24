#include "HUD/RunGameDebugSubsystem.h"
#include "HUD/RunGameDebugPanel.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "RunGame.h"

void URunGameDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URunGameDebugSubsystem::Deinitialize()
{
	if (DebugPanel)
	{
		DebugPanel->RemoveFromParent();
		DebugPanel = nullptr;
	}

	Super::Deinitialize();
}

void URunGameDebugSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	DebugPanel = CreateWidget<URunGameDebugPanel>(InWorld.GetFirstPlayerController(), URunGameDebugPanel::StaticClass());
	if (DebugPanel)
	{
		DebugPanel->AddToViewport(100);
		UE_LOG(LogRunGame, Warning, TEXT("RunGameDebugSubsystem: Debug panel created and visible"));
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameDebugSubsystem: Failed to create debug panel"));
	}
}
