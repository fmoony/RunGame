#include "Character/Animation/RunGameAnimInstance.h"
#include "Character/RunGameCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Engine/World.h"
#include "RunGame.h"

void URunGameAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
	if (Owner)
	{
		MovementComp = Owner->GetCharacterMovement();
	}

	CacheBaseSpeed();
	BindGameplayDelegates();
}

void URunGameAnimInstance::CacheBaseSpeed()
{
	if (MovementComp)
	{
		BaseMaxWalkSpeed = MovementComp->MaxWalkSpeed;
	}
}

void URunGameAnimInstance::BindGameplayDelegates()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TimerSubsystem = World->GetSubsystem<URunGameTimerSubsystem>();

	if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddDynamic(this, &URunGameAnimInstance::OnCharacterStateChanged);
		PRS->OnCharacterDied.AddDynamic(this, &URunGameAnimInstance::OnCharacterDied);
		PRS->OnHitReaction.AddDynamic(this, &URunGameAnimInstance::OnHitReaction);
	}
}

// ── Data pull ──

void URunGameAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner || !MovementComp) return;

	const FVector Velocity = Owner->GetVelocity();
	Speed = Velocity.Size();
	GroundSpeed = Velocity.Size2D();
	bIsInAir = MovementComp->IsFalling();
	bIsMoving = GroundSpeed > 10.0f;

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

// ── State reactions ──

void URunGameAnimInstance::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	if (NewState == ERunGameCharacterState::Sliding)
	{
		PlaySlideMontage();
	}

	if (OldState == ERunGameCharacterState::Sliding)
	{
		EndSlide();
	}

}

// ── Slide ──

void URunGameAnimInstance::PlaySlideMontage()
{
	if (!Owner || !SlideMontage) return;

	float FinalPlayRate = MontagePlayRate;
	if (TimerSubsystem && TimerSubsystem->IsTimerRunning() && MovementComp)
	{
		FinalPlayRate = MontagePlayRate * MovementComp->MaxWalkSpeed / FMath::Max(BaseMaxWalkSpeed, 1.0f);
	}

	OnMontageBlendingOut.AddUniqueDynamic(this, &URunGameAnimInstance::OnSlideBlendingOut);
	Owner->SetAnimRootMotionTranslationScale(RootMotionScale);
	Montage_Play(SlideMontage, FinalPlayRate);
}

void URunGameAnimInstance::EndSlide()
{
	if (Owner)
	{
		Owner->SetAnimRootMotionTranslationScale(1.0f);
	}
	OnMontageBlendingOut.RemoveDynamic(this, &URunGameAnimInstance::OnSlideBlendingOut);
}

void URunGameAnimInstance::OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != SlideMontage) return;

	EndSlide();

	// 告知状态机滑铲结束 → 切回 Idle
	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->SetCharacterState(ERunGameCharacterState::Idle);
		}
	}
}

// ── Death ──

void URunGameAnimInstance::PlayDeathMontage(FGameplayTag DamageType)
{
	if (!Owner) return;

	TObjectPtr<UAnimMontage>* Found = DeathMontages.Find(DamageType);
	UAnimMontage* Montage = Found ? Found->Get() : nullptr;

	if (Montage)
	{
		OnMontageBlendingOut.AddUniqueDynamic(this, &URunGameAnimInstance::OnDeathMontageBlendingOut);
		Montage_Play(Montage);
	}
}

void URunGameAnimInstance::OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	OnMontageBlendingOut.RemoveDynamic(this, &URunGameAnimInstance::OnDeathMontageBlendingOut);

	if (Owner)
	{
		Owner->GetCharacterMovement()->StopMovementImmediately();

		if (USkeletalMeshComponent* SkelMesh = Owner->GetMesh())
		{
			SkelMesh->bPauseAnims = true;
			SkelMesh->bNoSkeletonUpdate = true;
		}
	}

	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->NotifyDeathAnimationFinished();
		}
	}
}

// ── Character Died ──

void URunGameAnimInstance::OnCharacterDied(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter)
{
	PlayDeathMontage(DamageType);
}

// ── Hit ──

void URunGameAnimInstance::OnHitReaction(float Damage, FGameplayTag DamageType)
{
	TObjectPtr<UAnimMontage>* Found = HitReactionMontages.Find(DamageType);
	UAnimMontage* Montage = Found ? Found->Get() : nullptr;
	if (Montage)
	{
		Montage_Play(Montage);
	}
}
