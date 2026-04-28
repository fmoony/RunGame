// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Actor/Volume/RunGameInteractiveVolume.h"

#ifdef RUNGAME_RunGameInteractiveVolume_generated_h
#error "RunGameInteractiveVolume.generated.h already included, missing '#pragma once' in RunGameInteractiveVolume.h"
#endif
#define RUNGAME_RunGameInteractiveVolume_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class ARunGameCharacter;

// ********** Begin Delegate FOnInteractionBegin ***************************************************
#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_14_DELEGATE \
RUNGAME_API void FOnInteractionBegin_DelegateWrapper(const FMulticastScriptDelegate& OnInteractionBegin, ARunGameCharacter* PlayerCharacter);


// ********** End Delegate FOnInteractionBegin *****************************************************

// ********** Begin Delegate FOnInteractionEnd *****************************************************
#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_15_DELEGATE \
RUNGAME_API void FOnInteractionEnd_DelegateWrapper(const FMulticastScriptDelegate& OnInteractionEnd, ARunGameCharacter* PlayerCharacter);


// ********** End Delegate FOnInteractionEnd *******************************************************

// ********** Begin Class ARunGameInteractiveVolume ************************************************
#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnPlayerLeave); \
	DECLARE_FUNCTION(execOnPlayerEnter);


#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_CALLBACK_WRAPPERS
RUNGAME_API UClass* Z_Construct_UClass_ARunGameInteractiveVolume_NoRegister();

#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGameInteractiveVolume(); \
	friend struct Z_Construct_UClass_ARunGameInteractiveVolume_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGameInteractiveVolume_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGameInteractiveVolume, AVolume, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGameInteractiveVolume_NoRegister) \
	DECLARE_SERIALIZER(ARunGameInteractiveVolume)


#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGameInteractiveVolume(ARunGameInteractiveVolume&&) = delete; \
	ARunGameInteractiveVolume(const ARunGameInteractiveVolume&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGameInteractiveVolume); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGameInteractiveVolume); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(ARunGameInteractiveVolume) \
	NO_API virtual ~ARunGameInteractiveVolume();


#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_20_PROLOG
#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_CALLBACK_WRAPPERS \
	FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h_23_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGameInteractiveVolume;

// ********** End Class ARunGameInteractiveVolume **************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
