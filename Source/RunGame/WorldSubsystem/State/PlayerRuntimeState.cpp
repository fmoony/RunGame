#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "GameplayTagContainer.h"
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
	// 强制重置——绕过 Dead 守卫，且广播通知所有子系统
	// Force reset — bypass Dead guard, broadcast to all subsystems
	ActiveEffectTags.Reset();

	const ERunGameCharacterState OldState = CurrentCharacterState;
	CurrentCharacterState = ERunGameCharacterState::Idle;

	UE_LOG(LogRunGame, Warning, TEXT("PRS::ResetForNewGame: OldState=%d → Idle, broadcasting=%d"),
		(int32)OldState, OldState != CurrentCharacterState);

	if (OldState != CurrentCharacterState)
	{
		OnCharacterStateChanged.Broadcast(OldState, CurrentCharacterState);
	}
}

// ---- Effect Tags ----

void UPlayerRuntimeState::AddEffectTag(FGameplayTag Tag)
{
	if (!Tag.IsValid() || ActiveEffectTags.HasTag(Tag)) return;

	ActiveEffectTags.AddTag(Tag);
	UE_LOG(LogRunGame, Warning, TEXT("PlayerRuntimeState:AddEffectTag Tag:%s, Count:%d"), *Tag.ToString(), ActiveEffectTags.Num());
	OnEffectTagChanged.Broadcast(Tag, true);
}

void UPlayerRuntimeState::RemoveEffectTag(FGameplayTag Tag)
{
	if (!Tag.IsValid() || !ActiveEffectTags.HasTag(Tag)) return;

	ActiveEffectTags.RemoveTag(Tag);
	UE_LOG(LogRunGame, Warning, TEXT("PlayerRuntimeState:RemoveEffectTag Tag:%s, Count:%d"), *Tag.ToString(), ActiveEffectTags.Num());
	OnEffectTagChanged.Broadcast(Tag, false);
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
	TrySetCharacterState(NewState);
}

bool UPlayerRuntimeState::TrySetCharacterState(ERunGameCharacterState NewState)
{
	if (CurrentCharacterState == NewState)
	{
		return true;
	}

	if (!IsCharacterStateTransitionAllowed(NewState))
	{
		UE_LOG(LogRunGame, Warning, TEXT("PlayerRuntimeState: Blocked invalid character state transition from %d to %d"),
			static_cast<int32>(CurrentCharacterState), static_cast<int32>(NewState));
		return false;
	}

	const ERunGameCharacterState OldState = CurrentCharacterState;
	CurrentCharacterState = NewState;

	OnCharacterStateChanged.Broadcast(OldState, CurrentCharacterState);
	return true;
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
