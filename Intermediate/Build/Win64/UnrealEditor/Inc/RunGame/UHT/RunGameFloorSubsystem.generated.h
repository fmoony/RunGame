// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "WorldSubsystem/RunGameFloorSubsystem.h"

#ifdef RUNGAME_RunGameFloorSubsystem_generated_h
#error "RunGameFloorSubsystem.generated.h already included, missing '#pragma once' in RunGameFloorSubsystem.h"
#endif
#define RUNGAME_RunGameFloorSubsystem_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class AActor;
class AFloorBase;

// ********** Begin Delegate FOnFloorSystemReadyDelegate *******************************************
#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_11_DELEGATE \
RUNGAME_API void FOnFloorSystemReadyDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnFloorSystemReadyDelegate);


// ********** End Delegate FOnFloorSystemReadyDelegate *********************************************

// ********** Begin Class URunGameFloorSubsystem ***************************************************
#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnFloorClassesLoaded); \
	DECLARE_FUNCTION(execClearAllFloors); \
	DECLARE_FUNCTION(execGetPooledFloorCount); \
	DECLARE_FUNCTION(execGetActiveFloorCount); \
	DECLARE_FUNCTION(execGetNextSpawnTransform); \
	DECLARE_FUNCTION(execHideAllActiveFloors); \
	DECLARE_FUNCTION(execRecycleDistantFloors); \
	DECLARE_FUNCTION(execReturnFloor); \
	DECLARE_FUNCTION(execRequestFloorAt); \
	DECLARE_FUNCTION(execRequestNextFloor); \
	DECLARE_FUNCTION(execSpawnInitialFloors); \
	DECLARE_FUNCTION(execInitializeFloorSystem);


RUNGAME_API UClass* Z_Construct_UClass_URunGameFloorSubsystem_NoRegister();

#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesURunGameFloorSubsystem(); \
	friend struct Z_Construct_UClass_URunGameFloorSubsystem_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend RUNGAME_API UClass* Z_Construct_UClass_URunGameFloorSubsystem_NoRegister(); \
public: \
	DECLARE_CLASS2(URunGameFloorSubsystem, UWorldSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RunGame"), Z_Construct_UClass_URunGameFloorSubsystem_NoRegister) \
	DECLARE_SERIALIZER(URunGameFloorSubsystem)


#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_16_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	URunGameFloorSubsystem(URunGameFloorSubsystem&&) = delete; \
	URunGameFloorSubsystem(const URunGameFloorSubsystem&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, URunGameFloorSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(URunGameFloorSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(URunGameFloorSubsystem) \
	NO_API virtual ~URunGameFloorSubsystem();


#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_13_PROLOG
#define FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_16_INCLASS_NO_PURE_DECLS \
	FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class URunGameFloorSubsystem;

// ********** End Class URunGameFloorSubsystem *****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
