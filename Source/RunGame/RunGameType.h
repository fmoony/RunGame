#pragma once

#include "CoreMinimal.h"
#include "RunGameType.generated.h"

UENUM()
enum  class FFloorType :uint8
{
	StraightFloor,
	TurnFloor,
	UpAndDownFloor,
	MAX,
};

UENUM()
enum class ERunGameGameState : uint8
{
	MainMenu,
	CountDown,
	InGame,
	Pause,
	GameOver,
	MAX,
};