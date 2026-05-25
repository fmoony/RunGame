#include "Character/RunGameAnimationComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "Actor/Component/HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"
#include "Curves/CurveFloat.h"
#include "RunGame.h"
#include "Character/RunGameCharacter.h"

URunGameAnimationComponent::URunGameAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;

	BaseMaxWalkSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;
	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();

	// 绑角色状态变化——滑铲动画 / 停止动画 React to character state: slide / stop all
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddDynamic(this, &URunGameAnimationComponent::OnCharacterStateChanged);
	}

	// 绑 HealthComponent——受击 / 死亡动画 React to HealthComponent: hit reaction / death montage
	if (ARunGameCharacter* Char = Cast<ARunGameCharacter>(Owner))
	{
		if (UHealthComponent* HC = Char->GetHealthComponent())
		{
			HC->OnDamageTaken.AddDynamic(this, &URunGameAnimationComponent::OnDamageTaken);
			HC->OnDeath.AddDynamic(this, &URunGameAnimationComponent::OnDeath);
		}
	}
}

void URunGameAnimationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->OnCharacterStateChanged.RemoveDynamic(this, &URunGameAnimationComponent::OnCharacterStateChanged);
		}

		if (ARunGameCharacter* Char = Cast<ARunGameCharacter>(GetOwner()))
		{
			if (UHealthComponent* HC = Char->GetHealthComponent())
			{
				HC->OnDamageTaken.RemoveDynamic(this, &URunGameAnimationComponent::OnDamageTaken);
				HC->OnDeath.RemoveDynamic(this, &URunGameAnimationComponent::OnDeath);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}

// ---- State reactions ----

void URunGameAnimationComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	if (NewState == ERunGameCharacterState::Sliding)
	{
		PlaySlideMontage();
	}

	if (OldState == ERunGameCharacterState::Sliding)
	{
		EndSlide();
	}

	if (NewState == ERunGameCharacterState::Dead)
	{
		if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
		{
			if (UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance())
			{
				AnimInstance->StopAllMontages(0.0f);
			}
		}
	}
}

// ---- Slide ----

void URunGameAnimationComponent::PlaySlideMontage()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner || !SlideMontage) return;

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &URunGameAnimationComponent::OnSlideBlendingOut);

	float FinalPlayRate = MontagePlayRate;
	if (TimerSubsystem && TimerSubsystem->IsTimerRunning())
	{
		// 根据当前速度缩放播放速率 Scale play rate by current speed
		const float ElapsedTime = TimerSubsystem->GetTotalTimeSeconds();
		const float DesiredMaxSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;
		FinalPlayRate = MontagePlayRate * DesiredMaxSpeed / BaseMaxWalkSpeed;
	}
	
	Owner->SetAnimRootMotionTranslationScale(RootMotionScale);

	AnimInstance->Montage_Play(SlideMontage, FinalPlayRate);

	UE_LOG(LogRunGame, Warning, TEXT("AnimationComponent: Slide montage started. Rate: %.2f"), FinalPlayRate);
}

void URunGameAnimationComponent::EndSlide()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;

	Owner->SetAnimRootMotionTranslationScale(1.0f);

	if (UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &URunGameAnimationComponent::OnSlideBlendingOut);
	}

	UE_LOG(LogRunGame, Warning, TEXT("AnimationComponent: Slide ended."));
}

void URunGameAnimationComponent::OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	// 滑铲蒙太奇结束 → 请求返回 Idle Slide montage ended → request return to Idle
	if (ARunGameCharacter* Char = Cast<ARunGameCharacter>(GetOwner()))
	{
		Char->SetCharacterState(ERunGameCharacterState::Idle);
	}
}

// ---- Hit Reaction ----

void URunGameAnimationComponent::OnDamageTaken(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	PlayHitReaction(DamageType);
}

void URunGameAnimationComponent::PlayHitReaction(FGameplayTag DamageType)
{
	if (HitReactionMontages.IsEmpty()) return;

	UAnimMontage** Found = HitReactionMontages.Find(DamageType);
	UAnimMontage* Montage = Found ? *Found : nullptr;
	if (!Montage) return;

	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		Owner->PlayAnimMontage(Montage);
	}
}

// ---- Death ----

void URunGameAnimationComponent::OnDeath(FGameplayTag DamageType, AActor* DeathCauser)
{
	PlayDeathMontage(DamageType);
}

void URunGameAnimationComponent::PlayDeathMontage(FGameplayTag DamageType)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;

	UAnimMontage** FoundMontage = DeathMontages.Find(DamageType);
	UAnimMontage* DeathMontage = FoundMontage ? *FoundMontage : nullptr;

	UE_LOG(LogRunGame, Warning, TEXT("AnimationComponent: Death montage request. DamageType=%s, Montage=%s"),
		*DamageType.ToString(), *GetNameSafe(DeathMontage));

	if (DeathMontage && Owner->GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
		AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &URunGameAnimationComponent::OnDeathMontageBlendingOut);
		AnimInstance->Montage_Play(DeathMontage);
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameAnimationComponent: No Death Montage"));
		// 无死亡蒙太奇时直接触发完成广播 No death montage → fire complete immediately
		OnDeathMontageComplete.Broadcast();
	}
}

void URunGameAnimationComponent::OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		if (UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &URunGameAnimationComponent::OnDeathMontageBlendingOut);
		}

		Owner->GetCharacterMovement()->StopMovementImmediately();

		if (USkeletalMeshComponent* SkelMesh = Owner->GetMesh())
		{
			SkelMesh->bPauseAnims = true;
			SkelMesh->bNoSkeletonUpdate = true;
		}
	}

	OnDeathMontageComplete.Broadcast();
}
