// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "RunGameGameState.h"

#ifdef RUNGAME_RunGameGameState_generated_h
#error "RunGameGameState.generated.h already included, missing '#pragma once' in RunGameGameState.h"
#endif
#define RUNGAME_RunGameGameState_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

enum class ERunGameGameState : uint8;

// ********** Begin Delegate FOnCharacterDeathDelegate *********************************************
#define FID_RunGame_Source_RunGame_RunGameGameState_h_10_DELEGATE \
RUNGAME_API void FOnCharacterDeathDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnCharacterDeathDelegate);


// ********** End Delegate FOnCharacterDeathDelegate ***********************************************

// ********** Begin Delegate FOnGameStateChangedSignature ******************************************
#define FID_RunGame_Source_RunGame_RunGameGameState_h_11_DELEGATE \
RUNGAME_API void FOnGameStateChangedSignature_DelegateWrapper(const FMulticastScriptDelegate& OnGameStateChangedSignature, ERunGameGameState OldGameState, ERunGameGameState NewGameState);


// ********** End Delegate FOnGameStateChangedSignature ********************************************

// ********** Begin Delegate FOnCountdownUpdatedSignature ******************************************
#define FID_RunGame_Source_RunGame_RunGameGameState_h_12_DELEGATE \
RUNGAME_API void FOnCountdownUpdatedSignature_DelegateWrapper(const FMulticastScriptDelegate& OnCountdownUpdatedSignature, int32 CountdownSeconds);


// ********** End Delegate FOnCountdownUpdatedSignature ********************************************

// ********** Begin Class ARunGameGameState ********************************************************
#define FID_RunGame_Source_RunGame_RunGameGameState_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetCountdownSeconds); \
	DECLARE_FUNCTION(execSetCountdownSeconds); \
	DECLARE_FUNCTION(execGetCurrentState); \
	DECLARE_FUNCTION(execSetGameState);


RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameState_NoRegister();

#define FID_RunGame_Source_RunGame_RunGameGameState_h_22_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGameGameState(); \
	friend struct Z_Construct_UClass_ARunGameGameState_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameState_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGameGameState, AGameStateBase, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGameGameState_NoRegister) \
	DECLARE_SERIALIZER(ARunGameGameState)


#define FID_RunGame_Source_RunGame_RunGameGameState_h_22_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGameGameState(ARunGameGameState&&) = delete; \
	ARunGameGameState(const ARunGameGameState&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGameGameState); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGameGameState); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ARunGameGameState) \
	NO_API virtual ~ARunGameGameState();


#define FID_RunGame_Source_RunGame_RunGameGameState_h_19_PROLOG
#define FID_RunGame_Source_RunGame_RunGameGameState_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_RunGameGameState_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameGameState_h_22_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameGameState_h_22_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGameGameState;

// ********** End Class ARunGameGameState **********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_RunGameGameState_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
