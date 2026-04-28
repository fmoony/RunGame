// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HUD/RunGameGameOver.h"

#ifdef RUNGAME_RunGameGameOver_generated_h
#error "RunGameGameOver.generated.h already included, missing '#pragma once' in RunGameGameOver.h"
#endif
#define RUNGAME_RunGameGameOver_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class URunGameGameOver *********************************************************
#define FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnMainMenuButtonClicked); \
	DECLARE_FUNCTION(execOnRestartButtonClicked); \
	DECLARE_FUNCTION(execUpdateScoreAndTime);


RUNGAME_API UClass* Z_Construct_UClass_URunGameGameOver_NoRegister();

#define FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesURunGameGameOver(); \
	friend struct Z_Construct_UClass_URunGameGameOver_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_URunGameGameOver_NoRegister(); \
public: \
	DECLARE_CLASS2(URunGameGameOver, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_URunGameGameOver_NoRegister) \
	DECLARE_SERIALIZER(URunGameGameOver)


#define FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_21_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API URunGameGameOver(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	URunGameGameOver(URunGameGameOver&&) = delete; \
	URunGameGameOver(const URunGameGameOver&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, URunGameGameOver); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(URunGameGameOver); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(URunGameGameOver) \
	NO_API virtual ~URunGameGameOver();


#define FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_18_PROLOG
#define FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_21_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class URunGameGameOver;

// ********** End Class URunGameGameOver ***********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
