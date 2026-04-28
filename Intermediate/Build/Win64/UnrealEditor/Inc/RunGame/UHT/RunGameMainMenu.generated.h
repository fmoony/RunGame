// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HUD/RunGameMainMenu.h"

#ifdef RUNGAME_RunGameMainMenu_generated_h
#error "RunGameMainMenu.generated.h already included, missing '#pragma once' in RunGameMainMenu.h"
#endif
#define RUNGAME_RunGameMainMenu_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class URunGameMainMenu *********************************************************
#define FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnQuitClicked); \
	DECLARE_FUNCTION(execOnStartClicked); \
	DECLARE_FUNCTION(execQuitGame); \
	DECLARE_FUNCTION(execStartGame);


RUNGAME_API UClass* Z_Construct_UClass_URunGameMainMenu_NoRegister();

#define FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesURunGameMainMenu(); \
	friend struct Z_Construct_UClass_URunGameMainMenu_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_URunGameMainMenu_NoRegister(); \
public: \
	DECLARE_CLASS2(URunGameMainMenu, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_URunGameMainMenu_NoRegister) \
	DECLARE_SERIALIZER(URunGameMainMenu)


#define FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_18_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API URunGameMainMenu(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	URunGameMainMenu(URunGameMainMenu&&) = delete; \
	URunGameMainMenu(const URunGameMainMenu&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, URunGameMainMenu); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(URunGameMainMenu); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(URunGameMainMenu) \
	NO_API virtual ~URunGameMainMenu();


#define FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_15_PROLOG
#define FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_18_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class URunGameMainMenu;

// ********** End Class URunGameMainMenu ***********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
