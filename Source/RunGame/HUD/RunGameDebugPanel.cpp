#include "HUD/RunGameDebugPanel.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Game/RunGameGameState.h"
#include "Player/RunGamePlayerState.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameMovementComponent.h"
#include "Actor/Component/HealthComponent.h"
#include "Skill/SkillComponent.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Fonts/SlateFontInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "RunGame.h"

bool URunGameDebugPanel::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"));
	}

	// 根控件：Overlay 自动填充视口，SelfHitTestInvisible 穿透点击
	UOverlay* RootOverlay = NewObject<UOverlay>(this);
	RootOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	WidgetTree->RootWidget = RootOverlay;

	// 半透明黑色背景 + 文字区域的容器
	UBorder* Border = NewObject<UBorder>(this);
	Border->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
	Border->SetPadding(FMargin(12.0f, 8.0f));

	UOverlaySlot* OverlaySlot = RootOverlay->AddChildToOverlay(Border);
	OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
	OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);

	UVerticalBox* VBox = NewObject<UVerticalBox>(this);
	Border->SetContent(VBox);

	// 标题 — 大号青色粗体 Title — large cyan bold
	FSlateFontInfo TitleFont = FAppStyle::Get().GetFontStyle("NormalFont");;
	TitleFont.Size = 28;
	TitleText = NewObject<UTextBlock>(this);
	TitleText->SetText(FText::FromString(TEXT("DEBUG")));
	TitleText->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 1.0f, 1.0f)));
	TitleText->SetFont(TitleFont);
	VBox->AddChildToVerticalBox(TitleText);

	// GameFlow 区域
	FSlateFontInfo ContentFont = FAppStyle::Get().GetFontStyle("NormalFont");;
	ContentFont.Size = 18;
	GameFlowText = NewObject<UTextBlock>(this);
	GameFlowText->SetColorAndOpacity(FSlateColor(FLinearColor(0.8f, 0.85f, 1.0f, 1.0f)));
	GameFlowText->SetFont(ContentFont);
	VBox->AddChildToVerticalBox(GameFlowText);

	// Player 区域
	PlayerText = NewObject<UTextBlock>(this);
	PlayerText->SetColorAndOpacity(FSlateColor(FLinearColor(0.8f, 1.0f, 0.8f, 1.0f)));
	PlayerText->SetFont(ContentFont);
	VBox->AddChildToVerticalBox(PlayerText);

	// Combat 区域
	CombatText = NewObject<UTextBlock>(this);
	CombatText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.8f, 0.8f, 1.0f)));
	CombatText->SetFont(ContentFont);
	VBox->AddChildToVerticalBox(CombatText);

	return true;
}

void URunGameDebugPanel::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (!World) return;

	// 绑定 GameState 委托 Bind GameState delegates
	if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &URunGameDebugPanel::OnGameStateChangedCallback);
		GS->OnCountdownUpdated.AddDynamic(this, &URunGameDebugPanel::OnCountdownUpdatedCallback);
	}

	// 绑定 TimerSubsystem 委托 Bind TimerSubsystem delegates
	if (URunGameTimerSubsystem* TS = World->GetSubsystem<URunGameTimerSubsystem>())
	{
		TS->OnTimeChanged.AddDynamic(this, &URunGameDebugPanel::OnTimeChangedCallback);
	}

	// 绑定 PlayerRuntimeState 委托（角色状态） Bind PlayerRuntimeState (character state only)
	if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddDynamic(this, &URunGameDebugPanel::OnCharacterStateChangedCallback);
	}

	// 角色委托绑定推迟到 OnGameStateChanged(InGame) —— NativeConstruct 时角色尚未生成
	// Character delegate binding deferred to OnGameStateChanged(InGame) — character not spawned yet

	// 绑定 PlayerState 委托（分数） Bind PlayerState (score)
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ARunGamePlayerState* PS = PC->GetPlayerState<ARunGamePlayerState>())
		{
			PS->OnScoreChanged.AddDynamic(this, &URunGameDebugPanel::OnScoreChangedCallback);
		}
	}

	RefreshAllData();
}

void URunGameDebugPanel::NativeDestruct()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		Super::NativeDestruct();
		return;
	}

	if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.RemoveDynamic(this, &URunGameDebugPanel::OnGameStateChangedCallback);
		GS->OnCountdownUpdated.RemoveDynamic(this, &URunGameDebugPanel::OnCountdownUpdatedCallback);
	}

	if (URunGameTimerSubsystem* TS = World->GetSubsystem<URunGameTimerSubsystem>())
	{
		TS->OnTimeChanged.RemoveDynamic(this, &URunGameDebugPanel::OnTimeChangedCallback);
	}

	if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.RemoveDynamic(this, &URunGameDebugPanel::OnCharacterStateChangedCallback);
	}

	if (ARunGameCharacter* Char = GetCachedCharacter())
	{
		if (UHealthComponent* HC = Char->GetHealthComponent())
		{
			HC->OnHealthChanged.RemoveDynamic(this, &URunGameDebugPanel::OnHealthChangedCallback);
			HC->OnDeath.RemoveDynamic(this, &URunGameDebugPanel::OnDeathCallback);
			HC->OnInvincibilityChanged.RemoveDynamic(this, &URunGameDebugPanel::OnInvincibilityChangedCallback);
		}

		if (USkillComponent* SC = Char->GetSkillComponent())
		{
			SC->OnEnergyChanged.RemoveDynamic(this, &URunGameDebugPanel::OnEnergyChangedCallback);
		}
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ARunGamePlayerState* PS = PC->GetPlayerState<ARunGamePlayerState>())
		{
			PS->OnScoreChanged.RemoveDynamic(this, &URunGameDebugPanel::OnScoreChangedCallback);
		}
	}

	Super::NativeDestruct();
}

