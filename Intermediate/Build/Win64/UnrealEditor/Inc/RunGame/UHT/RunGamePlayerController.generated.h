// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "RunGamePlayerController.h"

#ifdef RUNGAME_RunGamePlayerController_generated_h
#error "RunGamePlayerController.generated.h already included, missing '#pragma once' in RunGamePlayerController.h"
#endif
#define RUNGAME_RunGamePlayerController_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

enum class ERunGameGameState : uint8;

// ********** Begin Class ARunGamePlayerController *************************************************
#define FID_RunGame_Source_RunGame_RunGamePlayerController_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnGameStateChangedCallback); \
	DECLARE_FUNCTION(execSetInputModeToUIOnly);


RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerController_NoRegister();

#define FID_RunGame_Source_RunGame_RunGamePlayerController_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGamePlayerController(); \
	friend struct Z_Construct_UClass_ARunGamePlayerController_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerController_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGamePlayerController, APlayerController, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGamePlayerController_NoRegister) \
	DECLARE_SERIALIZER(ARunGamePlayerController)


#define FID_RunGame_Source_RunGame_RunGamePlayerController_h_21_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ARunGamePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGamePlayerController(ARunGamePlayerController&&) = delete; \
	ARunGamePlayerController(const ARunGamePlayerController&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGamePlayerController); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGamePlayerController); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ARunGamePlayerController) \
	NO_API virtual ~ARunGamePlayerController();


#define FID_RunGame_Source_RunGame_RunGamePlayerController_h_18_PROLOG
#define FID_RunGame_Source_RunGame_RunGamePlayerController_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_RunGamePlayerController_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGamePlayerController_h_21_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_RunGamePlayerController_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGamePlayerController;

// ********** End Class ARunGamePlayerController ***************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_RunGamePlayerController_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
