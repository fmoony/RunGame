#include "HUD/RunGameDebugSubsystem.h"
#include "HUD/RunGameDebugPanel.h"
#include "RunGameGameState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
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

	if (UWorld* World = GetWorld())
	{
		if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
		{
			GS->OnGameStateChanged.RemoveDynamic(this, &URunGameDebugSubsystem::OnGameStateChanged);
		}
	}

	Super::Deinitialize();
}

void URunGameDebugSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// 延迟创建：绑定 GameState 变化，进入 CountDown 后（PC 已就绪）再创建面板
	// Deferred creation: bind to GameState, create panel when CountDown starts (PC is ready)
	if (ARunGameGameState* GS = InWorld.GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &URunGameDebugSubsystem::OnGameStateChanged);

		// 如果已经在 CountDown 或更后阶段（重新加载等边缘情况），立刻创建
		// If already past MainMenu (e.g. reload edge case), create immediately
		if (GS->GetCurrentState() != ERunGameGameState::MainMenu)
		{
			CreateDebugPanel();
		}
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameDebugSubsystem: Failed to get GameState"));
	}
}

void URunGameDebugSubsystem::OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (!bPanelCreated && NewState != ERunGameGameState::MainMenu)
	{
		CreateDebugPanel();
	}
}

void URunGameDebugSubsystem::CreateDebugPanel()
{
	if (bPanelCreated)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameDebugSubsystem: Cannot create debug panel — no PlayerController"));
		return;
	}

	DebugPanel = CreateWidget<URunGameDebugPanel>(PC, URunGameDebugPanel::StaticClass());
	if (DebugPanel)
	{
		DebugPanel->AddToViewport(100);
		bPanelCreated = true;
		UE_LOG(LogRunGame, Warning, TEXT("RunGameDebugSubsystem: Debug panel created and visible"));
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameDebugSubsystem: Failed to create debug panel"));
	}
}
