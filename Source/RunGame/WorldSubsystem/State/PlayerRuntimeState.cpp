#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "RunGameCharacter.h"
#include "RunGame.h"

void UPlayerRuntimeState::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPlayerRuntimeState::Deinitialize()
{
	CachedPlayerCharacter.Reset();
	Super::Deinitialize();
}

// ---- Score ----

void UPlayerRuntimeState::AddScore(int64 Value)
{
	if (Value != 0)
	{
		RunGameScore += Value;
		OnScoreChanged.Broadcast(RunGameScore);
	}
}

void UPlayerRuntimeState::SetRunGameScore(int64 NewScore)
{
	if (RunGameScore != NewScore)
	{
		RunGameScore = NewScore;
		OnScoreChanged.Broadcast(RunGameScore);
	}
}

void UPlayerRuntimeState::SetScoringActive(bool bActive)
{
	bIsScoringActive = bActive;
}

// ---- Character State ----

void UPlayerRuntimeState::SetCharacterState(ERunGameCharacterState NewState)
{
	if (CurrentCharacterState == NewState)
	{
		return;
	}

	if (!IsCharacterStateTransitionAllowed(NewState))
	{
		UE_LOG(LogRunGame, Warning, TEXT("PlayerRuntimeState: Blocked invalid character state transition from %d to %d"),
			static_cast<int32>(CurrentCharacterState), static_cast<int32>(NewState));
		return;
	}

	const ERunGameCharacterState OldState = CurrentCharacterState;
	CurrentCharacterState = NewState;
	OnCharacterStateChanged.Broadcast(OldState, CurrentCharacterState);
}

bool UPlayerRuntimeState::IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const
{
	if (CurrentCharacterState == ERunGameCharacterState::Dead)
	{
		return false;
	}

	switch (NewState)
	{
	case ERunGameCharacterState::Idle:
		return CurrentCharacterState == ERunGameCharacterState::Sliding
			|| CurrentCharacterState == ERunGameCharacterState::Airborne
			|| CurrentCharacterState == ERunGameCharacterState::Turning;

	case ERunGameCharacterState::Airborne:
		return CurrentCharacterState == ERunGameCharacterState::Idle
			|| CurrentCharacterState == ERunGameCharacterState::Turning;

	case ERunGameCharacterState::Sliding:
		return CurrentCharacterState == ERunGameCharacterState::Idle;

	case ERunGameCharacterState::Turning:
		return CurrentCharacterState == ERunGameCharacterState::Idle
			|| CurrentCharacterState == ERunGameCharacterState::Sliding
			|| CurrentCharacterState == ERunGameCharacterState::Airborne;

	case ERunGameCharacterState::Dead:
		return CurrentCharacterState != ERunGameCharacterState::Dead;

	default:
		return false;
	}
}

void UPlayerRuntimeState::SetTurnFlags(bool bInTurn, bool bInBox)
{
	bTurn = bInTurn;
	bInTurnBox = bInBox;
}

void UPlayerRuntimeState::GetTurnFlags(bool& OutTurn, bool& OutInTurnBox) const
{
	OutTurn = bTurn;
	OutInTurnBox = bInTurnBox;
}

void UPlayerRuntimeState::AddSpeedModifier(FGameplayTag ModifierTag, float Multiplier)
{
	if (float* Existing = SpeedModifiers.Find(ModifierTag))
	{
		CachedCompositeSpeedMultiplier /= *Existing;
	}
	SpeedModifiers.Add(ModifierTag, Multiplier);
	CachedCompositeSpeedMultiplier *= Multiplier;
}

void UPlayerRuntimeState::RemoveSpeedModifier(FGameplayTag ModifierTag)
{
	if (float* Existing = SpeedModifiers.Find(ModifierTag))
	{
		CachedCompositeSpeedMultiplier /= *Existing;
	}
	SpeedModifiers.Remove(ModifierTag);
}

void UPlayerRuntimeState::CachePlayerCharacter(ARunGameCharacter* InCharacter)
{
	CachedPlayerCharacter = InCharacter;
}
