// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameInGame.generated.h"

class UHorizontalBox;
class UProgressBar;
class URunGameTimerSubsystem;
class URunGameSkillSlot;
class USkillComponent;
class UTextBlock;
class UHealthComponent;

/**
 * 游戏中UI - 显示实时分数和计时
 */
UCLASS()
class RUNGAME_API URunGameInGame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Constructs the InGame widget and initializes bound widget references */
	URunGameInGame(const FObjectInitializer& ObjectInitializer);

protected:
	/** Native lifecycle functions */
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	// UI组件 - 使用BindWidget元数据绑定到蓝图
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> EnergyBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SkillBarContainer;

	/** Widget class used for each skill slot — assign in Blueprint */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Bar")
	TSubclassOf<URunGameSkillSlot> SkillSlotClass;

	/** Dynamically created skill slot widgets */
	UPROPERTY()
	TArray<TObjectPtr<URunGameSkillSlot>> SkillSlots;

	/** Cached SkillComponent reference for cleanup */
	UPROPERTY()
	TObjectPtr<USkillComponent> CachedSkillComponent;

	/** Timer subsystem reference for time display updates */
	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

	/** Cached HealthComponent reference for delegate unbinding */
	UPROPERTY()
	TObjectPtr<UHealthComponent> CachedHealthComponent;

	/** Updates the score text display when player score changes */
	UFUNCTION()
	void OnScoreUpdated(int64 NewScore);

	/** Updates the timer text display when game time changes */
	UFUNCTION()
	void OnTimerUpdated(float NewTime);

	/** Updates the health bar display when player health changes */
	UFUNCTION()
	void OnHealthUpdated(float CurrentHP, float MaxHP, float Delta);

	/** Updates the energy bar display when skill energy changes */
	UFUNCTION()
	void OnEnergyUpdated(float CurrentEnergy, float MaxEnergy);

	/** Formats time in seconds to a MM:SS.ms string */
	FString FormatTimeText(float TimeSeconds) const;
};
