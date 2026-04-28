// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Actor/Floor/StraightFloor.h"

#ifdef RUNGAME_StraightFloor_generated_h
#error "StraightFloor.generated.h already included, missing '#pragma once' in StraightFloor.h"
#endif
#define RUNGAME_StraightFloor_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AStraightFloor ***********************************************************
RUNGAME_API UClass* Z_Construct_UClass_AStraightFloor_NoRegister();

#define FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAStraightFloor(); \
	friend struct Z_Construct_UClass_AStraightFloor_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_AStraightFloor_NoRegister(); \
public: \
	DECLARE_CLASS2(AStraightFloor, AFloorBase, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_AStraightFloor_NoRegister) \
	DECLARE_SERIALIZER(AStraightFloor)


#define FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h_12_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AStraightFloor(AStraightFloor&&) = delete; \
	AStraightFloor(const AStraightFloor&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AStraightFloor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AStraightFloor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AStraightFloor) \
	NO_API virtual ~AStraightFloor();


#define FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h_9_PROLOG
#define FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h_12_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AStraightFloor;

// ********** End Class AStraightFloor *************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
