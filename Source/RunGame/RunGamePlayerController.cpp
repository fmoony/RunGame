// Copyright Epic Games, Inc. All Rights Reserved.


#include "RunGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "RunGame.h"
#include "RunGameHUD.h"
#include "RunGameCharacter.h"
#include "RunGameGameState.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Input/SVirtualJoystick.h"

void ARunGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogRunGame, Error, TEXT("Could not spawn mobile controls widget."));

		}
	}

	// spawn the HUD for this player
	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	// 绑定 GameState 状态变化，响应式管理输入模式
	if (ARunGameGameState* GameState = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GameState->OnGameStateChanged.AddDynamic(this, &ARunGamePlayerController::OnGameStateChangedCallback);
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("ARunGamePlayerController BeginPlay: Failed to get GameState for state binding."));
	}
}

void ARunGamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (ARunGameGameState* GameState = World->GetGameState<ARunGameGameState>())
		{
			GameState->OnGameStateChanged.RemoveDynamic(this, &ARunGamePlayerController::OnGameStateChangedCallback);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ARunGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void ARunGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>();
	if (!GS)
	{
		UE_LOG(LogRunGame, Error, TEXT("ARunGamePlayerController OnPossess: Failed to get GameState."));
		return;
	}

	GS->OnCharacterDeath.AddDynamic(this, &ARunGamePlayerController::SetInputModeToUIOnly);

	if (ARunGameCharacter* PlayerCharacter = Cast<ARunGameCharacter>(InPawn))
	{
		GS->OnCharacterDeath.AddDynamic(PlayerCharacter, &ARunGameCharacter::Die);
		UE_LOG(LogRunGame, Warning, TEXT("ARunGamePlayerController OnPossess: Bound Die + SetInputModeToUIOnly to GameState death delegate."));
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("ARunGamePlayerController OnPossess: Pawn is not ARunGameCharacter, cannot bind Die."));
	}
}

void ARunGamePlayerController::OnUnPossess()
{
	ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>();
	if (GS)
	{
		GS->OnCharacterDeath.RemoveDynamic(this, &ARunGamePlayerController::SetInputModeToUIOnly);

		if (ARunGameCharacter* PlayerCharacter = Cast<ARunGameCharacter>(GetPawn()))
		{
			GS->OnCharacterDeath.RemoveDynamic(PlayerCharacter, &ARunGameCharacter::Die);
		}
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("ARunGamePlayerController OnUnPossess: Failed to get GameState."));
	}

	Super::OnUnPossess();
}

void ARunGamePlayerController::SetInputModeToUIOnly()
{
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
	UE_LOG(LogRunGame, Warning, TEXT("ARunGamePlayerController: Input mode set to UI Only"));
}

void ARunGamePlayerController::SetViewTargetToMainMenuCamera()
{
	ACameraActor* FoundCamera = nullptr;

	// 1. 创建迭代器，限定只在 ACameraActor 及其子类中查找
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		// 2. 检查这个相机是否有我们需要的 Tag
		if (It->ActorHasTag(MainMenuCameraTag))
		{
			FoundCamera = *It;
			break; // 【核心优势】：找到了就立刻中止遍历，节省大量 CPU 算力！
		}
	}

	// 3. 执行绑定逻辑
	if (FoundCamera)
	{
		SetViewTargetWithBlend(FoundCamera, 0.3f);
		UE_LOG(LogRunGame, Warning, TEXT("ARunGamePlayerController: View target set to camera with tag '%s' for MainMenu!"), *MainMenuCameraTag.ToString());
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("ARunGamePlayerController: No camera found with tag '%s' for MainMenu view target!"), *MainMenuCameraTag.ToString());
	}
}

void ARunGamePlayerController::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (OldState == NewState)
	{
		return;
	}

	switch (NewState)
	{
	case ERunGameGameState::MainMenu:
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		SetViewTargetToMainMenuCamera();
		break;
	case ERunGameGameState::CountDown:
		bShowMouseCursor = false;
		SetInputMode(FInputModeUIOnly());
		if (OldState != ERunGameGameState::MainMenu)
		SetViewTargetToMainMenuCamera();
		break;
	case ERunGameGameState::InGame:
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		break;
	case ERunGameGameState::Pause:
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		break;
	case ERunGameGameState::GameOver:
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		break;
	default:
		break;
	}
}

