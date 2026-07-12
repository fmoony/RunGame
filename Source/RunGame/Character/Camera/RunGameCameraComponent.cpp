#include "Character/Camera/RunGameCameraComponent.h"
#include "Character/RunGameCharacter.h"
#include "Character/Locomotion/Movement/RunGameMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/RunGameGameState.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "RunGame.h"

URunGameCameraComponent::URunGameCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URunGameCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheOwnerComponents();

	ARunGameCharacter* Char = Cast<ARunGameCharacter>(GetOwner());
	if (!Char) return;

	if (MovementComponent)
	{
		MovementComponent->OnRunDirectionChanged.AddUObject(this, &URunGameCameraComponent::HandleRunDirectionChanged);
		SyncRunDirection(true);
	}

	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddUniqueDynamic(this, &URunGameCameraComponent::OnGameStateChanged);
	}

	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.AddUniqueDynamic(this, &URunGameCameraComponent::OnCharacterStateChanged);
		PRS->OnCharacterDied.AddUniqueDynamic(this, &URunGameCameraComponent::OnCharacterDied);
	}

	TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>();

	// 初始化时检查当前状态 — Character 提前生成，可能在 MainMenu/InGame 任一状态
	// Check current state on init — Character pre-spawned, may be in MainMenu or InGame
	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		const ERunGameGameState State = GS->GetCurrentState();
		if (State == ERunGameGameState::MainMenu)
		{
			SwitchToMainMenuCamera();
		}
		else if (State == ERunGameGameState::InGame)
		{
			SwitchToFollowCamera();
		}
	}
}

void URunGameCameraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (MovementComponent)
	{
		MovementComponent->OnRunDirectionChanged.RemoveAll(this);
	}

	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.RemoveDynamic(this, &URunGameCameraComponent::OnGameStateChanged);
	}

	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnCharacterStateChanged.RemoveDynamic(this, &URunGameCameraComponent::OnCharacterStateChanged);
		PRS->OnCharacterDied.RemoveDynamic(this, &URunGameCameraComponent::OnCharacterDied);
	}

	Super::EndPlay(EndPlayReason);
}

void URunGameCameraComponent::CacheOwnerComponents()
{
	ARunGameCharacter* Char = Cast<ARunGameCharacter>(GetOwner());
	if (!Char) return;

	CachedCameraBoom = Char->GetCameraBoom();
	CachedFollowCamera = Char->GetFollowCamera();
	MovementComponent = Char->GetRunGameMovementComponent();
}

void URunGameCameraComponent::ApplyLookInput(const FVector2D& LookAxis)
{
	ARunGameCharacter* Char = Cast<ARunGameCharacter>(GetOwner());
	if (!Char || !Char->GetController() || bCameraDetached)
	{
		return;
	}

	bReceivedLookInputSinceLastTick |= !LookAxis.IsNearlyZero();
	Char->AddControllerYawInput(LookAxis.X);
	Char->AddControllerPitchInput(LookAxis.Y);
}

// ---- Tick ----

void URunGameCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const bool bReceivedLookInput = bReceivedLookInputSinceLastTick;
	bReceivedLookInputSinceLastTick = false;

	if (!CachedFollowCamera || !CachedCameraBoom || bCameraDetached) return;

	UpdateControlRotation(DeltaTime, bReceivedLookInput);

	// ---- FOV：速度越快越宽 FOV: wider with speed ----
	const ACharacter* Char = Cast<ACharacter>(GetOwner());
	const float Speed = Char ? Char->GetVelocity().Size2D() : 0.0f;
	const float MaxSpeed = Char && Char->GetCharacterMovement() ? Char->GetCharacterMovement()->MaxWalkSpeed : 1200.0f;
	const float SpeedRatio = MaxSpeed > 0.0f ? FMath::Clamp(Speed / MaxSpeed, 0.0f, 1.0f) : 0.0f;
	CurrentTargetFOV = BaseFOV + SpeedFOVBoost * SpeedRatio;
	CurrentSmoothedFOV = FMath::FInterpTo(CurrentSmoothedFOV, CurrentTargetFOV, DeltaTime, FOVInterpSpeed);
	CachedFollowCamera->SetFieldOfView(CurrentSmoothedFOV);

	// ---- SpringArm 长度：状态驱动 Arm length: state-driven ----
	switch (CurrentCharState)
	{
	case ERunGameCharacterState::Airborne:
	case ERunGameCharacterState::CoyoteTime:
		CurrentTargetArmLength = AirborneArmLength;
		break;
	case ERunGameCharacterState::Sliding:
		CurrentTargetArmLength = SlideArmLength;
		break;
	default:
		CurrentTargetArmLength = DefaultArmLength;
		break;
	}
	CurrentSmoothedArmLength = FMath::FInterpTo(CurrentSmoothedArmLength, CurrentTargetArmLength, DeltaTime, ArmInterpSpeed);
	CachedCameraBoom->TargetArmLength = CurrentSmoothedArmLength;
}

