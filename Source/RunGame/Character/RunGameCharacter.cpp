#include "Character/RunGameCharacter.h"
#include "Player/RunGamePlayerController.h"
#include "Actor/Component/HealthComponent.h"
#include "Skill/SkillComponent.h"
#include "Character/RunGameMovementComponent.h"
#include "Character/RunGameInputBufferComponent.h"
#include "Character/RunGameEffectComponent.h"
#include "Character/RunGameCollisionAbilityComponent.h"
#include "Animation/RunGameAnimInstance.h"
#include "Skill/RunGameSkillConfigData.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "RunGame.h"
#include "Game/RunGameGameState.h"
#include "Materials/MaterialInstanceDynamic.h"

ARunGameCharacter::ARunGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URunGameMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CMC config — 默认值由 URunGameMovementComponent 接管，此处仅设基础
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	InputBuffer = CreateDefaultSubobject<URunGameInputBufferComponent>(TEXT("InputBuffer"));
	EffectComponent = CreateDefaultSubobject<URunGameEffectComponent>(TEXT("EffectComponent"));
	CollisionAbility = CreateDefaultSubobject<URunGameCollisionAbilityComponent>(TEXT("CollisionAbility"));

	// 注入 Native AnimInstance 类 Inject native AnimInstance class
	GetMesh()->SetAnimInstanceClass(URunGameAnimInstance::StaticClass());

	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
}

void ARunGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 状态机响应——自身逻辑 + 转发给蓝图 Reactive to state machine: self logic + forward to BP
	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->OnCharacterStateChanged.AddDynamic(this, &ARunGameCharacter::OnCharacterStateChangedCallback);
		RS->OnCharacterStateChanged.AddDynamic(this, &ARunGameCharacter::OnRSCharacterStateChanged);
	}

	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &ARunGameCharacter::OnGameStateChanged);
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ARunGameCharacter::OnHealthDepleted);
	}

	// 桥接 HealthComponent 伤害 → RuntimeState → AnimInstance Hit reaction bridge
	if (HealthComponent)
	{
		HealthComponent->OnDamageTaken.AddDynamic(this, &ARunGameCharacter::OnHealthDamageTaken);
	}

	// 输入缓冲消费：Jump → ACharacter::Jump，Slide → SetCharacterState(Sliding)
	// Input buffer consumption: Jump → ACharacter::Jump, Slide → SetCharacterState
	if (InputBuffer)
	{
		InputBuffer->OnInputCommandConsumed.BindUObject(this, &ARunGameCharacter::OnBufferedInputReady);
	}
}

void ARunGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* RS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			RS->OnCharacterStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnCharacterStateChangedCallback);
			RS->OnCharacterStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnRSCharacterStateChanged);
		}

		if (ARunGameGameState* GS = World->GetGameState<ARunGameGameState>())
		{
			GS->OnGameStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnGameStateChanged);
		}
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.RemoveDynamic(this, &ARunGameCharacter::OnHealthDepleted);
	}

	Super::EndPlay(EndPlayReason);
}

void ARunGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ARunGameCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARunGameCharacter::DoJumpEnd);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunGameCharacter::Move);

		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ARunGameCharacter::StartSlide);

		if (SkillComponent && SkillComponent->SkillConfig)
		{
			for (const FSkillDefinition& SkillDef : SkillComponent->SkillConfig->Skills)
			{
				if (SkillDef.InputAction && SkillDef.SkillTag.IsValid())
				{
					EnhancedInputComponent->BindAction(SkillDef.InputAction, ETriggerEvent::Started,
						this, &ARunGameCharacter::ActivateSkillByTag, SkillDef.SkillTag);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameCharacter: Failed to find Enhanced Input component!"));
	}
}

void ARunGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ARunGameCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ARunGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController() == nullptr || Right == 0.0f) return;

	URunGameMovementComponent* MoveComp = GetRunGameMovementComponent();
	if (!MoveComp) return;

	// 转弯系统：MovementComponent 处理旋转。返回 true = 在转向盒内，阻止横向输入
	// Turn system: MovementComponent handles rotation. Returns true = in turn box, block lateral input
	const bool bBlockLateral = MoveComp->ApplyTurnRotation(Right);
	if (!bBlockLateral)
	{
		const FRotator YawRotation(0, GetController()->GetControlRotation().Yaw, 0);
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Right);
	}
}

void ARunGameCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ARunGameCharacter::DoJumpStart()
{
	// CanJump 通过 → 执行跳跃（副作用在 OnJumped）；不通过 → 缓冲（滑铲中等场景）
	// CanJump passes → execute jump (side effects in OnJumped); fails → buffer (e.g. during slide)
	if (CanJump())
	{
		PreJumpState = GetCharacterState();
		Jump();
	}
	else
	{
		InputBuffer->BufferInput(ERunGameInputCommand::Jump);
	}
}

void ARunGameCharacter::DoJumpEnd() { StopJumping(); }

void ARunGameCharacter::StartSlide() { InputBuffer->BufferInput(ERunGameInputCommand::Slide); }

void ARunGameCharacter::OnBufferedInputReady(ERunGameInputCommand Command)
{
	switch (Command)
	{
	case ERunGameInputCommand::Jump:
		PreJumpState = GetCharacterState();
		Jump();  // CanJumpInternal validates; OnJumped handles side effects
		break;
	case ERunGameInputCommand::Slide:
		SetCharacterState(ERunGameCharacterState::Sliding);
		break;
	default:
		break;
	}
}
void ARunGameCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	if (CameraBoom) CameraBoom->TargetOffset.Z += ScaledHalfHeightAdjust;
}

void ARunGameCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	if (CameraBoom) CameraBoom->TargetOffset.Z -= ScaledHalfHeightAdjust;
}

void ARunGameCharacter::Die(FGameplayTag DamageType, float DestroyDelay, AActor* DeathCauser)
{
	// 1. 先切死亡摄像机（在状态变更前——广播同步，动画组件会立即播蒙太奇）
	// Camera first (before state change — broadcast is sync, animation starts immediately)
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		const FVector CamLoc = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation();
		const FRotator CamRot = FollowCamera ? FollowCamera->GetComponentRotation() : GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (ACameraActor* DeathCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CamLoc, CamRot, SpawnParams))
		{
			PC->SetViewTargetWithBlend(DeathCamera, 0.1f);
			PC->bAutoManageActiveCameraTarget = false;
			UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Death camera spawned."));
		}
	}

	// 2. 关闭碰撞 → 设置 Dead → 全组件同步响应
	// Disable collision → set Dead → all components react synchronously
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCharacterState(ERunGameCharacterState::Dead);

	// 3. 广播死亡 Broadcast death
	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->OnCharacterDied.Broadcast(DamageType, this);
	}
	OnCharacterDied.Broadcast(DamageType, this);
}

void ARunGameCharacter::OnHealthDepleted(FGameplayTag DamageType, AActor* DeathCauser)
{
	Die(DamageType, 3.0f, DeathCauser);
}

void ARunGameCharacter::OnHealthDamageTaken(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->TriggerHitReaction(Damage, DamageType);
	}
}

// ---- GameState ----

void ARunGameCharacter::OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (OldState == NewState) return;
	if (NewState == ERunGameGameState::MainMenu) Destroy();
}

// ---- RS forward ----

void ARunGameCharacter::OnRSCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	OnCharacterStateChanged.Broadcast(OldState, NewState);
}

void ARunGameCharacter::ActivateSkillByTag(FGameplayTag SkillTag)
{
	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

	const ERunGameCharacterState State = RS->GetCharacterState();
	if (State == ERunGameCharacterState::Dead || State == ERunGameCharacterState::Sliding) return;

	if (SkillComponent) SkillComponent->TryActivateSkill(SkillTag);
}

// ~begin IDamagable

void ARunGameCharacter::OnTakeDamage_Implementation(float Damage, FGameplayTag DamageType, AActor* DamageCauser)
{
	if (HealthComponent) HealthComponent->ApplyDamage(Damage, DamageType, DamageCauser);
}

void ARunGameCharacter::OnTakeHealing_Implementation(float HealAmount, AActor* Healer)
{
	if (HealthComponent) HealthComponent->Heal(HealAmount, Healer);
}

void ARunGameCharacter::OnDeath_Implementation(AActor* DeathCauser) {}

