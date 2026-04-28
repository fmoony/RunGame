// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Floor/FloorBase.h"
#include "Floor.generated.h"

/**
 * 保留 AFloor 作为兼容壳：继承 AFloorBase，无额外逻辑。
 * 新地板类型请使用 AStraightFloor / ATurnFloor。
 */
UCLASS()
class RUNGAME_API AFloor : public AFloorBase
{
	GENERATED_BODY()

public:
	AFloor();
};
