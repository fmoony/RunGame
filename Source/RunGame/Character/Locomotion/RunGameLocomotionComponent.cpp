#include "Character/Locomotion/RunGameLocomotionComponent.h"
#include "Character/RunGameCharacter.h"
#include "Character/RunGameMovementComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/World.h"

URunGameLocomotionComponent::URunGameLocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameLocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheDependencies();
}

void URunGameLocomotionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OwnerCharacter = nullptr;
	MovementComponent = nullptr;
	RuntimeState = nullptr;

	Super::EndPlay(EndPlayReason);
}

ERunGameCharacterState URunGameLocomotionComponent::GetRuntimeCharacterState() const
{
	return RuntimeState
		? RuntimeState->GetCharacterState()
		: ERunGameCharacterState::Idle;
}

bool URunGameLocomotionComponent::TryConsumeInputCommand(ERunGameInputCommand Command) const
{
	return MovementComponent && MovementComponent->TryConsumeInputCommand(Command);
}

void URunGameLocomotionComponent::HandleJumpInputReleased() const
{
	if (MovementComponent)
	{
		MovementComponent->HandleJumpInputReleased();
	}
}

bool URunGameLocomotionComponent::CanStartJump(bool bDefaultCanJump) const
{
	return MovementComponent
		? MovementComponent->CanStartJump(bDefaultCanJump)
		: bDefaultCanJump;
}

void URunGameLocomotionComponent::HandleOwnerJumped() const
{
	if (MovementComponent)
	{
		MovementComponent->HandleOwnerJumped();
	}
}

void URunGameLocomotionComponent::HandleOwnerLanded(const FHitResult& Hit) const
{
	if (MovementComponent)
	{
		MovementComponent->HandleOwnerLanded(Hit);
	}
}

bool URunGameLocomotionComponent::ApplyTurnRotation(float Right) const
{
	return MovementComponent && MovementComponent->ApplyTurnRotation(Right);
}

void URunGameLocomotionComponent::CacheDependencies()
{
	OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	MovementComponent = OwnerCharacter
		? OwnerCharacter->GetRunGameMovementComponent()
		: nullptr;

	if (UWorld* World = GetWorld())
	{
		RuntimeState = World->GetSubsystem<UPlayerRuntimeState>();
	}
}