// ---- Helpers ----

ARunGameCharacter* URunGameDebugPanel::GetCachedCharacter() const
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			return Cast<ARunGameCharacter>(PC->GetPawn());
		}
		else
		{
			UE_LOG(LogRunGame, Error, TEXT("RunGameDebugPanel: Failed to get PlayerController in GetCachedCharacter"));
		}
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameDebugPanel: Failed to get World in GetCachedCharacter"));
	}
	return nullptr;
}

// ---- Delegate callbacks ----

void URunGameDebugPanel::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	// 进入游戏时角色已生成，绑定其组件委托 Bind character component delegates when game starts
	if (NewState == ERunGameGameState::InGame)
	{
		if (ARunGameCharacter* Char = GetCachedCharacter())
		{
			if (UHealthComponent* HC = Char->GetHealthComponent())
			{
				HC->OnHealthChanged.AddDynamic(this, &URunGameDebugPanel::OnHealthChangedCallback);
				HC->OnDeath.AddDynamic(this, &URunGameDebugPanel::OnDeathCallback);
				HC->OnInvincibilityChanged.AddDynamic(this, &URunGameDebugPanel::OnInvincibilityChangedCallback);
			}

			if (USkillComponent* SC = Char->GetSkillComponent())
			{
				SC->OnEnergyChanged.AddDynamic(this, &URunGameDebugPanel::OnEnergyChangedCallback);
			}
		}
		else
		{
			UE_LOG(LogRunGame, Error, TEXT("RunGameDebugPanel: Failed to get character on GameState InGame"));
		}
	}

	// 游戏结束时解绑旧角色委托 Unbind old character delegates when game ends
	if (NewState == ERunGameGameState::GameOver)
	{
		if (ARunGameCharacter* Char = GetCachedCharacter())
		{
			if (UHealthComponent* HC = Char->GetHealthComponent())
			{
				HC->OnHealthChanged.RemoveDynamic(this, &URunGameDebugPanel::OnHealthChangedCallback);
				HC->OnDeath.RemoveDynamic(this, &URunGameDebugPanel::OnDeathCallback);
				HC->OnInvincibilityChanged.RemoveDynamic(this, &URunGameDebugPanel::OnInvincibilityChangedCallback);
			}

			if (USkillComponent* SC = Char->GetSkillComponent())
			{
				SC->OnEnergyChanged.RemoveDynamic(this, &URunGameDebugPanel::OnEnergyChangedCallback);
			}
		}
	}

	RefreshAllData();
}

void URunGameDebugPanel::OnCountdownUpdatedCallback(int32 CountdownSeconds)
{
	RefreshGameFlowData();
}

void URunGameDebugPanel::OnTimeChangedCallback(float NewTime)
{
	RefreshGameFlowData();
}

void URunGameDebugPanel::OnScoreChangedCallback(int64 NewScore)
{
	RefreshPlayerData();
}

void URunGameDebugPanel::OnCharacterStateChangedCallback(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	RefreshPlayerData();
}

void URunGameDebugPanel::OnHealthChangedCallback(float CurrentHP, float MaxHP, float Delta)
{
	RefreshCombatData();
}

void URunGameDebugPanel::OnEnergyChangedCallback(float CurrentEnergy, float MaxEnergy)
{
	RefreshCombatData();
}

void URunGameDebugPanel::OnInvincibilityChangedCallback(bool bNewInvincible)
{
	RefreshCombatData();
}

void URunGameDebugPanel::OnDeathCallback(FGameplayTag DamageType, AActor* DeathCauser)
{
	RefreshCombatData();
}

// ---- Refresh methods ----

void URunGameDebugPanel::RefreshAllData()
{
	RefreshGameFlowData();
	RefreshPlayerData();
	RefreshCombatData();
}

