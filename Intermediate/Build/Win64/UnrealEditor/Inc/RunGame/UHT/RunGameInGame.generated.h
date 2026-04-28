// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HUD/RunGameInGame.h"

#ifdef RUNGAME_RunGameInGame_generated_h
#error "RunGameInGame.generated.h already included, missing '#pragma once' in RunGameInGame.h"
#endif
#define RUNGAME_RunGameInGame_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class URunGameInGame ***********************************************************
#define FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnTimerUpdated); \
	DECLARE_FUNCTION(execOnScoreUpdated);


RUNGAME_API UClass* Z_Construct_UClass_URunGameInGame_NoRegister();

#define FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesURunGameInGame(); \
	friend struct Z_Construct_UClass_URunGameInGame_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_URunGameInGame_NoRegister(); \
public: \
	DECLARE_CLASS2(URunGameInGame, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_URunGameInGame_NoRegister) \
	DECLARE_SERIALIZER(URunGameInGame)


#define FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_18_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	URunGameInGame(URunGameInGame&&) = delete; \
	URunGameInGame(const URunGameInGame&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, URunGameInGame); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(URunGameInGame); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(URunGameInGame) \
	NO_API virtual ~URunGameInGame();


#define FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_15_PROLOG
#define FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_18_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameInGame_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class URunGameInGame;

// ********** End Class URunGameInGame *************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_HUD_RunGameInGame_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
