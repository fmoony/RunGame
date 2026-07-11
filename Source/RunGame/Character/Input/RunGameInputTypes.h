#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RunGameInputTypes.generated.h"

/** 离散输入请求类型 / Discrete input request type */
UENUM()
enum class ERunGameInputRequestType : uint8
{
	Jump,
	Slide,
	Skill,
};

/** 领域执行输入请求后的结果 / Result returned by a domain after evaluating an input request */
UENUM()
enum class ERunGameInputRequestResult : uint8
{
	Applied,
	Deferred,
	Rejected,
};

/** 等待控制管线处理的离散输入请求 / Discrete input request waiting for the control pipeline */
USTRUCT()
struct FRunGameInputRequest
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 RequestId = 0;

	UPROPERTY()
	ERunGameInputRequestType Type = ERunGameInputRequestType::Jump;

	UPROPERTY()
	FGameplayTag SkillTag;

	UPROPERTY()
	float CreatedAt = 0.0f;
};

/** 输入组件提供给控制管线的只读快照 / Read-only input snapshot exposed to the control pipeline */
USTRUCT()
struct FRunGameInputSnapshot
{
	GENERATED_BODY()

	UPROPERTY()
	FVector2D MoveAxis = FVector2D::ZeroVector;

	UPROPERTY()
	FVector2D LookAxis = FVector2D::ZeroVector;

	UPROPERTY()
	bool bJumpHeld = false;

	UPROPERTY()
	uint64 JumpReleaseGeneration = 0;

	UPROPERTY()
	TArray<FRunGameInputRequest> Requests;
};
