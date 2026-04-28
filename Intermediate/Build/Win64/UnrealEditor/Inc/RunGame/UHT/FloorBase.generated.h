// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Actor/Floor/FloorBase.h"

#ifdef RUNGAME_FloorBase_generated_h
#error "FloorBase.generated.h already included, missing '#pragma once' in FloorBase.h"
#endif
#define RUNGAME_FloorBase_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class AActor;
class UPrimitiveComponent;
struct FHitResult;

// ********** Begin Class AFloorBase ***************************************************************
#define FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execBoxOverlap);


RUNGAME_API UClass* Z_Construct_UClass_AFloorBase_NoRegister();

#define FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAFloorBase(); \
	friend struct Z_Construct_UClass_AFloorBase_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_AFloorBase_NoRegister(); \
public: \
	DECLARE_CLASS2(AFloorBase, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_AFloorBase_NoRegister) \
	DECLARE_SERIALIZER(AFloorBase)


#define FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_18_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AFloorBase(AFloorBase&&) = delete; \
	AFloorBase(const AFloorBase&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AFloorBase); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AFloorBase); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AFloorBase) \
	NO_API virtual ~AFloorBase();


#define FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_15_PROLOG
#define FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_18_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AFloorBase;

// ********** End Class AFloorBase *****************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
