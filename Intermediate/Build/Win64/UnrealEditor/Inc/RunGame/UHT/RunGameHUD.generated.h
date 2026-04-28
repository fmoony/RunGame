// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "RunGameHUD.h"

#ifdef RUNGAME_RunGameHUD_generated_h
#error "RunGameHUD.generated.h already included, missing '#pragma once' in RunGameHUD.h"
#endif
#define RUNGAME_RunGameHUD_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UUserWidget;
enum class ERunGameGameState : uint8;

// ********** Begin Class ARunGameHUD **************************************************************
#define FID_RunGame_Source_RunGame_RunGameHUD_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnGameStateChangedCallback); \
	DECLARE_FUNCTION(execGetCurrentUI); \
	DECLARE_FUNCTION(execHideUI); \
	DECLARE_FUNCTION(execShowUI);


RUNGAME_API UClass* Z_Construct_UClass_ARunGameHUD_NoRegister();

#define FID_RunGame_Source_RunGame_RunGameHUD_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGameHUD(); \
	friend struct Z_Construct_UClass_ARunGameHUD_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGameHUD_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGameHUD, AHUD, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGameHUD_NoRegister) \
	DECLARE_SERIALIZER(ARunGameHUD)


#define FID_RunGame_Source_RunGame_RunGameHUD_h_18_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ARunGameHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGameHUD(ARunGameHUD&&) = delete; \
	ARunGameHUD(const ARunGameHUD&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGameHUD); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGameHUD); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ARunGameHUD) \
	NO_API virtual ~ARunGameHUD();


#define FID_RunGame_Source_RunGame_RunGameHUD_h_15_PROLOG
#define FID_RunGame_Source_RunGame_RunGameHUD_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_RunGameHUD_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameHUD_h_18_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGameHUD_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGameHUD;

// ********** End Class ARunGameHUD ****************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_RunGameHUD_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
