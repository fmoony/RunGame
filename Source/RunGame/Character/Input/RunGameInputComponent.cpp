#include "Character/Input/RunGameInputComponent.h"

#include "Character/RunGameCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "RunGame.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Skill/SkillComponent.h"

URunGameInputComponent::URunGameInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameInputComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheOwnerComponents();
}

void URunGameInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SkillComponent = nullptr;
	PendingRequests.Reset();
	Super::EndPlay(EndPlayReason);
}

void URunGameInputComponent::BindInput(UInputComponent* PlayerInputComponent)
{
	CacheOwnerComponents();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameInputComponent: Failed to find Enhanced Input component"));
		return;
	}

	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &URunGameInputComponent::HandleJumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &URunGameInputComponent::HandleJumpCompleted);
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &URunGameInputComponent::HandleMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &URunGameInputComponent::HandleMoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &URunGameInputComponent::HandleMoveCompleted);
	}

	if (SlideAction)
	{
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &URunGameInputComponent::HandleSlideStarted);
	}

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &URunGameInputComponent::HandleLook);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &URunGameInputComponent::HandleLookCompleted);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Canceled, this, &URunGameInputComponent::HandleLookCompleted);
	}

	if (MouseLookAction)
	{
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &URunGameInputComponent::HandleLook);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Completed, this, &URunGameInputComponent::HandleLookCompleted);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Canceled, this, &URunGameInputComponent::HandleLookCompleted);
	}

	if (SkillComponent && SkillComponent->SkillConfig)
	{
		for (const FSkillDefinition& SkillDef : SkillComponent->SkillConfig->Skills)
		{
			if (SkillDef.InputAction && SkillDef.SkillTag.IsValid())
			{
				EnhancedInputComponent->BindAction(
					SkillDef.InputAction,
					ETriggerEvent::Started,
					this,
					&URunGameInputComponent::HandleSkillStarted,
					SkillDef.SkillTag);
			}
		}
	}
}

FRunGameInputSnapshot URunGameInputComponent::GetInputSnapshot() const
{
	FRunGameInputSnapshot Snapshot;
	Snapshot.MoveAxis = MoveAxis;
	Snapshot.LookAxis = LookAxis;
	Snapshot.bJumpHeld = bJumpHeld;
	Snapshot.JumpReleaseGeneration = JumpReleaseGeneration;
	Snapshot.Requests = PendingRequests;
	return Snapshot;
}

void URunGameInputComponent::ExpireRequests(float CurrentTime)
{
	for (int32 Index = PendingRequests.Num() - 1; Index >= 0; --Index)
	{
		const FRunGameInputRequest& Request = PendingRequests[Index];
		if ((CurrentTime - Request.CreatedAt) > RequestTimeout)
		{
			UE_LOG(LogRunGame, Verbose, TEXT("Input request expired: Id=%llu Type=%d"),
				Request.RequestId, static_cast<int32>(Request.Type));
			PendingRequests.RemoveAt(Index);
		}
	}
}

bool URunGameInputComponent::RemoveRequest(uint64 RequestId)
{
	const int32 Index = PendingRequests.IndexOfByPredicate(
		[RequestId](const FRunGameInputRequest& Request)
		{
			return Request.RequestId == RequestId;
		});

	if (Index == INDEX_NONE)
	{
		return false;
	}

	PendingRequests.RemoveAt(Index);
	return true;
}

void URunGameInputComponent::ClearInputState()
{
	MoveAxis = FVector2D::ZeroVector;
	LookAxis = FVector2D::ZeroVector;
	bJumpHeld = false;
	PendingRequests.Reset();
}

void URunGameInputComponent::CacheOwnerComponents()
{
	if (!SkillComponent)
	{
		if (const ARunGameCharacter* OwnerCharacter = Cast<ARunGameCharacter>(GetOwner()))
		{
			SkillComponent = OwnerCharacter->GetSkillComponent();
		}
	}
}

void URunGameInputComponent::EnqueueRequest(ERunGameInputRequestType Type, FGameplayTag SkillTag)
{
	if (PendingRequests.Num() >= RequestCapacity)
	{
		UE_LOG(LogRunGame, Warning,
			TEXT("Input request queue full: Capacity=%d RejectedType=%d NextRequestId=%llu"),
			RequestCapacity, static_cast<int32>(Type), NextRequestId);
		return;
	}

	FRunGameInputRequest& Request = PendingRequests.AddDefaulted_GetRef();
	Request.RequestId = NextRequestId++;
	Request.Type = Type;
	Request.SkillTag = SkillTag;
	Request.CreatedAt = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
}

void URunGameInputComponent::HandleMove(const FInputActionValue& Value)
{
	MoveAxis = Value.Get<FVector2D>();
}

void URunGameInputComponent::HandleMoveCompleted()
{
	MoveAxis = FVector2D::ZeroVector;
}

void URunGameInputComponent::HandleLook(const FInputActionValue& Value)
{
	LookAxis = Value.Get<FVector2D>();
}

void URunGameInputComponent::HandleLookCompleted()
{
	LookAxis = FVector2D::ZeroVector;
}

void URunGameInputComponent::HandleJumpStarted()
{
	bJumpHeld = true;
	EnqueueRequest(ERunGameInputRequestType::Jump);
}

void URunGameInputComponent::HandleJumpCompleted()
{
	bJumpHeld = false;
	++JumpReleaseGeneration;
}

void URunGameInputComponent::HandleSlideStarted()
{
	EnqueueRequest(ERunGameInputRequestType::Slide);
}

void URunGameInputComponent::HandleSkillStarted(FGameplayTag SkillTag)
{
	EnqueueRequest(ERunGameInputRequestType::Skill, SkillTag);
}
