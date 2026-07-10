#include "Character/Input/RunGameInputComponent.h"

#include "Character/Input/RunGameInputContextComponent.h"
#include "Character/Pipeline/RunGameControlPipelineComponent.h"
#include "Character/RunGameCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Skill/RunGameSkillConfigData.h"
#include "Skill/SkillComponent.h"
#include "RunGame.h"

URunGameInputComponent::URunGameInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameInputComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	CacheOwnerComponents();
}

void URunGameInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OwnerCharacter = nullptr;
	InputContext = nullptr;
	ControlPipeline = nullptr;
	SkillComponent = nullptr;

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
	}

	if (SlideAction)
	{
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &URunGameInputComponent::HandleSlideStarted);
	}

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &URunGameInputComponent::HandleLook);
	}

	if (MouseLookAction)
	{
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &URunGameInputComponent::HandleLook);
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

void URunGameInputComponent::BufferInput(ERunGameInputCommand Command)
{
	CacheOwnerComponents();

	if (InputContext)
	{
		InputContext->BufferCommand(Command);
		if (ControlPipeline)
		{
			ControlPipeline->ProcessInputFrame();
		}
	}
}

void URunGameInputComponent::ClearBuffer()
{
	if (InputContext)
	{
		InputContext->ClearInputContext();
	}
}

void URunGameInputComponent::CacheOwnerComponents()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ARunGameCharacter>(GetOwner());
	}

	if (!OwnerCharacter)
	{
		return;
	}

	if (!InputContext)
	{
		InputContext = OwnerCharacter->FindComponentByClass<URunGameInputContextComponent>();
	}

	if (!SkillComponent)
	{
		SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	}

	if (!ControlPipeline)
	{
		ControlPipeline = OwnerCharacter->FindComponentByClass<URunGameControlPipelineComponent>();
	}
}

void URunGameInputComponent::HandleMove(const FInputActionValue& Value)
{
	CacheOwnerComponents();

	if (InputContext)
	{
		InputContext->SetMoveAxis(Value.Get<FVector2D>());
		if (ControlPipeline)
		{
			ControlPipeline->ProcessInputFrame();
		}
	}
}

void URunGameInputComponent::HandleLook(const FInputActionValue& Value)
{
	CacheOwnerComponents();

	if (InputContext)
	{
		InputContext->SetLookAxis(Value.Get<FVector2D>());
		if (ControlPipeline)
		{
			ControlPipeline->ProcessInputFrame();
		}
	}
}

void URunGameInputComponent::HandleJumpStarted()
{
	BufferInput(ERunGameInputCommand::Jump);
}

void URunGameInputComponent::HandleJumpCompleted()
{
	CacheOwnerComponents();

	if (InputContext)
	{
		InputContext->NotifyJumpReleased();
		if (ControlPipeline)
		{
			ControlPipeline->ProcessInputFrame();
		}
	}
}

void URunGameInputComponent::HandleSlideStarted()
{
	BufferInput(ERunGameInputCommand::Slide);
}

void URunGameInputComponent::HandleSkillStarted(FGameplayTag SkillTag)
{
	CacheOwnerComponents();

	if (InputContext)
	{
		InputContext->RequestSkill(SkillTag);
		if (ControlPipeline)
		{
			ControlPipeline->ProcessInputFrame();
		}
	}
}
