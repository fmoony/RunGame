// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/RunGameInGame.h"
#include "Actor/Component/HealthComponent.h"
#include "Actor/Component/SkillComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/RunGameSkillSlot.h"
#include "Skill/RunGameSkillConfigData.h"
#include "GameFramework/PlayerController.h"
#include "RunGameCharacter.h"
#include "RunGamePlayerState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"

URunGameInGame::URunGameInGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ScoreText(nullptr)
	, TimerText(nullptr)
	, HealthBar(nullptr)
	, SkillBarContainer(nullptr)
	, SkillSlotClass(nullptr)
	, CachedSkillComponent(nullptr)
	, TimerSubsystem(nullptr)
	, CachedHealthComponent(nullptr)
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
		UE_LOG(LogTemp, Error, TEXT("ScoreText is not bound in RunGameInGame widget!"));
	}
	else
	{
		// 初始化分数显�?
		ScoreText->SetText(FText::FromString(TEXT("Score: 000000000")));
	}

	if (!TimerText)
	{
		UE_LOG(LogTemp, Error, TEXT("TimerText is not bound in RunGameInGame widget!"));
	}
	else
	{
		// 初始化计时显�?
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

		// 获取PlayerState并绑定分数变更委�?
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGamePlayerState* PlayerState = Cast<ARunGamePlayerState>(PC->GetPlayerState<APlayerState>()))
			{
				PlayerState->OnScoreChanged.AddDynamic(this, &URunGameInGame::OnScoreUpdated);

				// 立即更新UI显示当前�?
				OnScoreUpdated(PlayerState->GetRunGameScore());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get PlayerState for score binding"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get PlayerController for score binding"));
		}

		OnTimerUpdated(TimerSubsystem->GetTotalTimeSeconds());

		UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Successfully bound to TimerSubsystem events"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get TimerSubsystem!"));
	}

	// Skill bar setup
	if (SkillSlotClass && SkillBarContainer)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGameCharacter* Character = Cast<ARunGameCharacter>(PC->GetPawn()))
			{
				if (USkillComponent* SkillComp = Character->GetSkillComponent())
				{
					CachedSkillComponent = SkillComp;

					if (USkillConfigData* Config = SkillComp->SkillConfig)
					{
						for (const FSkillDefinition& SkillDef : Config->Skills)
						{
							if (!SkillDef.SkillTag.IsValid())
							{
								UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Skipping skill with invalid tag in SkillConfig: %s"), *SkillDef.SkillName.ToString());
								continue;
							}

							URunGameSkillSlot* SkillSlot = CreateWidget<URunGameSkillSlot>(this, SkillSlotClass);
							if (SkillSlot)
							{
								SkillSlot->SetupSlot(SkillDef, SkillDef.SkillTag, CachedSkillComponent);
								SkillBarContainer->AddChild(SkillSlot);
								SkillSlots.Add(SkillSlot);
								UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Created skill slot for skill '%s' with tag '%s'"), *SkillDef.SkillName.ToString(), *SkillDef.SkillTag.ToString());
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to create skill slot widget for skill '%s'"), *SkillDef.SkillName.ToString());
							}
						}

						UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Skill bar setup complete with %d skills"), SkillSlots.Num());
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("RunGameInGame: SkillComponent on PlayerPawn does not have a SkillConfig set for skill bar setup"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get SkillComponent for skill bar setup"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get PlayerPawn for skill bar setup"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("RunGameInGame: Failed to get PlayerController for skill bar setup"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameInGame: SkillSlotClass or SkillBarContainer is not set!"));
	}
}

void URunGameInGame::NativeDestruct()
{
	// 解绑HealthComponent委托
	if (CachedHealthComponent)
	{
		CachedHealthComponent->OnHealthChanged.RemoveDynamic(this, &URunGameInGame::OnHealthUpdated);
		CachedHealthComponent = nullptr;
	}

	// 解绑委托回调，防止内存泄�?
	if (TimerSubsystem)
	{
		TimerSubsystem->OnTimeChanged.RemoveDynamic(this, &URunGameInGame::OnTimerUpdated);
	
		// 解绑PlayerState的分数委�?
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (ARunGamePlayerState* PlayerState = Cast<ARunGamePlayerState>(PC->GetPlayerState<APlayerState>()))
			{
				PlayerState->OnScoreChanged.RemoveDynamic(this, &URunGameInGame::OnScoreUpdated);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Successfully unbound from TimerSubsystem events"));
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
		//UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Score updated to %d"), NewScore);
	}
}

void URunGameInGame::OnTimerUpdated(float NewTime)
{
	// 更新计时文本显示
	if (TimerText)
	{
		FString TimeString = FString::Printf(TEXT("Time: %s"), *FormatTimeText(NewTime));
		TimerText->SetText(FText::FromString(TimeString));
		//UE_LOG(LogTemp, Warning, TEXT("RunGameInGame: Time updated to %f seconds"), NewTime);
	}
}

void URunGameInGame::OnHealthUpdated(float CurrentHP, float MaxHP, float Delta)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f);
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
