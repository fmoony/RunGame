#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameType.h"
#include "GameplayTagContainer.h"
#include "RunGameDebugPanel.generated.h"

class UTextBlock;
class UBorder;
class UPlayerRuntimeState;
class ARunGameCharacter;

/**
 * 调试面板 —— 实时展示所有运行时数据。
 * 纯 C++ 构建 UI 布局，不依赖 Blueprint 子类。
 * 由 RunGameDebugSubsystem 自动创建并显示。
 * 点击穿透，不干扰其他 UI 的交互。
 * Debug panel — real-time display of all runtime data. Click-through, no BP dependency.
 */
UCLASS()
class RUNGAME_API URunGameDebugPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void RefreshAllData();

private:
	void RefreshGameFlowData();
	void RefreshPlayerData();
	void RefreshCombatData();
	void RefreshSkillTags();

	ARunGameCharacter* GetCachedCharacter() const;

	static FString GameStateToString(ERunGameGameState State);
	static FString CharacterStateToString(ERunGameCharacterState State);
	static FString FormatTimeMMSS(float TotalSeconds);
	static FString BoolStr(bool b) { return b ? TEXT("Yes") : TEXT("No"); }

	// 委托回调 Delegate callbacks
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

	UFUNCTION()
	void OnCountdownUpdatedCallback(int32 CountdownSeconds);

	UFUNCTION()
	void OnTimeChangedCallback(float NewTime);

	UFUNCTION()
	void OnScoreChangedCallback(int64 NewScore);

	UFUNCTION()
	void OnCharacterStateChangedCallback(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UFUNCTION()
	void OnHealthChangedCallback(float CurrentHP, float MaxHP, float Delta);

	UFUNCTION()
	void OnEnergyChangedCallback(float CurrentEnergy, float MaxEnergy);

	UFUNCTION()
	void OnInvincibilityChangedCallback(bool bNewInvincible);

	UFUNCTION()
	void OnDeathCallback(FGameplayTag DamageType, AActor* DeathCauser);

	UFUNCTION()
	void OnEffectTagChangedCallback(FGameplayTag Tag, bool bAdded);

	UFUNCTION()
	void OnCollisionStateChangedCallback();

	// Widgets
	UPROPERTY()
	UTextBlock* TitleText;

	UPROPERTY()
	UTextBlock* GameFlowText;

	UPROPERTY()
	UTextBlock* PlayerText;

	UPROPERTY()
	UTextBlock* CombatText;

	UPROPERTY()
	UTextBlock* SkillTagsText;
};
