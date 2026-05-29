#include "Character/Animation/RunGameAnimInstance.h"
#include "Character/RunGameCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/World.h"

void URunGameAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
	if (Owner)
	{
		MovementComp = Owner->GetCharacterMovement();
	}

	BindGameplayDelegates();
}

void URunGameAnimInstance::BindGameplayDelegates()
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
	{
		// 受击事件 → 设触发变量 → ABP 播放受击动画
		PRS->OnHitReaction.AddDynamic(this, &URunGameAnimInstance::OnHitReaction);
	}
}

void URunGameAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner || !MovementComp) return;

	// ── Locomotion ──

	const FVector Velocity = Owner->GetVelocity();
	Speed = Velocity.Size();
	GroundSpeed = Velocity.Size2D();
	bIsInAir = MovementComp->IsFalling();
	bIsMoving = GroundSpeed > 10.0f;

	// ── State ──

	if (UWorld* World = GetWorld())
	{
		if (const UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			CharacterState = PRS->GetCharacterState();
			bIsSliding = (CharacterState == ERunGameCharacterState::Sliding);
			bIsTurning = (CharacterState == ERunGameCharacterState::Turning);
			bIsDead = (CharacterState == ERunGameCharacterState::Dead);
		}
	}

	// ── Direction ──

	if (GroundSpeed > 10.0f)
	{
		const FRotator ActorRotation = Owner->GetActorRotation();
		const FVector VelocityDir2D = Velocity.GetSafeNormal2D();
		const FVector ForwardDir2D = ActorRotation.Vector().GetSafeNormal2D();

		const float Dot = FVector::DotProduct(ForwardDir2D, VelocityDir2D);
		const float Cross = FVector::CrossProduct(ForwardDir2D, VelocityDir2D).Z;

		MoveDirectionAngle = FMath::RadiansToDegrees(FMath::Atan2(Cross, Dot));
	}
	else
	{
		MoveDirectionAngle = 0.0f;
	}
}

// ── Hit reaction ──

void URunGameAnimInstance::OnHitReaction(float Damage, FGameplayTag DamageType)
{
	bPlayHitReaction = true;
	HitReactionDamageType = DamageType;
}

void URunGameAnimInstance::NotifyHitReactionFinished()
{
	bPlayHitReaction = false;
	HitReactionDamageType = FGameplayTag();
}
