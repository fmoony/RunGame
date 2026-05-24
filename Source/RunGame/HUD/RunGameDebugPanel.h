#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameType.h"
#include "RunGameDebugPanel.generated.h"

class UTextBlock;
class UBorder;
class UGameFlowRuntimeState;
class UPlayerRuntimeState;
class UCombatRuntimeState;

/**
 * 调试面板 —— 实时展示三个 RuntimeState 的完整数据。
 * 纯 C++ 构建 UI 布局，不依赖 Blueprint 子类。
 * 由 RunGameDebugSubsystem 自动创建并显示。
 * 点击穿透，不干扰其他 UI 的交互。
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

	UGameFlowRuntimeState* GetGameFlowRS() const;
	UPlayerRuntimeState* GetPlayerRS() const;
	UCombatRuntimeState* GetCombatRS() const;

	static FString GameStateToString(ERunGameGameState State);
	static FString CharacterStateToString(ERunGameCharacterState State);
	static FString FormatTimeMMSS(float TotalSeconds);
	static FString BoolStr(bool b) { return b ? TEXT("Yes") : TEXT("No"); }

	// 委托回调 —— AddDynamic 要求 UFUNCTION
	UFUNCTION()
	void OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	UFUNCTION()
	void OnCountdownUpdated(int32 CountdownSeconds);

	UFUNCTION()
	void OnTimeChanged(float NewTime);

	UFUNCTION()
	void OnScoreChanged(int64 NewScore);

	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UFUNCTION()
	void OnHealthChanged(float CurrentHP, float MaxHP, float Delta);

	UFUNCTION()
	void OnEnergyChanged(float CurrentEnergy, float MaxEnergy);

	UFUNCTION()
	void OnInvincibilityChanged(bool bNewInvincible);

	UFUNCTION()
	void OnDeath(FGameplayTag DamageType, AActor* DeathCauser);

	// Widgets
	UPROPERTY()
	UTextBlock* TitleText;

	UPROPERTY()
	UTextBlock* GameFlowText;

	UPROPERTY()
	UTextBlock* PlayerText;

	UPROPERTY()
	UTextBlock* CombatText;
};
