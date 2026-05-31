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

/** 角色核心状态 —— 互斥的移动/生命状态。遵循与 ERunGameGameState 相同的状态机模式 */
UENUM(BlueprintType)
enum class ERunGameCharacterState : uint8
{
	Idle,        // 默认地面状态：行走、跑步、站立
	CoyoteTime,  // 土狼时间：走下边缘后的短暂缓冲（0.15s），期间仍可跳跃
	Airborne,    // 空中状态：跳跃、下落 —— 支持二段跳（bAirJumpAvailable）
	Sliding,     // 滑铲中 —— 蹲伏、无地面摩擦力、蒙太奇播放中
	Turning,     // 处于转弯检测区域内 —— 横向移动被阻止，旋转锁定
	Dead,        // 生命值耗尽 —— 所有输入和移动被阻止，溶解中
	MAX,         // 哨兵值，也用作「无缓冲输入」
};