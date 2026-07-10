#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Character/Input/RunGameInputTypes.h"
#include "RunGameInputContextComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FRunGameMoveInputChanged, const FVector2D&);
DECLARE_MULTICAST_DELEGATE_OneParam(FRunGameLookInputChanged, const FVector2D&);
DECLARE_MULTICAST_DELEGATE_OneParam(FRunGameInputCommandBuffered, ERunGameInputCommand);
DECLARE_MULTICAST_DELEGATE(FRunGameJumpReleased);
DECLARE_MULTICAST_DELEGATE_OneParam(FRunGameSkillRequested, FGameplayTag);

/**
 * 轻量输入上下文组件，承载输入管线中的短生命周期输入数据
 * Lightweight input context component that carries short-lived input data inside the input pipeline.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameInputContextComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameInputContextComponent();

	FRunGameMoveInputChanged OnMoveInputChanged;
	FRunGameLookInputChanged OnLookInputChanged;
	FRunGameInputCommandBuffered OnCommandBuffered;
	FRunGameJumpReleased OnJumpReleased;
	FRunGameSkillRequested OnSkillRequested;

	/** 输入命令缓冲超时时间 Input command buffer timeout */
	UPROPERTY(EditAnywhere, Category = "Input|Buffer")
	float CommandBufferTimeout = 0.3f;

	void SetMoveAxis(const FVector2D& InMoveAxis);
	void SetLookAxis(const FVector2D& InLookAxis);
	void BufferCommand(ERunGameInputCommand Command);
	bool ConsumeLatestCommand(ERunGameInputCommand ExpectedCommand);
	void ClearLatestCommand();
	void ExpireLatestCommand(float CurrentTime);
	void NotifyJumpReleased();
	void RequestSkill(FGameplayTag SkillTag);
	void ClearInputContext();

	FORCEINLINE const FVector2D& GetMoveAxis() const { return MoveAxis; }
	FORCEINLINE const FVector2D& GetLookAxis() const { return LookAxis; }
	FORCEINLINE ERunGameInputCommand GetLatestCommand() const { return LatestCommand.Command; }
	FORCEINLINE FGameplayTag GetRequestedSkillTag() const { return RequestedSkillTag; }

private:
	UPROPERTY()
	FVector2D MoveAxis = FVector2D::ZeroVector;

	UPROPERTY()
	FVector2D LookAxis = FVector2D::ZeroVector;

	UPROPERTY()
	FRunGameBufferedInputCommand LatestCommand;

	UPROPERTY()
	FGameplayTag RequestedSkillTag;

	bool bJumpReleasedThisFrame = false;
};
