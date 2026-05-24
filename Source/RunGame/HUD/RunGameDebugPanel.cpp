#include "HUD/RunGameDebugPanel.h"
#include "WorldSubsystem/State/GameFlowRuntimeState.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "WorldSubsystem/State/CombatRuntimeState.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Fonts/SlateFontInfo.h"
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

	// 标题 — 大号青色粗体
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

	// 绑定三个 RuntimeState 的委托，实时刷新
	if (UGameFlowRuntimeState* GRS = GetGameFlowRS())
	{
		GRS->OnGameStateChanged.AddDynamic(this, &URunGameDebugPanel::OnGameStateChanged);
		GRS->OnCountdownUpdated.AddDynamic(this, &URunGameDebugPanel::OnCountdownUpdated);
		GRS->OnTimeChanged.AddDynamic(this, &URunGameDebugPanel::OnTimeChanged);
	}

	if (UPlayerRuntimeState* PRS = GetPlayerRS())
	{
		PRS->OnScoreChanged.AddDynamic(this, &URunGameDebugPanel::OnScoreChanged);
		PRS->OnCharacterStateChanged.AddDynamic(this, &URunGameDebugPanel::OnCharacterStateChanged);
	}

	if (UCombatRuntimeState* CRS = GetCombatRS())
	{
		CRS->OnHealthChanged.AddDynamic(this, &URunGameDebugPanel::OnHealthChanged);
		CRS->OnEnergyChanged.AddDynamic(this, &URunGameDebugPanel::OnEnergyChanged);
		CRS->OnInvincibilityChanged.AddDynamic(this, &URunGameDebugPanel::OnInvincibilityChanged);
		CRS->OnDeath.AddDynamic(this, &URunGameDebugPanel::OnDeath);
	}

	RefreshAllData();
}

void URunGameDebugPanel::NativeDestruct()
{
	if (UGameFlowRuntimeState* GRS = GetGameFlowRS())
	{
		GRS->OnGameStateChanged.RemoveDynamic(this, &URunGameDebugPanel::OnGameStateChanged);
		GRS->OnCountdownUpdated.RemoveDynamic(this, &URunGameDebugPanel::OnCountdownUpdated);
		GRS->OnTimeChanged.RemoveDynamic(this, &URunGameDebugPanel::OnTimeChanged);
	}

	if (UPlayerRuntimeState* PRS = GetPlayerRS())
	{
		PRS->OnScoreChanged.RemoveDynamic(this, &URunGameDebugPanel::OnScoreChanged);
		PRS->OnCharacterStateChanged.RemoveDynamic(this, &URunGameDebugPanel::OnCharacterStateChanged);
	}

	if (UCombatRuntimeState* CRS = GetCombatRS())
	{
		CRS->OnHealthChanged.RemoveDynamic(this, &URunGameDebugPanel::OnHealthChanged);
		CRS->OnEnergyChanged.RemoveDynamic(this, &URunGameDebugPanel::OnEnergyChanged);
		CRS->OnInvincibilityChanged.RemoveDynamic(this, &URunGameDebugPanel::OnInvincibilityChanged);
		CRS->OnDeath.RemoveDynamic(this, &URunGameDebugPanel::OnDeath);
	}

	Super::NativeDestruct();
}

// ---- 委托回调 ----

void URunGameDebugPanel::OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState)
{
	RefreshAllData();
}

void URunGameDebugPanel::OnCountdownUpdated(int32 CountdownSeconds)
{
	RefreshGameFlowData();
}

void URunGameDebugPanel::OnTimeChanged(float NewTime)
{
	RefreshGameFlowData();
}

void URunGameDebugPanel::OnScoreChanged(int64 NewScore)
{
	RefreshPlayerData();
}

void URunGameDebugPanel::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	RefreshPlayerData();
}

void URunGameDebugPanel::OnHealthChanged(float CurrentHP, float MaxHP, float Delta)
{
	RefreshCombatData();
}

void URunGameDebugPanel::OnEnergyChanged(float CurrentEnergy, float MaxEnergy)
{
	RefreshCombatData();
}

void URunGameDebugPanel::OnInvincibilityChanged(bool bNewInvincible)
{
	RefreshCombatData();
}

void URunGameDebugPanel::OnDeath(FGameplayTag DamageType, AActor* DeathCauser)
{
	RefreshCombatData();
}

