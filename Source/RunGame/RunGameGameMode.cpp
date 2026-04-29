// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunGameGameMode.h"
#include "RunGameCharacter.h"
#include "RunGamePlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "WorldSubsystem/RunGameFloorSubsystem.h"
#include "RunGameGameState.h"
#include "RunGamePlayerState.h"

ARunGameGameMode::ARunGameGameMode()
{
	// stub
	DefaultPawnClass = ASpectatorPawn::StaticClass();

	// 设置默认游戏时间
	GameTotalTime = 0.0f;
}

void ARunGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 绑定 GameState 状态变化 — 进入 InGame 时生成玩家
	if (ARunGameGameState* GS = GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &ARunGameGameMode::OnGameStateChangedCallback);
	}

	if (URunGameFloorSubsystem* FloorSystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>())
	{
		if (StraightFloorClasses.Num() > 0)
		{
			FloorSystem->InitializeFloorSystem(StraightFloorClasses, TurnFloorClasses, PreAllocateFloorCount);
			FloorSystem->OnFloorSystemReady.AddDynamic(this, &ARunGameGameMode::OnFloorSystemReadyCallback);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: StraightFloorClasses is empty. Skipping floor initialization."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameGameMode: Failed to get URunGameFloorSubsystem!"));
	}
}

void ARunGameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ARunGameGameState* GS = GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.RemoveDynamic(this, &ARunGameGameMode::OnGameStateChangedCallback);
	}

	Super::EndPlay(EndPlayReason);
}

void ARunGameGameMode::OnFloorSystemReadyCallback()
{
	if (URunGameFloorSubsystem* FloorSystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>())
	{
		FTransform StartTransform = FTransform::Identity;
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->GetPawn())
		{
			StartTransform = PC->GetPawn()->GetTransform();
		}

		FloorSystem->SpawnInitialFloors(StartTransform, InitialStraightFloorCount, InitialRandomFloorCount);
	}
}

void ARunGameGameMode::StartGameCountDown(int32 CountdownSeconds /* = 3*/)
{
	if (ARunGameGameState* CurrentGameState = GetGameState<ARunGameGameState>())
	{
		CurrentGameState->SetGameState(ERunGameGameState::CountDown);
	}
	// TimerSubsystem 自行监听 OnGameStateChanged 来启动倒计时
	// SpawnPlayer 由 OnGameStateChangedCallback 在状态变为 InGame 时自行触发
}

void ARunGameGameMode::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (OldState == NewState)
	{
		return;
	}

	switch (NewState)
	{
	case ERunGameGameState::MainMenu:
		bResumingFromPause = false;
		break;
	case ERunGameGameState::InGame:
		if (OldState == ERunGameGameState::Pause || bResumingFromPause)
		{
			bResumingFromPause = false;
		}
		else
		{
			SpawnPlayer();
		}
		break;
	case ERunGameGameState::Pause:
		if (OldState == ERunGameGameState::InGame)
		{
			bResumingFromPause = true;
		}
		break;
	case ERunGameGameState::GameOver:
		bResumingFromPause = false;
		break;
	default:
		break;
	}
}

void ARunGameGameMode::SpawnPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: SpawnPlayer called - spawning player character"));

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		// 寻找合适的PlayerStart
		AActor* StartSpot = ChoosePlayerStart(PC);
		FTransform SpawnTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (GameCharacterClass)
		{
			if (APawn* RealPlayer = GetWorld()->SpawnActor<APawn>(GameCharacterClass, SpawnTransform, SpawnParams))
			{
				// 拥有玩家
				PC->Possess(RealPlayer);
				// 视角从死亡摄像机切回角色
				PC->SetViewTargetWithBlend(RealPlayer, 0.3f);
				PC->bAutoManageActiveCameraTarget = true;

				// 输入模式由 Controller 自行监听 GameState 状态变化来管理

				UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: Player spawned and possessed successfully"));

				// InitializeGameTimer removed: SetGameState(InGame) triggers StartTimer reactively
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("RunGameGameMode: Failed to spawn player character"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("RunGameGameMode: GameCharacterClass is NOT set in GameMode! Cannot spawn player."));
		}
	}
	else
		{
		UE_LOG(LogTemp, Error, TEXT("RunGameGameMode: Failed to get PlayerController"));
	}
}

