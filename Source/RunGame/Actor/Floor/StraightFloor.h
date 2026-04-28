// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Floor/FloorBase.h"
#include "StraightFloor.generated.h"

UCLASS()
class RUNGAME_API AStraightFloor : public AFloorBase
{
	GENERATED_BODY()

public:
	/** Constructs a straight floor segment */
	AStraightFloor();
};
