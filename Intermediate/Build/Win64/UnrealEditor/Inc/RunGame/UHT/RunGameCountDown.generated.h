// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HUD/RunGameCountDown.h"

#ifdef RUNGAME_RunGameCountDown_generated_h
#error "RunGameCountDown.generated.h already included, missing '#pragma once' in RunGameCountDown.h"
#endif
#define RUNGAME_RunGameCountDown_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class URunGameCountDown ********************************************************
#define FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnCountdownReceived);


#define FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_CALLBACK_WRAPPERS
RUNGAME_API UClass* Z_Construct_UClass_URunGameCountDown_NoRegister();

#define FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesURunGameCountDown(); \
	friend struct Z_Construct_UClass_URunGameCountDown_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_URunGameCountDown_NoRegister(); \
public: \
	DECLARE_CLASS2(URunGameCountDown, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_URunGameCountDown_NoRegister) \
	DECLARE_SERIALIZER(URunGameCountDown)


#define FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API URunGameCountDown(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	URunGameCountDown(URunGameCountDown&&) = delete; \
	URunGameCountDown(const URunGameCountDown&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, URunGameCountDown); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(URunGameCountDown); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(URunGameCountDown) \
	NO_API virtual ~URunGameCountDown();


#define FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_12_PROLOG
#define FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_CALLBACK_WRAPPERS \
	FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class URunGameCountDown;

// ********** End Class URunGameCountDown **********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
