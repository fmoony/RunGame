// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Actor/Floor.h"

#ifdef RUNGAME_Floor_generated_h
#error "Floor.generated.h already included, missing '#pragma once' in Floor.h"
#endif
#define RUNGAME_Floor_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AFloor *******************************************************************
RUNGAME_API UClass* Z_Construct_UClass_AFloor_NoRegister();

#define FID_RunGame_Source_RunGame_Actor_Floor_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAFloor(); \
	friend struct Z_Construct_UClass_AFloor_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_AFloor_NoRegister(); \
public: \
	DECLARE_CLASS2(AFloor, AFloorBase, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_AFloor_NoRegister) \
	DECLARE_SERIALIZER(AFloor)


#define FID_RunGame_Source_RunGame_Actor_Floor_h_16_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AFloor(AFloor&&) = delete; \
	AFloor(const AFloor&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AFloor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AFloor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AFloor) \
	NO_API virtual ~AFloor();


#define FID_RunGame_Source_RunGame_Actor_Floor_h_13_PROLOG
#define FID_RunGame_Source_RunGame_Actor_Floor_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_Actor_Floor_h_16_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Floor_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AFloor;

// ********** End Class AFloor *********************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_Actor_Floor_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
