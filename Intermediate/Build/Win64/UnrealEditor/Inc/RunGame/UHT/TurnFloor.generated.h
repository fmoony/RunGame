// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Actor/Floor/TurnFloor.h"

#ifdef RUNGAME_TurnFloor_generated_h
#error "TurnFloor.generated.h already included, missing '#pragma once' in TurnFloor.h"
#endif
#define RUNGAME_TurnFloor_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class AActor;
class UPrimitiveComponent;
struct FHitResult;

// ********** Begin Class ATurnFloor ***************************************************************
#define FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnTurnBoxEndOverlap); \
	DECLARE_FUNCTION(execOnTurnBoxBeginOverlap);


RUNGAME_API UClass* Z_Construct_UClass_ATurnFloor_NoRegister();

#define FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesATurnFloor(); \
	friend struct Z_Construct_UClass_ATurnFloor_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ATurnFloor_NoRegister(); \
public: \
	DECLARE_CLASS2(ATurnFloor, AFloorBase, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ATurnFloor_NoRegister) \
	DECLARE_SERIALIZER(ATurnFloor)


#define FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_14_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ATurnFloor(ATurnFloor&&) = delete; \
	ATurnFloor(const ATurnFloor&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATurnFloor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATurnFloor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ATurnFloor) \
	NO_API virtual ~ATurnFloor();


#define FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_11_PROLOG
#define FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_14_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ATurnFloor;

// ********** End Class ATurnFloor *****************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_Actor_Floor_TurnFloor_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
