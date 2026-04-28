// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "RunGameType.h"

#ifdef RUNGAME_RunGameType_generated_h
#error "RunGameType.generated.h already included, missing '#pragma once' in RunGameType.h"
#endif
#define RUNGAME_RunGameType_generated_h

#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_RunGameType_h

// ********** Begin Enum FFloorType ****************************************************************
#define FOREACH_ENUM_FFLOORTYPE(op) \
	op(FFloorType::StraightFloor) \
	op(FFloorType::TurnFloor) \
	op(FFloorType::UpAndDownFloor) 

enum class FFloorType : uint8;
template<> struct TIsUEnumClass<FFloorType> { enum { Value = true }; };
template<> RUNGAME_API UEnum* StaticEnum<FFloorType>();
// ********** End Enum FFloorType ******************************************************************

// ********** Begin Enum ERunGameGameState *********************************************************
#define FOREACH_ENUM_ERUNGAMEGAMESTATE(op) \
	op(ERunGameGameState::MainMenu) \
	op(ERunGameGameState::CountDown) \
	op(ERunGameGameState::InGame) \
	op(ERunGameGameState::Pause) \
	op(ERunGameGameState::GameOver) 

enum class ERunGameGameState : uint8;
template<> struct TIsUEnumClass<ERunGameGameState> { enum { Value = true }; };
template<> RUNGAME_API UEnum* StaticEnum<ERunGameGameState>();
// ********** End Enum ERunGameGameState ***********************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
