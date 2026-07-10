#include "Character/RunGameCharacter.h"
#include "Player/RunGamePlayerController.h"
#include "Actor/Component/HealthComponent.h"
#include "Skill/SkillComponent.h"
#include "Character/Locomotion/Movement/RunGameMovementComponent.h"
#include "Character/Locomotion/RunGameLocomotionComponent.h"
#include "Character/Input/RunGameInputComponent.h"
#include "Character/Input/RunGameInputContextComponent.h"
#include "Character/Effect/RunGameEffectComponent.h"
#include "Character/Collision/RunGameCollisionAbilityComponent.h"
#include "Character/Camera/RunGameCameraComponent.h"
#include "Animation/RunGameAnimInstance.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	GetCharacterMovement()->AirControl = 0.8f;  // 空中左右移动更自如 Better air strafing
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
	InputContextComponent = CreateDefaultSubobject<URunGameInputContextComponent>(TEXT("InputContextComponent"));
	RunGameInputComponent = CreateDefaultSubobject<URunGameInputComponent>(TEXT("RunGameInputComponent"));
	LocomotionComponent = CreateDefaultSubobject<URunGameLocomotionComponent>(TEXT("LocomotionComponent"));
	EffectComponent = CreateDefaultSubobject<URunGameEffectComponent>(TEXT("EffectComponent"));
	CollisionAbility = CreateDefaultSubobject<URunGameCollisionAbilityComponent>(TEXT("CollisionAbility"));
	CameraComponent = CreateDefaultSubobject<URunGameCameraComponent>(TEXT("CameraComponent"));

	// 注入 Native AnimInstance 类 Inject native AnimInstance class
	GetMesh()->SetAnimInstanceClass(URunGameAnimInstance::StaticClass());

	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
}

void ARunGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 状态机响应 Reactive to state machine
	if (UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		RS->OnCharacterStateChanged.AddDynamic(this, &ARunGameCharacter::OnRSCharacterStateChanged);
	}

	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &ARunGameCharacter::OnGameStateChanged);
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ARunGameCharacter::OnHealthDepleted);
		HealthComponent->OnDamageTaken.AddDynamic(this, &ARunGameCharacter::OnHealthDamageTaken);
	}

	// 输入生命周期由正式输入组件管理，Character 仅转发 EnhancedInput
	// Input lifetime is owned by the formal input component; Character only forwards EnhancedInput.
}

void ARunGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* RS = World->GetSubsystem<UPlayerRuntimeState>())
		{
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
		HealthComponent->OnDamageTaken.RemoveDynamic(this, &ARunGameCharacter::OnHealthDamageTaken);
	}

	Super::EndPlay(EndPlayReason);
}

void ARunGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (RunGameInputComponent)
	{
		RunGameInputComponent->BindInput(PlayerInputComponent);
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameCharacter: Missing RunGameInputComponent"));
	}
}

void ARunGameCharacter::Die(FGameplayTag DamageType, float DestroyDelay, AActor* DeathCauser)
{
	// 镜头由 CameraComponent 的 OnCharacterStateChanged(Dead) → DetachCameraToWorld 接管
	// Camera handled by CameraComponent::OnCharacterStateChanged(Dead) → DetachCameraToWorld

	// 关闭碰撞 → 设置 Dead → 全组件同步响应
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

	// 回到主菜单 → 隐藏（GameMode::SpawnPlayer 负责在 InGame 时显示）
	// Back to MainMenu → hide (GameMode::SpawnPlayer handles showing on InGame)
	if (NewState == ERunGameGameState::MainMenu)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

// ---- RS forward ----

void ARunGameCharacter::OnRSCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	OnCharacterStateChanged.Broadcast(OldState, NewState);
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
	const bool bDefaultCanJump = Super::CanJumpInternal_Implementation();
	if (const URunGameLocomotionComponent* LocomotionComp = GetRunGameLocomotionComponent())
	{
		return LocomotionComp->CanStartJump(bDefaultCanJump);
	}

	return bDefaultCanJump;
}

void ARunGameCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	if (URunGameLocomotionComponent* LocomotionComp = GetRunGameLocomotionComponent())
	{
		LocomotionComp->HandleOwnerJumped();
	}
}

// -- State Machine --

void ARunGameCharacter::SetCharacterState(ERunGameCharacterState NewState)
{
	UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (!RS) return;

	// Character 只作为 RuntimeState 转发门面；校验由 RuntimeState/组件域规则负责
	// Character is only a RuntimeState forwarding facade; validation belongs to RuntimeState/component-domain rules
	RS->TrySetCharacterState(NewState);
}

bool ARunGameCharacter::IsCharacterStateTransitionAllowed(ERunGameCharacterState NewState) const
{
	const UPlayerRuntimeState* RS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	return RS ? RS->IsCharacterStateTransitionAllowed(NewState) : false;
}

void ARunGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (URunGameLocomotionComponent* LocomotionComp = GetRunGameLocomotionComponent())
	{
		LocomotionComp->HandleOwnerLanded(Hit);
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

URunGameLocomotionComponent* ARunGameCharacter::GetRunGameLocomotionComponent() const
{
	return LocomotionComponent;
}