void URunGameDebugPanel::RefreshGameFlowData()
{
	if (!GameFlowText) return;

	UWorld* World = GetWorld();
	if (!World)
	{
		GameFlowText->SetText(FText::FromString(TEXT("[GameFlow] No World")));
		return;
	}

	ARunGameGameState* GS = World->GetGameState<ARunGameGameState>();
	URunGameTimerSubsystem* TS = World->GetSubsystem<URunGameTimerSubsystem>();

	const FString Text = FString::Printf(
		TEXT("[Game Flow]\n")
		TEXT("  GameState : %s\n")
		TEXT("  Countdown : %d s\n")
		TEXT("  TotalTime : %s  (%.1f s)\n")
		TEXT("  TimerRunning : %s"),
		GS ? *GameStateToString(GS->GetCurrentState()) : TEXT("?"),
		GS ? GS->GetCountdownSeconds() : 0,
		TS ? *FormatTimeMMSS(TS->GetTotalTimeSeconds()) : TEXT("?"),
		TS ? TS->GetTotalTimeSeconds() : 0.0f,
		TS ? *BoolStr(TS->IsTimerRunning()) : TEXT("?")
	);

	GameFlowText->SetText(FText::FromString(Text));
}

void URunGameDebugPanel::RefreshPlayerData()
{
	if (!PlayerText) return;

	UWorld* World = GetWorld();
	if (!World)
	{
		PlayerText->SetText(FText::FromString(TEXT("[Player] No World")));
		return;
	}

	UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>();
	ARunGameCharacter* Char = GetCachedCharacter();

	int64 Score = 0;
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ARunGamePlayerState* PS = PC->GetPlayerState<ARunGamePlayerState>())
		{
			Score = PS->GetRunGameScore();
		}
	}

	URunGameMovementComponent* MoveComp = Char ? Char->GetRunGameMovementComponent() : nullptr;
	bool bTurn = MoveComp ? MoveComp->IsInTurn() : false;
	bool bInTurnBox = MoveComp ? MoveComp->IsInTurnBox() : false;
	float SpeedMulti = MoveComp ? MoveComp->GetCompositeSpeedMultiplier() : 1.0f;

	const FString Text = FString::Printf(
		TEXT("[Player]\n")
		TEXT("  Score        : %lld\n")
		TEXT("  CharState    : %s\n")
		TEXT("  Turn         : %s\n")
		TEXT("  InTurnBox    : %s\n")
		TEXT("  SpeedMulti   : %.2f"),
		Score,
		PRS ? *CharacterStateToString(PRS->GetCharacterState()) : TEXT("?"),
		*BoolStr(bTurn),
		*BoolStr(bInTurnBox),
		SpeedMulti
	);

	PlayerText->SetText(FText::FromString(Text));
}

void URunGameDebugPanel::RefreshCombatData()
{
	if (!CombatText) return;

	ARunGameCharacter* Char = GetCachedCharacter();
	if (!Char)
	{
		CombatText->SetText(FText::FromString(TEXT("[Combat] No Character")));
		return;
	}

	UHealthComponent* HC = Char->GetHealthComponent();
	USkillComponent* SC = Char->GetSkillComponent();

	const FString Text = FString::Printf(
		TEXT("[Combat]\n")
		TEXT("  HP         : %.1f / %.1f  (%.0f%%)\n")
		TEXT("  Dead       : %s\n")
		TEXT("  Invincible : %s\n")
		TEXT("  Energy     : %.1f / %.1f"),
		HC ? HC->GetCurrentHP() : 0.0f,
		HC ? HC->GetMaxHP() : 0.0f,
		HC ? HC->GetHealthPercentage() * 100.0f : 0.0f,
		HC ? *BoolStr(HC->IsDead()) : TEXT("?"),
		HC ? *BoolStr(HC->IsInvincible()) : TEXT("?"),
		SC ? SC->GetCurrentEnergy() : 0.0f,
		SC ? SC->MaxEnergy : 0.0f
	);

	CombatText->SetText(FText::FromString(Text));
}

// ---- Static formatters ----

FString URunGameDebugPanel::GameStateToString(ERunGameGameState State)
{
	switch (State)
	{
	case ERunGameGameState::MainMenu:  return TEXT("MainMenu");
	case ERunGameGameState::CountDown: return TEXT("CountDown");
	case ERunGameGameState::InGame:    return TEXT("InGame");
	case ERunGameGameState::Pause:     return TEXT("Pause");
	case ERunGameGameState::GameOver:  return TEXT("GameOver");
	default:                           return TEXT("???");
	}
}

FString URunGameDebugPanel::CharacterStateToString(ERunGameCharacterState State)
{
	switch (State)
	{
	case ERunGameCharacterState::Idle:       return TEXT("Idle");
	case ERunGameCharacterState::CoyoteTime: return TEXT("CoyoteTime");
	case ERunGameCharacterState::Airborne:   return TEXT("Airborne");
	case ERunGameCharacterState::Sliding:    return TEXT("Sliding");
	case ERunGameCharacterState::Turning:    return TEXT("Turning");
	case ERunGameCharacterState::Dead:       return TEXT("Dead");
	default:                               return TEXT("???");
	}
}

FString URunGameDebugPanel::FormatTimeMMSS(float TotalSeconds)
{
	const int32 Total = FMath::Max(0, FMath::FloorToInt(TotalSeconds * 100.0f));
	const int32 Minutes = Total / 6000;
	const int32 Seconds = (Total % 6000) / 100;
	const int32 Centis = Total % 100;
	return FString::Printf(TEXT("%02d:%02d.%02d"), Minutes, Seconds, Centis);
}
