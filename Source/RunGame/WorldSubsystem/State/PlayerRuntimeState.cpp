#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "RunGame.h"

void UPlayerRuntimeState::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPlayerRuntimeState::Deinitialize()
{
	Super::Deinitialize();
}

void UPlayerRuntimeState::ResetForNewGame()
{
	CurrentCharacterState = ERunGameCharacterState::Idle;
}

// ---- Animation Events ----

void UPlayerRuntimeState::TriggerHitReaction(float Damage, FGameplayTag DamageType)
{
	OnHitReaction.Broadcast(Damage, DamageType);
}

void UPlayerRuntimeState::NotifyDeathAnimationFinished()
{
	OnDeathAnimationFinished.Broadcast();
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
			|| CurrentCharacterState == ERunGameCharacterState::CoyoteTime
			|| CurrentCharacterState == ERunGameCharacterState::Turning;

	case ERunGameCharacterState::CoyoteTime:
		return CurrentCharacterState == ERunGameCharacterState::Idle
			|| CurrentCharacterState == ERunGameCharacterState::Turning;

	case ERunGameCharacterState::Airborne:
		return CurrentCharacterState == ERunGameCharacterState::Idle
			|| CurrentCharacterState == ERunGameCharacterState::CoyoteTime
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
