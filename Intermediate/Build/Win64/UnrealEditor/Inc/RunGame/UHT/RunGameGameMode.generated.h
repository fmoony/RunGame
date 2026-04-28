// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "RunGameGameMode.h"

#ifdef RUNGAME_RunGameGameMode_generated_h
#error "RunGameGameMode.generated.h already included, missing '#pragma once' in RunGameGameMode.h"
#endif
#define RUNGAME_RunGameGameMode_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class ARunGameCharacter;
class ARunGameDeathVolume;

// ********** Begin Delegate FOnPlayerDeathDelegate ************************************************
#define FID_RunGame_Source_RunGame_RunGameGameMode_h_11_DELEGATE \
RUNGAME_API void FOnPlayerDeathDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnPlayerDeathDelegate, ARunGameCharacter* PlayerCharacter);


// ********** End Delegate FOnPlayerDeathDelegate **************************************************

// ********** Begin Class ARunGameGameMode *********************************************************
#define FID_RunGame_Source_RunGame_RunGameGameMode_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnFloorSystemReadyCallback); \
	DECLARE_FUNCTION(execHandlePlayerDeath); \
	DECLARE_FUNCTION(execStartNewGame); \
	DECLARE_FUNCTION(execResetGame); \
	DECLARE_FUNCTION(execInitializeGameTimer); \
	DECLARE_FUNCTION(execSpawnPlayer); \
	DECLARE_FUNCTION(execStartGameCountDown);


RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameMode_NoRegister();

#define FID_RunGame_Source_RunGame_RunGameGameMode_h_19_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGameGameMode(); \
	friend struct Z_Construct_UClass_ARunGameGameMode_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameMode_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGameGameMode, AGameModeBase, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGameGameMode_NoRegister) \
	DECLARE_SERIALIZER(ARunGameGameMode)


#define FID_RunGame_Source_RunGame_RunGameGameMode_h_19_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGameGameMode(ARunGameGameMode&&) = delete; \
	ARunGameGameMode(const ARunGameGameMode&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGameGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGameGameMode); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(ARunGameGameMode) \
	NO_API virtual ~ARunGameGameMode();


#define FID_RunGame_Source_RunGame_RunGameGameMode_h_16_PROLOG
#define FID_RunGame_Source_RunGame_RunGameGameMode_h_19_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_RunGameGameMode_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameGameMode_h_19_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameGameMode_h_19_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGameGameMode;

// ********** End Class ARunGameGameMode ***********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_RunGameGameMode_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
