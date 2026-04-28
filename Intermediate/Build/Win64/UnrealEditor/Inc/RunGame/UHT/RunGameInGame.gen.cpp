// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "HUD/RunGameInGame.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameInGame() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_URunGameInGame();
RUNGAME_API UClass* Z_Construct_UClass_URunGameInGame_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_URunGameTimerSubsystem_NoRegister();
UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class URunGameInGame Function OnScoreUpdated ***********************************
struct Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics
{
	struct RunGameInGame_eventOnScoreUpdated_Parms
	{
		int64 NewScore;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Updates the score text display when player score changes */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameInGame.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Updates the score text display when player score changes" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_NewScore;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::NewProp_NewScore = { "NewScore", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameInGame_eventOnScoreUpdated_Parms, NewScore), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::NewProp_NewScore,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameInGame, nullptr, "OnScoreUpdated", Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::RunGameInGame_eventOnScoreUpdated_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::RunGameInGame_eventOnScoreUpdated_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameInGame_OnScoreUpdated()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameInGame_OnScoreUpdated_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameInGame::execOnScoreUpdated)
{
	P_GET_PROPERTY(FInt64Property,Z_Param_NewScore);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnScoreUpdated(Z_Param_NewScore);
	P_NATIVE_END;
}
// ********** End Class URunGameInGame Function OnScoreUpdated *************************************

// ********** Begin Class URunGameInGame Function OnTimerUpdated ***********************************
struct Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics
{
	struct RunGameInGame_eventOnTimerUpdated_Parms
	{
		float NewTime;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Updates the timer text display when game time changes */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameInGame.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Updates the timer text display when game time changes" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::NewProp_NewTime = { "NewTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameInGame_eventOnTimerUpdated_Parms, NewTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::NewProp_NewTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameInGame, nullptr, "OnTimerUpdated", Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::RunGameInGame_eventOnTimerUpdated_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::RunGameInGame_eventOnTimerUpdated_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameInGame_OnTimerUpdated()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameInGame_OnTimerUpdated_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameInGame::execOnTimerUpdated)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_NewTime);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnTimerUpdated(Z_Param_NewTime);
	P_NATIVE_END;
}
// ********** End Class URunGameInGame Function OnTimerUpdated *************************************

// ********** Begin Class URunGameInGame ***********************************************************
void URunGameInGame::StaticRegisterNativesURunGameInGame()
{
	UClass* Class = URunGameInGame::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnScoreUpdated", &URunGameInGame::execOnScoreUpdated },
		{ "OnTimerUpdated", &URunGameInGame::execOnTimerUpdated },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_URunGameInGame;
UClass* URunGameInGame::GetPrivateStaticClass()
{
	using TClass = URunGameInGame;
	if (!Z_Registration_Info_UClass_URunGameInGame.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameInGame"),
			Z_Registration_Info_UClass_URunGameInGame.InnerSingleton,
			StaticRegisterNativesURunGameInGame,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_URunGameInGame.InnerSingleton;
}
UClass* Z_Construct_UClass_URunGameInGame_NoRegister()
{
	return URunGameInGame::GetPrivateStaticClass();
}
struct Z_Construct_UClass_URunGameInGame_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe6\xb8\xb8\xe6\x88\x8f\xe4\xb8\xadUI - \xe6\x98\xbe\xe7\xa4\xba\xe5\xae\x9e\xe6\x97\xb6\xe5\x88\x86\xe6\x95\xb0\xe5\x92\x8c\xe8\xae\xa1\xe6\x97\xb6\n */" },
#endif
		{ "IncludePath", "HUD/RunGameInGame.h" },
		{ "ModuleRelativePath", "HUD/RunGameInGame.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xb8\xb8\xe6\x88\x8f\xe4\xb8\xadUI - \xe6\x98\xbe\xe7\xa4\xba\xe5\xae\x9e\xe6\x97\xb6\xe5\x88\x86\xe6\x95\xb0\xe5\x92\x8c\xe8\xae\xa1\xe6\x97\xb6" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScoreText_MetaData[] = {
		{ "BindWidget", "" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// UI\xe7\xbb\x84\xe4\xbb\xb6 - \xe4\xbd\xbf\xe7\x94\xa8""BindWidget\xe5\x85\x83\xe6\x95\xb0\xe6\x8d\xae\xe7\xbb\x91\xe5\xae\x9a\xe5\x88\xb0\xe8\x93\x9d\xe5\x9b\xbe\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameInGame.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UI\xe7\xbb\x84\xe4\xbb\xb6 - \xe4\xbd\xbf\xe7\x94\xa8""BindWidget\xe5\x85\x83\xe6\x95\xb0\xe6\x8d\xae\xe7\xbb\x91\xe5\xae\x9a\xe5\x88\xb0\xe8\x93\x9d\xe5\x9b\xbe" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerText_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameInGame.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerSubsystem_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Timer subsystem reference for time display updates */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameInGame.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Timer subsystem reference for time display updates" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ScoreText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TimerText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TimerSubsystem;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_URunGameInGame_OnScoreUpdated, "OnScoreUpdated" }, // 4274198471
		{ &Z_Construct_UFunction_URunGameInGame_OnTimerUpdated, "OnTimerUpdated" }, // 2148363734
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URunGameInGame>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameInGame_Statics::NewProp_ScoreText = { "ScoreText", nullptr, (EPropertyFlags)0x0144000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameInGame, ScoreText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScoreText_MetaData), NewProp_ScoreText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameInGame_Statics::NewProp_TimerText = { "TimerText", nullptr, (EPropertyFlags)0x0144000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameInGame, TimerText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerText_MetaData), NewProp_TimerText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameInGame_Statics::NewProp_TimerSubsystem = { "TimerSubsystem", nullptr, (EPropertyFlags)0x0144000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameInGame, TimerSubsystem), Z_Construct_UClass_URunGameTimerSubsystem_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerSubsystem_MetaData), NewProp_TimerSubsystem_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URunGameInGame_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameInGame_Statics::NewProp_ScoreText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameInGame_Statics::NewProp_TimerText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameInGame_Statics::NewProp_TimerSubsystem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameInGame_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_URunGameInGame_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameInGame_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_URunGameInGame_Statics::ClassParams = {
	&URunGameInGame::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_URunGameInGame_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_URunGameInGame_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameInGame_Statics::Class_MetaDataParams), Z_Construct_UClass_URunGameInGame_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_URunGameInGame()
{
	if (!Z_Registration_Info_UClass_URunGameInGame.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URunGameInGame.OuterSingleton, Z_Construct_UClass_URunGameInGame_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_URunGameInGame.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(URunGameInGame);
URunGameInGame::~URunGameInGame() {}
// ********** End Class URunGameInGame *************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameInGame_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_URunGameInGame, URunGameInGame::StaticClass, TEXT("URunGameInGame"), &Z_Registration_Info_UClass_URunGameInGame, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URunGameInGame), 194076333U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameInGame_h__Script_RunGame_2772149394(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameInGame_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameInGame_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
