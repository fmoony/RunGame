// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "WorldSubsystem/RunGameTimerSubsystem.h"

#ifdef RUNGAME_RunGameTimerSubsystem_generated_h
#error "RunGameTimerSubsystem.generated.h already included, missing '#pragma once' in RunGameTimerSubsystem.h"
#endif
#define RUNGAME_RunGameTimerSubsystem_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

enum class ERunGameGameState : uint8;

// ********** Begin Delegate FOnCountdownCompleteDelegate ******************************************
#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_11_DELEGATE \
RUNGAME_API void FOnCountdownCompleteDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnCountdownCompleteDelegate);


// ********** End Delegate FOnCountdownCompleteDelegate ********************************************

// ********** Begin Delegate FOnTimeChangedDelegate ************************************************
#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_12_DELEGATE \
RUNGAME_API void FOnTimeChangedDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnTimeChangedDelegate, float NewTime);


// ********** End Delegate FOnTimeChangedDelegate **************************************************

// ********** Begin Class URunGameTimerSubsystem ***************************************************
#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_25_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnGameStateChangedCallback); \
	DECLARE_FUNCTION(execIsTimerRunning); \
	DECLARE_FUNCTION(execGetTotalTimeSeconds);


RUNGAME_API UClass* Z_Construct_UClass_URunGameTimerSubsystem_NoRegister();

#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_25_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesURunGameTimerSubsystem(); \
	friend struct Z_Construct_UClass_URunGameTimerSubsystem_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_URunGameTimerSubsystem_NoRegister(); \
public: \
	DECLARE_CLASS2(URunGameTimerSubsystem, UWorldSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_URunGameTimerSubsystem_NoRegister) \
	DECLARE_SERIALIZER(URunGameTimerSubsystem)


#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_25_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	URunGameTimerSubsystem(URunGameTimerSubsystem&&) = delete; \
	URunGameTimerSubsystem(const URunGameTimerSubsystem&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, URunGameTimerSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(URunGameTimerSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(URunGameTimerSubsystem) \
	NO_API virtual ~URunGameTimerSubsystem();


#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_22_PROLOG
#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_25_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_25_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_25_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h_25_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class URunGameTimerSubsystem;

// ********** End Class URunGameTimerSubsystem *****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
