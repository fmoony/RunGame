// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Actor/Volume/RunGameDeathVolume.h"

#ifdef RUNGAME_RunGameDeathVolume_generated_h
#error "RunGameDeathVolume.generated.h already included, missing '#pragma once' in RunGameDeathVolume.h"
#endif
#define RUNGAME_RunGameDeathVolume_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class ARunGameDeathVolume ******************************************************
RUNGAME_API UClass* Z_Construct_UClass_ARunGameDeathVolume_NoRegister();

#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARunGameDeathVolume(); \
	friend struct Z_Construct_UClass_ARunGameDeathVolume_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_ARunGameDeathVolume_NoRegister(); \
public: \
	DECLARE_CLASS2(ARunGameDeathVolume, ARunGameInteractiveVolume, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_ARunGameDeathVolume_NoRegister) \
	DECLARE_SERIALIZER(ARunGameDeathVolume)


#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h_17_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ARunGameDeathVolume(ARunGameDeathVolume&&) = delete; \
	ARunGameDeathVolume(const ARunGameDeathVolume&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARunGameDeathVolume); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARunGameDeathVolume); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ARunGameDeathVolume) \
	NO_API virtual ~ARunGameDeathVolume();


#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h_14_PROLOG
#define FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h_17_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ARunGameDeathVolume;

// ********** End Class ARunGameDeathVolume ********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
