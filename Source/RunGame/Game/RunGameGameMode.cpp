// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/RunGameGameMode.h"
#include "Character/RunGameCharacter.h"
#include "Player/RunGamePlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "WorldSubsystem/RunGameFloorSubsystem.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Game/RunGameGameState.h"
#include "Character/Locomotion/Movement/RunGameMovementComponent.h"
#include "Player/RunGamePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "RunGame.h"

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

	// 监听角色死亡——响应式设置 GameOver Listen to character death — reactively set GameOver
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterDied.AddDynamic(this, &ARunGameGameMode::OnCharacterDiedCallback);
	}

	if (URunGameFloorSubsystem* FloorSystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>())
	{
		if (FloorConfig)
		{
			FloorSystem->InitializeFloorSystem(FloorConfig);
			FloorSystem->OnFloorSystemReady.AddDynamic(this, &ARunGameGameMode::OnFloorSystemReadyCallback);
		}
		else
		{
			UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: FloorConfig is not set. Skipping floor initialization."));
		}
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameGameMode: Failed to get URunGameFloorSubsystem!"));
	}
}

void ARunGameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
		{
			GS->OnGameStateChanged.RemoveDynamic(this, &ARunGameGameMode::OnGameStateChangedCallback);
		}

		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->OnCharacterDied.RemoveDynamic(this, &ARunGameGameMode::OnCharacterDiedCallback);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void ARunGameGameMode::OnFloorSystemReadyCallback()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	// 提前生成 Character — MainMenu 时就存在，CameraComponent 可切主菜单镜头
	// Spawn Character early — exists during MainMenu, CameraComponent can switch to menu camera
	if (PC && GameCharacterClass)
	{
		// 销毁 GameMode 自动生成的 Spectator Destroy auto-spawned Spectator from GameMode
		if (APawn* ExistingPawn = PC->GetPawn())
		{
			if (ExistingPawn->IsA(ASpectatorPawn::StaticClass()))
			{
				PC->UnPossess();
				ExistingPawn->Destroy();
			}
		}

		AActor* StartSpot = ChoosePlayerStart(PC);
		const FTransform SpawnTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;

		if (!PC->GetPawn())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			if (APawn* Player = GetWorld()->SpawnActor<APawn>(GameCharacterClass, SpawnTransform, SpawnParams))
			{
				PC->Possess(Player);
				Player->SetActorHiddenInGame(true);
				Player->SetActorEnableCollision(false);
				UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: Character pre-spawned in MainMenu"));
			}
		}
	}

	if (URunGameFloorSubsystem* FloorSystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>())
	{
		const FTransform StartTransform = (PC && PC->GetPawn()) ? PC->GetPawn()->GetTransform() : FTransform::Identity;

		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, [FloorSystem, StartTransform]()
		{
			FloorSystem->SpawnInitialFloors(StartTransform);
		}, 1.f, false);
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

void ARunGameGameMode::OnCharacterDiedCallback(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter)
{
	if (ARunGameGameState* GS = GetGameState<ARunGameGameState>())
	{
		GS->SetGameState(ERunGameGameState::GameOver);
	}
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
	case ERunGameGameState::CountDown:
		SpawnPlayer();
		break;
	case ERunGameGameState::InGame:
		if (OldState == ERunGameGameState::Pause || bResumingFromPause)
		{
			bResumingFromPause = false;
		}
		else
		{
			// 恢复运动 — CountDown 时 SetActive(false) 了 Resume movement — paused during CountDown
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				if (ACharacter* Char = Cast<ACharacter>(PC->GetPawn()))
				{
					Char->GetCharacterMovement()->SetActive(true);
				}
			}
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
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	// Character 已在 OnFloorSystemReady 时预生成 — 只需重启状态 + 显示
	// Character already pre-spawned in OnFloorSystemReady — just reset state + show
	AActor* StartSpot = ChoosePlayerStart(PC);
	if (!StartSpot)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameGameMode: SpawnPlayer — ChoosePlayerStart returned NULL! No PlayerStart in level!"));
	}
	const FTransform SpawnTransform = StartSpot
		? FTransform(StartSpot->GetActorRotation(), StartSpot->GetActorLocation())
		: FTransform::Identity;

	APawn* Pawn = PC->GetPawn();
	if (StartSpot) { UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: StartSpot Location=%s"), *StartSpot->GetActorLocation().ToString()); }
	UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: SpawnPlayer — Pawn=%s, StartSpot=%s"),
		*GetNameSafe(Pawn), *GetNameSafe(StartSpot));

	if (Pawn && Pawn->IsA(GameCharacterClass))
	{
		ERunGameCharacterState PRSState = ERunGameCharacterState::MAX;
		if (UPlayerRuntimeState* P = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
		{
			PRSState = P->GetCharacterState();
		}
		UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: SpawnPlayer — PRS State=%d"), (int32)PRSState);

		Pawn->SetActorTransform(SpawnTransform);
		Pawn->SetActorHiddenInGame(false);
		Pawn->SetActorEnableCollision(true);
		PC->Possess(Pawn);

		if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->ResetForNewGame();
		}

		// CountDown 期间停运动 — InGame 时恢复
		if (ACharacter* Char = Cast<ACharacter>(Pawn))
		{
			Char->GetCharacterMovement()->SetActive(false);
		}

		UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: Reused pre-spawned Character"));
		return;
	}

	// 预生成未就绪 → 回退原始生成 Pre-spawn not ready → fallback spawn
	if (!GameCharacterClass)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameGameMode: GameCharacterClass is NOT set in GameMode! Cannot spawn player."));
		return;
	}

	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->ResetForNewGame();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (APawn* RealPlayer = GetWorld()->SpawnActor<APawn>(GameCharacterClass, SpawnTransform, SpawnParams))
	{
		PC->Possess(RealPlayer);
		UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: Fallback spawn — pre-spawn was not ready"));
	}
}

// 游戏计时相关函数实现
void ARunGameGameMode::InitializeGameTimer()
{
	UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: InitializeGameTimer deprecated - timer starts reactively via GameState state change."));
}

// 重置游戏函数（完全重置到初始状态 = 回到主菜单）
void ARunGameGameMode::ResetGame()
{
	if(GetWorld()->IsPaused())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

	// 设置游戏状态为 MainMenu，触发 HUD 自动切换到主菜单 UI
	if (ARunGameGameState* CurrentGameState = GetGameState<ARunGameGameState>())
	{
		CurrentGameState->SetGameState(ERunGameGameState::MainMenu);
	}

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

		FloorSystem->SpawnInitialFloors(StartTransform);
	}

	// 输入模式由 Controller 自行监听 GameState 状态变化来管理

	UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: Game reset, all data cleared, returned to MainMenu"));
}

// 重新开始游戏（绕过主菜单，直接进入倒计时）
void ARunGameGameMode::StartNewGame()
{
	bResumingFromPause = false;

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

		FloorSystem->SpawnInitialFloors(StartTransform);
	}

	StartGameCountDown();

	UE_LOG(LogRunGame, Warning, TEXT("RunGameGameMode: New game started directly"));
}

void ARunGameGameMode::RequestRestartFromPause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// 不再销毁角色 — CameraComponent 需要角色存活以管理镜头
	// Don't destroy pawn — CameraComponent needs Character alive for camera management
	StartNewGame();
}
