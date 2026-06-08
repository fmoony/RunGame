// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/RunGameInGame.h"
#include "Actor/Component/HealthComponent.h"
#include "Skill/SkillComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/RunGameSkillSlot.h"
#include "Skill/RunGameSkillConfigData.h"
#include "GameFramework/PlayerController.h"
#include "Character/RunGameCharacter.h"
#include "Player/RunGamePlayerState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "RunGame.h"

URunGameInGame::URunGameInGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ScoreText(nullptr)
	, TimerText(nullptr)
	, HealthBar(nullptr)
	, EnergyBar(nullptr)
	, SkillBarContainer(nullptr)
	, SkillSlotClass(nullptr)
	, CachedSkillComponent(nullptr)
	, TimerSubsystem(nullptr)
	, CachedHealthComponent(nullptr)
	, SkillBarRetryCount(0)
{
}

void URunGameInGame::NativeConstruct()
{
	Super::NativeConstruct();

	// 获取TimerSubsystem引用
	if (UWorld* World = GetWorld())
	{
		TimerSubsystem = World->GetSubsystem<URunGameTimerSubsystem>();
	}

	// 检查UI组件是否正确绑定
	if (!ScoreText)
	{
		UE_LOG(LogRunGame, Error, TEXT("ScoreText is not bound in RunGameInGame widget!"));
	}
	else
	{
		// 初始化分数显示
		ScoreText->SetText(FText::FromString(TEXT("Score: 000000000")));
	}

	if (!TimerText)
	{
		UE_LOG(LogRunGame, Error, TEXT("TimerText is not bound in RunGameInGame widget!"));
	}
	else
	{
		// 初始化计时显示
		TimerText->SetText(FText::FromString(TEXT("Time: 0.000s")));
	}

	// 绑定HealthComponent委托
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ARunGameCharacter* Character = Cast<ARunGameCharacter>(PC->GetPawn()))
		{
			if (UHealthComponent* HealthComp = Character->GetHealthComponent())
			{
				CachedHealthComponent = HealthComp;
				HealthComp->OnHealthChanged.AddDynamic(this, &URunGameInGame::OnHealthUpdated);
				OnHealthUpdated(HealthComp->GetCurrentHP(), HealthComp->GetMaxHP(), 0.0f);
			}
		}
	}

	// 绑定委托回调
	if (TimerSubsystem)
	{
		// 绑定时间变更委托
		TimerSubsystem->OnTimeChanged.AddDynamic(this, &URunGameInGame::OnTimerUpdated);

		// 获取PlayerState并绑定分数变更委托
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGamePlayerState* PlayerState = Cast<ARunGamePlayerState>(PC->GetPlayerState<APlayerState>()))
			{
				PlayerState->OnScoreChanged.AddDynamic(this, &URunGameInGame::OnScoreUpdated);

				// 立即更新UI显示当前值
				OnScoreUpdated(PlayerState->GetRunGameScore());
			}
			else
			{
				UE_LOG(LogRunGame, Error, TEXT("RunGameInGame: Failed to get PlayerState for score binding"));
			}
		}
		else
		{
			UE_LOG(LogRunGame, Error, TEXT("RunGameInGame: Failed to get PlayerController for score binding"));
		}

		OnTimerUpdated(TimerSubsystem->GetTotalTimeSeconds());

		UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Successfully bound to TimerSubsystem events"));
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameInGame: Failed to get TimerSubsystem!"));
	}

	// 延迟技能栏构建到下一帧 — 先让世界跑起来，避免解冻首帧 Widget 创建阻塞
	// Defer skill bar to next frame — let the world start first, avoid blocking first frame after unfreeze
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &URunGameInGame::SetupSkillBar);
}

