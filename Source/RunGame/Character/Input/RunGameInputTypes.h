#pragma once

#include "CoreMinimal.h"
#include "RunGameInputTypes.generated.h"

/** 输入命令类型，与角色状态分离，仅表达玩家意图 Input command, separated from character state and only expressing player intent */
UENUM()
enum class ERunGameInputCommand : uint8
{
	None,
	Jump,
	Slide,
};

/** 缓冲输入命令，记录意图和产生时间 Buffered input command, recording intent and timestamp */
USTRUCT()
struct FRunGameBufferedInputCommand
{
	GENERATED_BODY()

	UPROPERTY()
	ERunGameInputCommand Command = ERunGameInputCommand::None;

	UPROPERTY()
	float Timestamp = 0.0f;

	bool IsExpired(float CurrentTime, float Timeout) const
	{
		return (CurrentTime - Timestamp) > Timeout;
	}
};