void URunGameCameraComponent::UpdateControlRotation(float DeltaTime, bool bReceivedLookInput)
{
	APlayerController* PC = GetPC();
	if (!PC || !MovementComponent)
	{
		return;
	}

	if (!bRunDirectionInitialized)
	{
		SyncRunDirection(false);
	}

	const float PreviousReferenceYaw = CurrentReferenceYaw;
	CurrentReferenceYaw = FMath::FixedTurn(
		CurrentReferenceYaw,
		TargetReferenceYaw,
		TurnFollowYawSpeed * DeltaTime);
	const float ReferenceDelta = FMath::FindDeltaAngleDegrees(PreviousReferenceYaw, CurrentReferenceYaw);

	FRotator ControlRotation = PC->GetControlRotation();
	ControlRotation.Yaw = FRotator::NormalizeAxis(ControlRotation.Yaw + ReferenceDelta);

	const bool bFollowingTurn = !FMath::IsNearlyEqual(CurrentReferenceYaw, TargetReferenceYaw, 0.01f);
	if (bEnableYawRecentering
		&& !bReceivedLookInput
		&& !bFollowingTurn
		&& YawRecenteringSpeed > 0.0f)
	{
		const FRotator TargetRotation(ControlRotation.Pitch, CurrentReferenceYaw, ControlRotation.Roll);
		ControlRotation = FMath::RInterpTo(ControlRotation, TargetRotation, DeltaTime, YawRecenteringSpeed);
	}

	const float YawOffset = FMath::Clamp(
		FMath::FindDeltaAngleDegrees(CurrentReferenceYaw, ControlRotation.Yaw),
		-MaxYawOffset,
		MaxYawOffset);
	ControlRotation.Yaw = FRotator::NormalizeAxis(CurrentReferenceYaw + YawOffset);
	PC->SetControlRotation(ControlRotation);
}

void URunGameCameraComponent::SyncRunDirection(bool bResetViewYaw)
{
	if (!MovementComponent)
	{
		return;
	}

	CurrentReferenceYaw = FRotator::NormalizeAxis(MovementComponent->GetDesireRotation().Yaw);
	TargetReferenceYaw = CurrentReferenceYaw;
	bRunDirectionInitialized = true;

	if (bResetViewYaw)
	{
		if (APlayerController* PC = GetPC())
		{
			FRotator ControlRotation = PC->GetControlRotation();
			ControlRotation.Yaw = CurrentReferenceYaw;
			PC->SetControlRotation(ControlRotation);
		}
	}
}

void URunGameCameraComponent::HandleRunDirectionChanged(float OldYaw, float NewYaw)
{
	if (!bRunDirectionInitialized)
	{
		CurrentReferenceYaw = FRotator::NormalizeAxis(OldYaw);
		bRunDirectionInitialized = true;
	}

	TargetReferenceYaw = FRotator::NormalizeAxis(NewYaw);
}

// ---- 状态机 State machine ----

void URunGameCameraComponent::OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState)
{
	switch (NewState)
	{
	case ERunGameGameState::MainMenu:
		SyncRunDirection(false);
		SwitchToMainMenuCamera();
		ReattachCameraToOwner();
		break;
	case ERunGameGameState::CountDown:
		// 重启时：切主菜单视点 + 挂回 SpringArm → InGame 时仅需 Blend
		// On restart: switch to menu view + reattach SpringArm → InGame only needs Blend
		if (OldState != ERunGameGameState::MainMenu && OldState != ERunGameGameState::Pause)
		{
			SwitchToMainMenuCamera();
		}
		ReattachCameraToOwner();
		if (OldState != ERunGameGameState::Pause)
		{
			SyncRunDirection(true);
		}
		break;
	case ERunGameGameState::InGame:
		SwitchToFollowCamera();
		break;
	default:
		break;
	}
}

void URunGameCameraComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	CurrentCharState = NewState;

	// 死亡 → 镜头脱离 Death → detach camera
	if (NewState == ERunGameCharacterState::Dead)
	{
		DetachCameraToWorld();
	}
}

void URunGameCameraComponent::OnCharacterDied(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter)
{
	// 也由 OnCharacterStateChanged(Dead) 触发，此处冗余但保险 Also triggered by state change, redundant but safe
	if (DeadCharacter != GetOwner()) return;
	DetachCameraToWorld();
}

// ---- 镜头操作 Camera operations ----

void URunGameCameraComponent::SwitchToMainMenuCamera()
{
	APlayerController* PC = GetPC();
	if (!PC) return;

	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(MainMenuCameraTag))
		{
			PC->SetViewTargetWithBlend(*It, RestoreBlendTime);
			PC->bAutoManageActiveCameraTarget = false;
			return;
		}
	}
}

void URunGameCameraComponent::SwitchToFollowCamera()
{
	APlayerController* PC = GetPC();
	if (PC && GetOwner())
	{
		ReattachCameraToOwner();
		PC->SetViewTargetWithBlend(GetOwner(), RestoreBlendTime);
		PC->bAutoManageActiveCameraTarget = true;
	}
}

void URunGameCameraComponent::DetachCameraToWorld()
{
	if (!CachedCameraBoom || bCameraDetached) return;

	CachedCameraBoom->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	CachedCameraBoom->bDoCollisionTest = false;
	bCameraDetached = true;

	if (APlayerController* PC = GetPC())
	{
		PC->bAutoManageActiveCameraTarget = false;
	}

	UE_LOG(LogRunGame, Warning, TEXT("CameraComponent: Detached at death position"));
}

void URunGameCameraComponent::ReattachCameraToOwner()
{
	if (!CachedCameraBoom || !bCameraDetached) return;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Snap 挂回 + 重置相对位置 — 不用 KeepWorldTransform（死亡偏移会带到出生点）
	// Snap reattach + reset relative position — not KeepWorldTransform (death offset carries to spawn)
	CachedCameraBoom->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CachedCameraBoom->SetRelativeLocation(FVector::ZeroVector);
	CachedCameraBoom->bDoCollisionTest = true;
	CachedCameraBoom->TargetArmLength = DefaultArmLength;
	bCameraDetached = false;
	CurrentSmoothedArmLength = DefaultArmLength;
	SyncRunDirection(true);

	UE_LOG(LogRunGame, Warning, TEXT("CameraComponent: Reattached (position reset)"));
}

APlayerController* URunGameCameraComponent::GetPC() const
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}