void URunGameInGame::SetupSkillBar()
{
	if (!SkillSlotClass || !SkillBarContainer)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameInGame: SkillSlotClass or SkillBarContainer is not set!"));
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: No PlayerController yet — will retry skill bar setup"));
		ScheduleSkillBarRetry();
		return;
	}

	ARunGameCharacter* Character = Cast<ARunGameCharacter>(PC->GetPawn());
	if (!Character)
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: No PlayerPawn yet — will retry skill bar setup"));
		ScheduleSkillBarRetry();
		return;
	}

	USkillComponent* SkillComp = Character->GetSkillComponent();
	if (!SkillComp)
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: No SkillComponent yet — will retry skill bar setup"));
		ScheduleSkillBarRetry();
		return;
	}

	// 成功——设置技能栏
	CachedSkillComponent = SkillComp;
	SkillComp->OnEnergyChanged.AddDynamic(this, &URunGameInGame::OnEnergyUpdated);
	OnEnergyUpdated(SkillComp->GetCurrentEnergy(), SkillComp->MaxEnergy);

	if (USkillConfigData* Config = SkillComp->SkillConfig)
	{
		for (const FSkillDefinition& SkillDef : Config->Skills)
		{
			if (!SkillDef.SkillTag.IsValid())
			{
				UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Skipping skill with invalid tag in SkillConfig: %s"), *SkillDef.SkillName.ToString());
				continue;
			}

			URunGameSkillSlot* SkillSlot = CreateWidget<URunGameSkillSlot>(this, SkillSlotClass);
			if (SkillSlot)
			{
				SkillSlot->SetupSlot(SkillDef, SkillDef.SkillTag, CachedSkillComponent);
				SkillBarContainer->AddChild(SkillSlot);
				SkillSlots.Add(SkillSlot);
				UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Created skill slot for skill '%s' with tag '%s'"), *SkillDef.SkillName.ToString(), *SkillDef.SkillTag.ToString());
			}
			else
			{
				UE_LOG(LogRunGame, Error, TEXT("RunGameInGame: Failed to create skill slot widget for skill '%s'"), *SkillDef.SkillName.ToString());
			}
		}

		UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Skill bar setup complete with %d skills"), SkillSlots.Num());
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameInGame: SkillComponent on PlayerPawn does not have a SkillConfig set for skill bar setup"));
	}
}

void URunGameInGame::ScheduleSkillBarRetry()
{
	if (SkillBarRetryCount < MaxSkillBarRetries)
	{
		SkillBarRetryCount++;
		UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Retrying skill bar setup (attempt %d/%d)"), SkillBarRetryCount, MaxSkillBarRetries);
		GetWorld()->GetTimerManager().SetTimer(SkillBarRetryTimer, this, &URunGameInGame::SetupSkillBar, 0.1f, false);
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameInGame: Skill bar setup failed after %d retries"), MaxSkillBarRetries);
	}
}

void URunGameInGame::NativeDestruct()
{
	// 清理技能栏重试定时器 Clear skill bar retry timer
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SkillBarRetryTimer);
	}

	// 解绑HealthComponent委托
	if (CachedHealthComponent)
	{
		CachedHealthComponent->OnHealthChanged.RemoveDynamic(this, &URunGameInGame::OnHealthUpdated);
		CachedHealthComponent = nullptr;
	}

	// 解绑委托回调，防止内存泄漏
	if (TimerSubsystem)
	{
		TimerSubsystem->OnTimeChanged.RemoveDynamic(this, &URunGameInGame::OnTimerUpdated);

		// 解绑PlayerState的分数委托
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGamePlayerState* PlayerState = Cast<ARunGamePlayerState>(PC->GetPlayerState<APlayerState>()))
			{
				PlayerState->OnScoreChanged.RemoveDynamic(this, &URunGameInGame::OnScoreUpdated);
			}
		}

		UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Successfully unbound from TimerSubsystem events"));
	}

	// 解绑能量委托
	if (CachedSkillComponent)
	{
		CachedSkillComponent->OnEnergyChanged.RemoveDynamic(this, &URunGameInGame::OnEnergyUpdated);
	}

	SkillSlots.Empty();
	CachedSkillComponent = nullptr;

	Super::NativeDestruct();
}

void URunGameInGame::OnScoreUpdated(int64 NewScore)
{
	// 更新分数文本显示
	if (ScoreText)
	{
		FString ScoreString = FString::Printf(TEXT("Score: %09d"), NewScore);
		ScoreText->SetText(FText::FromString(ScoreString));
		//UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Score updated to %d"), NewScore);
	}
}

void URunGameInGame::OnTimerUpdated(float NewTime)
{
	// 更新计时文本显示
	if (TimerText)
	{
		FString TimeString = FString::Printf(TEXT("Time: %s"), *FormatTimeText(NewTime));
		TimerText->SetText(FText::FromString(TimeString));
		//UE_LOG(LogRunGame, Warning, TEXT("RunGameInGame: Time updated to %f seconds"), NewTime);
	}
}

void URunGameInGame::OnHealthUpdated(float CurrentHP, float MaxHP, float Delta)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f);
	}
}

void URunGameInGame::OnEnergyUpdated(float CurrentEnergy, float MaxEnergy)
{
	if (EnergyBar)
	{
		EnergyBar->SetPercent(MaxEnergy > 0.0f ? CurrentEnergy / MaxEnergy : 0.0f);
	}
}

FString URunGameInGame::FormatTimeText(float TimeSeconds) const
{
	// 格式化时间显示为 MM:SS.ms 格式
	int32 Minutes = FMath::FloorToInt(TimeSeconds / 60.0f);
	int32 Seconds = FMath::FloorToInt(TimeSeconds) % 60;
	int32 Milliseconds = FMath::FloorToInt((TimeSeconds - FMath::FloorToInt(TimeSeconds)) * 1000.0f);

	return FString::Printf(TEXT("%02d:%02d.%03d"), Minutes, Seconds, Milliseconds);
}