float ARunGameCharacter::GetCurrentHP_Implementation() const { return HealthComponent ? HealthComponent->GetCurrentHP() : 0.0f; }
float ARunGameCharacter::GetMaxHP_Implementation() const { return HealthComponent ? HealthComponent->GetMaxHP() : 0.0f; }
bool ARunGameCharacter::IsDead_Implementation() const { return HealthComponent ? HealthComponent->IsDead() : false; }

// ~end IDamagable

// -- Jump Override --

bool ARunGameCharacter::CanJumpInternal_Implementation() const
{
	// 默认地面检查 Default ground check
	if (Super::CanJumpInternal_Implementation())
	{
		return true;
	}

	// CoyoteTime — 土狼时间缓冲期内允许跳跃 CoyoteTime grace period
	const ERunGameCharacterState State = GetCharacterState();
	if (State == ERunGameCharacterState::CoyoteTime)
	{
		return true;
	}

	// Airborne + 二段跳可用 Double jump available
	if (State == ERunGameCharacterState::Airborne && bAirJumpAvailable)
	{
		return true;
	}

	return false;
}

void ARunGameCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	if (PreJumpState == ERunGameCharacterState::CoyoteTime)
	{
		// 土狼时间中起跳 → 消耗空中资格 + 转入 Airborne
		// Jumped during coyote time → consume air jump + transition to Airborne
		bAirJumpAvailable = false;
		SetCharacterState(ERunGameCharacterState::Airborne);
	}
	else if (PreJumpState == ERunGameCharacterState::Airborne)
	{
		// 二段跳 → 消耗空中资格
		// Double jump → consume air jump
		bAirJumpAvailable = false;
	}
	// PreJumpState == Idle: 地面跳——空中资格由 OnCharacterStateChangedCallback 在 Idle→CoyoteTime 时授予
	// PreJumpState == Idle: ground jump — air jump granted by OnCharacterStateChangedCallback on Idle→CoyoteTime
}

// -- State Machine --

void ARunGameCharacter::SetCharacterState(ERunGameCharacterState NewState)
{
	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

	const ERunGameCharacterState CurrentState = RS->GetCharacterState();
	if (CurrentState == NewState) return;

	if (!IsCharacterStateTransitionAllowed(NewState))
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Blocked transition from %d to %d"),
			static_cast<int32>(CurrentState), static_cast<int32>(NewState));
		return;
	}

	// Character 层额外守卫：滑动必须在地面 Extra guard: sliding requires ground
	if (NewState == ERunGameCharacterState::Sliding && GetCharacterMovement()->IsFalling()) return;

	RS->SetCharacterState(NewState);
}

bool ARunGameCharacter::IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const
{
	const UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	return RS ? RS->IsCharacterStateTransitionAllowed(NewState) : false;
}

void ARunGameCharacter::OnCharacterStateChangedCallback(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	// 进入 CoyoteTime 或从地面跳起 → 授予空中跳跃资格 Entering CoyoteTime or ground jump → grant air jump
	if (NewState == ERunGameCharacterState::CoyoteTime)
	{
		bAirJumpAvailable = true;
	}

	// 地面起跳 → Airborne → 授予空中跳跃资格 Ground jump → Airborne → grant air jump
	if (NewState == ERunGameCharacterState::Airborne && OldState == ERunGameCharacterState::Idle)
	{
		bAirJumpAvailable = true;
	}

	// 着陆 → 重置空中跳跃资格 Landing → reset air jump
	if (NewState == ERunGameCharacterState::Idle)
	{
		bAirJumpAvailable = true;
	}
}

void ARunGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

	const ERunGameCharacterState State = RS->GetCharacterState();
	if (State == ERunGameCharacterState::Airborne || State == ERunGameCharacterState::CoyoteTime)
	{
		SetCharacterState(ERunGameCharacterState::Idle);
		// InputBuffer 自行监听状态变化 → Idle → 自动消费缓冲的 Slide
	}
}

ERunGameCharacterState ARunGameCharacter::GetCharacterState() const
{
	const UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	return RS ? RS->GetCharacterState() : ERunGameCharacterState::Idle;
}

bool ARunGameCharacter::IsSliding() const
{
	return GetCharacterState() == ERunGameCharacterState::Sliding;
}

URunGameMovementComponent* ARunGameCharacter::GetRunGameMovementComponent() const
{
	return Cast<URunGameMovementComponent>(GetCharacterMovement());
}