// 游戏计时相关函数实现
void ARunGameGameMode::InitializeGameTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: InitializeGameTimer deprecated - timer starts reactively via GameState state change."));
}

// 重置游戏函数（完全重置到初始状态 = 回到主菜单）
void ARunGameGameMode::ResetGame()
{
	// 分数由 PlayerState 自行监听 GameState 状态变化来清除

	// 回收所有活跃地板并重新生成初始地板链
	if (URunGameFloorSubsystem* FloorSystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>())
	{
		FloorSystem->HideAllActiveFloors();

		FTransform StartTransform = FTransform::Identity;
		if (AActor* StartSpot = ChoosePlayerStart(GetWorld()->GetFirstPlayerController()))
		{
			StartTransform = StartSpot->GetActorTransform();
		}

		FloorSystem->SpawnInitialFloors(StartTransform, InitialStraightFloorCount, InitialRandomFloorCount);
	}

	// 设置游戏状态为 MainMenu，触发 HUD 自动切换到主菜单 UI
	if (ARunGameGameState* CurrentGameState = GetGameState<ARunGameGameState>())
	{
		CurrentGameState->SetGameState(ERunGameGameState::MainMenu);
	}
	// 输入模式由 Controller 自行监听 GameState 状态变化来管理

	UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: Game reset, all data cleared, returned to MainMenu"));
}

// 重新开始游戏（绕过主菜单，直接进入倒计时）
void ARunGameGameMode::StartNewGame()
{
	// 分数由 PlayerState 自行监听 GameState 状态变化来清除

	// 回收所有活跃地板并重新生成初始地板链
	if (URunGameFloorSubsystem* FloorSystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>())
	{
		FloorSystem->HideAllActiveFloors();

		FTransform StartTransform = FTransform::Identity;
		if (AActor* StartSpot = ChoosePlayerStart(GetWorld()->GetFirstPlayerController()))
		{
			StartTransform = StartSpot->GetActorTransform();
		}

		FloorSystem->SpawnInitialFloors(StartTransform, InitialStraightFloorCount, InitialRandomFloorCount);
	}

	StartGameCountDown();

	UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: New game started directly"));
}

// 玩家死亡处理函数实现
void ARunGameGameMode::HandlePlayerDeath(
		ARunGameDeathVolume* DeathVolume,
		ARunGameCharacter* PlayerCharacter,
		bool bImmediate,
		float Delay)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// 广播玩家死亡事件
	OnPlayerDeath.Broadcast(PlayerCharacter);

	// 设置游戏状态为 GameOver，HUD 自动切换到 GameOver UI
	if (ARunGameGameState* CurrentGameState = GetGameState<ARunGameGameState>())
	{
		CurrentGameState->SetGameState(ERunGameGameState::GameOver);
	}

	// 通过 GameState 的死亡多播广播，Die() 已绑定在其上
	if (ARunGameGameState* GS = GetGameState<ARunGameGameState>())
	{
		GS->OnCharacterDeath.Broadcast();
	}

	UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: Player death handled - Immediate: %d, Delay: %.2f"), bImmediate, Delay);

	// 鼠标/输入模式控制已由 Character::Die() 自行处理
	// 销毁前先 UnPossess，防止引擎层面因 Pawn 被销毁而自动重置视角
	if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()))
	{
		PC->UnPossess();
	}

	if (bImmediate || Delay <= 0.0f)
	{
		PlayerCharacter->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: Player destroyed immediately"));
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[this, PlayerCharacter]() {
				if (IsValid(PlayerCharacter))
				{
					PlayerCharacter->Destroy();
					UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: Player destroyed after delay"));
				}
			},
			Delay,
			false
		);
		UE_LOG(LogTemp, Warning, TEXT("RunGameGameMode: Player destruction scheduled with delay: %.2f seconds"), Delay);
	}
}
