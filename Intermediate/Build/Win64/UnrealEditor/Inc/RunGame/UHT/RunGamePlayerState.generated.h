// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "RunGamePlayerState.h"

#ifdef RUNGAME_RunGamePlayerState_generated_h
#error "RunGamePlayerState.generated.h already included, missing '#pragma once' in RunGamePlayerState.h"
#endif
#define RUNGAME_RunGamePlayerState_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

enum class ERunGameGameState : uint8;

// ********** Begin Delegate FOnScoreChangedDelegate ***********************************************
#define FID_RunGame_Source_RunGame_RunGamePlayerState_h_12_DELEGATE \
RUNGAME_API void FOnScoreChangedDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnScoreChangedDelegate, int64 NewScore);


// ********** End Delegate FOnScoreChangedDelegate *************************************************

// ********** Begin Class ARunGamePlayerState ******************************************************
#define FID_RunGame_Source_RunGame_RunGamePlayerState_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnGameStateChangedCallback); \
	DECLARE_FUNCTION(execCalculateScoreProcess); \
	DECLARE_FUNCTION(execGetRunGameScore); \
	DECLARE_FUNCTION(execSetRunGameScore); \
	DECLARE_FUNCTION(execAddScore); \
	DECLARE_FUNCTION(execSetScoringActive);


RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerState_NoRegister();

#define FID_RunGame_Source_RunGame_RunGamePlayerState_h_22_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGamePlayerState(); \
	friend struct Z_Construct_UClass_ARunGamePlayerState_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerState_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGamePlayerState, APlayerState, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGamePlayerState_NoRegister) \
	DECLARE_SERIALIZER(ARunGamePlayerState)


#define FID_RunGame_Source_RunGame_RunGamePlayerState_h_22_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGamePlayerState(ARunGamePlayerState&&) = delete; \
	ARunGamePlayerState(const ARunGamePlayerState&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGamePlayerState); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGamePlayerState); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ARunGamePlayerState) \
	NO_API virtual ~ARunGamePlayerState();


#define FID_RunGame_Source_RunGame_RunGamePlayerState_h_19_PROLOG
#define FID_RunGame_Source_RunGame_RunGamePlayerState_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_RunGamePlayerState_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGamePlayerState_h_22_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGamePlayerState_h_22_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGamePlayerState;

// ********** End Class ARunGamePlayerState ********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_RunGamePlayerState_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