// ---- 辅助 ----

UGameFlowRuntimeState* URunGameDebugPanel::GetGameFlowRS() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetSubsystem<UGameFlowRuntimeState>();
	}
	return nullptr;
}

UPlayerRuntimeState* URunGameDebugPanel::GetPlayerRS() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetSubsystem<UPlayerRuntimeState>();
	}
	return nullptr;
}

UCombatRuntimeState* URunGameDebugPanel::GetCombatRS() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetSubsystem<UCombatRuntimeState>();
	}
	return nullptr;
}

// ---- 刷新器 ----

void URunGameDebugPanel::RefreshAllData()
{
	RefreshGameFlowData();
	RefreshPlayerData();
	RefreshCombatData();
}

void URunGameDebugPanel::RefreshGameFlowData()
{
	if (!GameFlowText) return;

	UGameFlowRuntimeState* GRS = GetGameFlowRS();
	if (!GRS)
	{
		GameFlowText->SetText(FText::FromString(TEXT("[GameFlow] No RuntimeState")));
		return;
	}

	const FString Text = FString::Printf(
		TEXT("[Game Flow]\n")
		TEXT("  GameState : %s\n")
		TEXT("  Countdown : %d s\n")
		TEXT("  TotalTime : %s  (%.1f s)\n")
		TEXT("  TimerRunning : %s"),
		*GameStateToString(GRS->GetGameState()),
		GRS->GetCountdownSeconds(),
		*FormatTimeMMSS(GRS->GetTotalTimeSeconds()),
		GRS->GetTotalTimeSeconds(),
		*BoolStr(GRS->IsTimerRunning())
	);

	GameFlowText->SetText(FText::FromString(Text));
}

void URunGameDebugPanel::RefreshPlayerData()
{
	if (!PlayerText) return;

	UPlayerRuntimeState* PRS = GetPlayerRS();
	if (!PRS)
	{
		PlayerText->SetText(FText::FromString(TEXT("[Player] No RuntimeState")));
		return;
	}

	bool bTurn, bInTurnBox;
	PRS->GetTurnFlags(bTurn, bInTurnBox);

	const FString Text = FString::Printf(
		TEXT("[Player]\n")
		TEXT("  Score        : %lld\n")
		TEXT("  ScoringActive: %s\n")
		TEXT("  CharState    : %s\n")
		TEXT("  Turn         : %s\n")
		TEXT("  InTurnBox    : %s\n")
		TEXT("  SpeedMulti   : %.2f\n")
		TEXT("  SpeedMods    : %d"),
		PRS->GetRunGameScore(),
		*BoolStr(PRS->IsScoringActive()),
		*CharacterStateToString(PRS->GetCharacterState()),
		*BoolStr(bTurn),
		*BoolStr(bInTurnBox),
		PRS->GetCompositeSpeedMultiplier(),
		PRS->GetSpeedModifierCount()
	);

	PlayerText->SetText(FText::FromString(Text));
}

void URunGameDebugPanel::RefreshCombatData()
{
	if (!CombatText) return;

	UCombatRuntimeState* CRS = GetCombatRS();
	if (!CRS)
	{
		CombatText->SetText(FText::FromString(TEXT("[Combat] No RuntimeState")));
		return;
	}

	const FString Text = FString::Printf(
		TEXT("[Combat]\n")
		TEXT("  HP         : %.1f / %.1f  (%.0f%%)\n")
		TEXT("  Dead       : %s\n")
		TEXT("  Invincible : %s\n")
		TEXT("  Energy     : %.1f / %.1f"),
		CRS->GetCurrentHP(),
		CRS->GetMaxHP(),
		CRS->GetHealthPercentage() * 100.0f,
		*BoolStr(CRS->IsDead()),
		*BoolStr(CRS->IsInvincible()),
		CRS->GetCurrentEnergy(),
		CRS->GetMaxEnergy()
	);

	CombatText->SetText(FText::FromString(Text));
}

// ---- 静态格式化 ----

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
	case ERunGameCharacterState::Idle:     return TEXT("Idle");
	case ERunGameCharacterState::Airborne: return TEXT("Airborne");
	case ERunGameCharacterState::Sliding:  return TEXT("Sliding");
	case ERunGameCharacterState::Turning:  return TEXT("Turning");
	case ERunGameCharacterState::Dead:     return TEXT("Dead");
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
