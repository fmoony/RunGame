// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "RunGameCharacter.h"

#ifdef RUNGAME_RunGameCharacter_generated_h
#error "RunGameCharacter.generated.h already included, missing '#pragma once' in RunGameCharacter.h"
#endif
#define RUNGAME_RunGameCharacter_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UAnimMontage;
enum class ERunGameGameState : uint8;

// ********** Begin Class ARunGameCharacter ********************************************************
#define FID_RunGame_Source_RunGame_RunGameCharacter_h_26_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnGameStateChangedCallback); \
	DECLARE_FUNCTION(execOnSlideBlendingOut); \
	DECLARE_FUNCTION(execDoJumpEnd); \
	DECLARE_FUNCTION(execDoJumpStart); \
	DECLARE_FUNCTION(execDoLook); \
	DECLARE_FUNCTION(execDoMove); \
	DECLARE_FUNCTION(execDie);


RUNGAME_API UClass* Z_Construct_UClass_ARunGameCharacter_NoRegister();

#define FID_RunGame_Source_RunGame_RunGameCharacter_h_26_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGameCharacter(); \
	friend struct Z_Construct_UClass_ARunGameCharacter_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGameCharacter_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGameCharacter, ACharacter, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGameCharacter_NoRegister) \
	DECLARE_SERIALIZER(ARunGameCharacter)


#define FID_RunGame_Source_RunGame_RunGameCharacter_h_26_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGameCharacter(ARunGameCharacter&&) = delete; \
	ARunGameCharacter(const ARunGameCharacter&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGameCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGameCharacter); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(ARunGameCharacter) \
	NO_API virtual ~ARunGameCharacter();


#define FID_RunGame_Source_RunGame_RunGameCharacter_h_23_PROLOG
#define FID_RunGame_Source_RunGame_RunGameCharacter_h_26_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_RunGameCharacter_h_26_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameCharacter_h_26_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameCharacter_h_26_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGameCharacter;

// ********** End Class ARunGameCharacter **********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_RunGameCharacter_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
