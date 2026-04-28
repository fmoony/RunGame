// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunGameCharacter.h"
#include "RunGamePlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "RunGame.h"
#include "RunGameGameState.h"

ARunGameCharacter::ARunGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	bIsSliding = false;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ARunGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;

	// 绑定 GameState 状态变化，MainMenu 时自毁
	if (ARunGameGameState* GameState = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GameState->OnGameStateChanged.AddDynamic(this, &ARunGameCharacter::OnGameStateChangedCallback);
	}
}

void ARunGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (ARunGameGameState* GameState = World->GetGameState<ARunGameGameState>())
		{
			GameState->OnGameStateChanged.RemoveDynamic(this, &ARunGameCharacter::OnGameStateChangedCallback);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ARunGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunGameCharacter::Move);
		//EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ARunGameCharacter::Look);

		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARunGameCharacter::Look);
	
		//Slide
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ARunGameCharacter::StartSlide);
		//EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Completed, this, &ARunGameCharacter::EndSlide);
	
	}
	else
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameCharacter: '%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARunGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetController() == nullptr) return;

	FRotator CurrentRotation = GetController()->GetControlRotation();
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, DesireRotation, DeltaSeconds, 10.f);
	GetController()->SetControlRotation(SmoothRotation);

	// 2. �������㡿��ȡ���Ե���ǰ����
	// �������������ò�ֵ�е�ƽ���Ƕȣ������þ��Զ����ܵ��� DesireRotation��
	const FRotator YawRotation(0, DesireRotation.Yaw, 0);
	const FVector Desiron = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//��ǰ��
	AddMovementInput(Desiron, 1.0f);

}

void ARunGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ARunGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ARunGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr && Right != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//// get forward vector
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//�Ƿ���ת
		if (bTurn)
		{
			//�µ���ת
			FRotator NewRotation = FRotator(0.f, 90.f * Right, 0.f);

			//�Ƕȼ��㣬��Ԫ��
			FQuat QuatA = FQuat(DesireRotation);
			FQuat QuatB = FQuat(NewRotation);
			DesireRotation = FRotator(QuatA * QuatB);

			//Ťת��������
			if(UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
			{
				//�������㡿��ת��ǰ�ٶ�����
				FVector CurrentVelocity = MovementComponent->Velocity;
				//�������㡿��ת��ǰ�ٶ�����
				FVector NewVelocity = QuatB.RotateVector(CurrentVelocity);
				//�������㡿ֱ�������µ��ٶ�����
				MovementComponent->Velocity = NewVelocity;
			}


			bTurn = false;
		}
		else
		{
			// add movement 
			//AddMovementInput(ForwardDirection, Forward);
			if (!InTurnBox)
			{
				//const FRotator YawRotation(0, DesireRotation.Yaw, 0);
				//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				AddMovementInput(RightDirection, Right);
			}
		}

	}
}

void ARunGameCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ARunGameCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ARunGameCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ARunGameCharacter::StartSlide()
{
	if (!bIsSliding && !GetCharacterMovement()->IsFalling())
	{
		bIsSliding = true;

		Crouch();

		GetCharacterMovement()->GroundFriction = 0.0f;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &ARunGameCharacter::OnSlideBlendingOut);

			if (SlideMontage)
			{
				AnimRootMotionTranslationScale = RootMotionScale;
				AnimInstance->Montage_Play(SlideMontage, MontagePlayRate);
				UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Slide started. Playing montage: %s"), *SlideMontage->GetName());
			}
		}
	}
}


void ARunGameCharacter::EndSlide()
{
	if (bIsSliding)
	{
		bIsSliding = false;
		UnCrouch();
		AnimRootMotionTranslationScale = 1.0f;
		UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Slide ended."));
	}
}

void ARunGameCharacter::OnSlideBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == SlideMontage)
	{
		EndSlide();
		
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ARunGameCharacter::OnSlideBlendingOut);
		}
	}
}

void ARunGameCharacter::Die()
{
	// 原地生成新摄像机并绑定玩家视角
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		const FVector CamLocation = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation();
		const FRotator CamRotation = FollowCamera ? FollowCamera->GetComponentRotation() : GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (ACameraActor* DeathCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CamLocation, CamRotation, SpawnParams))
		{
			PC->SetViewTargetWithBlend(DeathCamera, 0.1f);
			PC->bAutoManageActiveCameraTarget = false;
			UE_LOG(LogRunGame, Warning, TEXT("RunGameCharacter: Death camera spawned at %s, view blended."), *CamLocation.ToString());
		}
	}

	//if (CameraBoom && CameraBoom->GetAttachParent())
	//{
	//	CameraBoom->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//}

	// UnPossess 已在 GameMode::HandlePlayerDeath 中调用，无需在此处禁用输入或碰撞

	UE_LOG(LogRunGame, Error, TEXT("RunGameCharacter: Character %s died"), *GetName());
}

void ARunGameCharacter::OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState)
{
	if (OldState == NewState)
	{
		return;
	}

	if (NewState == ERunGameGameState::MainMenu)
	{
		Destroy();
	}
}
