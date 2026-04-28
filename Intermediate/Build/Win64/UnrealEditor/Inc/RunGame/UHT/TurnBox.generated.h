// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Actor/TurnBox.h"

#ifdef RUNGAME_TurnBox_generated_h
#error "TurnBox.generated.h already included, missing '#pragma once' in TurnBox.h"
#endif
#define RUNGAME_TurnBox_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class AActor;
class UPrimitiveComponent;
struct FHitResult;

// ********** Begin Class ATurnBox *****************************************************************
#define FID_RunGame_Source_RunGame_Actor_TurnBox_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execCharaterOverlapEnd); \
	DECLARE_FUNCTION(execCharaterOverlapStart);


RUNGAME_API UClass* Z_Construct_UClass_ATurnBox_NoRegister();

#define FID_RunGame_Source_RunGame_Actor_TurnBox_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesATurnBox(); \
	friend struct Z_Construct_UClass_ATurnBox_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ATurnBox_NoRegister(); \
public: \
	DECLARE_CLASS2(ATurnBox, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ATurnBox_NoRegister) \
	DECLARE_SERIALIZER(ATurnBox)


#define FID_RunGame_Source_RunGame_Actor_TurnBox_h_14_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ATurnBox(ATurnBox&&) = delete; \
	ATurnBox(const ATurnBox&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATurnBox); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATurnBox); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ATurnBox) \
	NO_API virtual ~ATurnBox();


#define FID_RunGame_Source_RunGame_Actor_TurnBox_h_11_PROLOG
#define FID_RunGame_Source_RunGame_Actor_TurnBox_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_Actor_TurnBox_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_TurnBox_h_14_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_TurnBox_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ATurnBox;

// ********** End Class ATurnBox *******************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_Actor_TurnBox_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
