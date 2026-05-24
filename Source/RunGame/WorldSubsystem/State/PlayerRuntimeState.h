#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "PlayerRuntimeState.generated.h"

class ARunGameCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPR_OnScoreChanged, int64, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPR_OnCharacterStateChanged, ERunGameCharacterState, OldState, ERunGameCharacterState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPR_OnCharacterDied, FGameplayTag, DamageType, ARunGameCharacter*, DeadCharacter);

/**
 * 玩家运行时状态 —— 分数、角色状态、速度修改器。
 * 通过 GetWorld()->GetSubsystem<UPlayerRuntimeState>() 访问。
 */
UCLASS()
class RUNGAME_API UPlayerRuntimeState : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// ---- Score ----

	UFUNCTION(BlueprintCallable, Category = "Player|Score")
	void AddScore(int64 Value);

	UFUNCTION(BlueprintCallable, Category = "Player|Score")
	void SetRunGameScore(int64 NewScore);

	UFUNCTION(BlueprintPure, Category = "Player|Score")
	int64 GetRunGameScore() const { return RunGameScore; }

	UFUNCTION(BlueprintCallable, Category = "Player|Score")
	void SetScoringActive(bool bActive);

	UFUNCTION(BlueprintPure, Category = "Player|Score")
	bool IsScoringActive() const { return bIsScoringActive; }

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnScoreChanged OnScoreChanged;

	// ---- Character State ----

	UFUNCTION(BlueprintCallable, Category = "Player|Character")
	void SetCharacterState(ERunGameCharacterState NewState);

	UFUNCTION(BlueprintPure, Category = "Player|Character")
	ERunGameCharacterState GetCharacterState() const { return CurrentCharacterState; }

	UFUNCTION(BlueprintPure, Category = "Player|Character")
	bool IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnCharacterStateChanged OnCharacterStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FPR_OnCharacterDied OnCharacterDied;

	void SetTurnFlags(bool bInTurn, bool bInBox);
	void GetTurnFlags(bool& OutTurn, bool& OutInTurnBox) const;

	void AddSpeedModifier(FGameplayTag ModifierTag, float Multiplier);
	void RemoveSpeedModifier(FGameplayTag ModifierTag);
	float GetCompositeSpeedMultiplier() const { return CachedCompositeSpeedMultiplier; }

	int32 GetSpeedModifierCount() const { return SpeedModifiers.Num(); }

	void CachePlayerCharacter(ARunGameCharacter* InCharacter);
	UFUNCTION(BlueprintPure, Category = "Player|Character")
	ARunGameCharacter* GetPlayerCharacter() const { return CachedPlayerCharacter.Get(); }

	// ---- Lifecycle ----

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	int64 RunGameScore = 0;
	bool bIsScoringActive = false;
	ERunGameCharacterState CurrentCharacterState = ERunGameCharacterState::Idle;
	bool bTurn = false;
	bool bInTurnBox = false;
	TMap<FGameplayTag, float> SpeedModifiers;
	float CachedCompositeSpeedMultiplier = 1.0f;
	TWeakObjectPtr<ARunGameCharacter> CachedPlayerCharacter;
};
