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

	// 缓存 RuntimeState —— 避免每帧 GetSubsystem 查找
	CachedPRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();

	if (MovementComp)
	{
		BaseMaxWalkSpeed = MovementComp->MaxWalkSpeed;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	TimerSubsystem = World->GetSubsystem<URunGameTimerSubsystem>();

	if (CachedPRS.IsValid())
	{
		CachedPRS->OnCharacterStateChanged.AddDynamic(this, &URunGameAnimInstance::OnCharacterStateChanged);
		CachedPRS->OnCharacterDied.AddDynamic(this, &URunGameAnimInstance::OnCharacterDied);
		CachedPRS->OnHitReaction.AddDynamic(this, &URunGameAnimInstance::OnHitReaction);
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

	if (CachedPRS.IsValid())
	{
		CharacterState = CachedPRS->GetCharacterState();
		bIsCoyoteTime = (CharacterState == ERunGameCharacterState::CoyoteTime);
		bIsSliding = (CharacterState == ERunGameCharacterState::Sliding);
		bIsTurning = (CharacterState == ERunGameCharacterState::Turning);
		bIsDead = (CharacterState == ERunGameCharacterState::Dead);
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
	UE_LOG(LogRunGame, Warning, TEXT("AnimInstance::OnCharacterStateChanged: %d → %d, Owner=%s"),
		(int32)OldState, (int32)NewState, *GetNameSafe(Owner));

	if (NewState == ERunGameCharacterState::Sliding)
	{
		PlaySlideMontage();
	}

	if (OldState == ERunGameCharacterState::Sliding)
	{
		EndSlide();
	}

	// 从 Dead 恢复 — 停死亡动画 + 解冻骨骼 + 恢复可见
	if (OldState == ERunGameCharacterState::Dead)
	{
		UE_LOG(LogRunGame, Warning, TEXT("AnimInstance: Leaving Dead — cleaning up"));

		// 停掉可能还在播的死亡蒙太奇 Stop any in-progress death montage
		OnMontageBlendingOut.RemoveDynamic(this, &URunGameAnimInstance::OnDeathMontageBlendingOut);
		Montage_Stop(0.0f);

		if (Owner)
		{
			if (USkeletalMeshComponent* SkelMesh = Owner->GetMesh())
			{
				SkelMesh->bPauseAnims = false;
				SkelMesh->bNoSkeletonUpdate = false;
				SkelMesh->SetVisibility(true);
				SkelMesh->SetHiddenInGame(false);
				SkelMesh->bRecentlyRendered = true;
				SkelMesh->MarkRenderStateDirty();
				UE_LOG(LogRunGame, Warning, TEXT("AnimInstance: Skeleton unfrozen + forced visible"));
			}
		}
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

	// 广播动画结束信号，由 Locomotion 决定是否结束滑铲 Broadcast animation end; Locomotion decides whether to finish slide
	OnSlideMontageEnded.Broadcast(Montage, bInterrupted);
}

// ── Death ──

void URunGameAnimInstance::PlayDeathMontage(FGameplayTag DamageType)
{
	if (!Owner) { UE_LOG(LogRunGame, Warning, TEXT("AnimInstance::PlayDeathMontage — Owner is null!")); return; }

	TObjectPtr<UAnimMontage>* Found = DeathMontages.Find(DamageType);
	UAnimMontage* Montage = Found ? Found->Get() : nullptr;

	if (Montage)
	{
		UE_LOG(LogRunGame, Warning, TEXT("AnimInstance::PlayDeathMontage — Playing montage for %s"), *DamageType.ToString());
		OnMontageBlendingOut.AddUniqueDynamic(this, &URunGameAnimInstance::OnDeathMontageBlendingOut);
		Montage_Play(Montage);
	}
	else
	{
		UE_LOG(LogRunGame, Warning, TEXT("AnimInstance::PlayDeathMontage — No montage found for %s"), *DamageType.ToString());
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

	if (CachedPRS.IsValid())
	{
		CachedPRS->NotifyDeathAnimationFinished();
	}
}

// ── Character Died ──

void URunGameAnimInstance::OnCharacterDied(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter)
{
	UE_LOG(LogRunGame, Warning, TEXT("AnimInstance::OnCharacterDied — DamageType=%s, Owner=%s"),
		*DamageType.ToString(), *GetNameSafe(Owner));
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
