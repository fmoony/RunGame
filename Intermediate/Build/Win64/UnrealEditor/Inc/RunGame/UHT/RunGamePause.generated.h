// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HUD/RunGamePause.h"

#ifdef RUNGAME_RunGamePause_generated_h
#error "RunGamePause.generated.h already included, missing '#pragma once' in RunGamePause.h"
#endif
#define RUNGAME_RunGamePause_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class URunGamePause ************************************************************
RUNGAME_API UClass* Z_Construct_UClass_URunGamePause_NoRegister();

#define FID_RunGame_Source_RunGame_HUD_RunGamePause_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesURunGamePause(); \
	friend struct Z_Construct_UClass_URunGamePause_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_URunGamePause_NoRegister(); \
public: \
	DECLARE_CLASS2(URunGamePause, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_URunGamePause_NoRegister) \
	DECLARE_SERIALIZER(URunGamePause)


#define FID_RunGame_Source_RunGame_HUD_RunGamePause_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API URunGamePause(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	URunGamePause(URunGamePause&&) = delete; \
	URunGamePause(const URunGamePause&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, URunGamePause); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(URunGamePause); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(URunGamePause) \
	NO_API virtual ~URunGamePause();


#define FID_RunGame_Source_RunGame_HUD_RunGamePause_h_12_PROLOG
#define FID_RunGame_Source_RunGame_HUD_RunGamePause_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_HUD_RunGamePause_h_15_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGamePause_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class URunGamePause;

// ********** End Class URunGamePause **************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_HUD_RunGamePause_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
