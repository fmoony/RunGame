// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "HUD/RunGameGameOver.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameGameOver() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_URunGameGameOver();
RUNGAME_API UClass* Z_Construct_UClass_URunGameGameOver_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class URunGameGameOver Function OnMainMenuButtonClicked ************************
struct Z_Construct_UFunction_URunGameGameOver_OnMainMenuButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Handles main menu button click, resets game to main menu */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Handles main menu button click, resets game to main menu" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameGameOver_OnMainMenuButtonClicked_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameGameOver, nullptr, "OnMainMenuButtonClicked", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameGameOver_OnMainMenuButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameGameOver_OnMainMenuButtonClicked_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameGameOver_OnMainMenuButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameGameOver_OnMainMenuButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameGameOver::execOnMainMenuButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnMainMenuButtonClicked();
	P_NATIVE_END;
}
// ********** End Class URunGameGameOver Function OnMainMenuButtonClicked **************************

// ********** Begin Class URunGameGameOver Function OnRestartButtonClicked *************************
struct Z_Construct_UFunction_URunGameGameOver_OnRestartButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Handles restart button click, starts a new game via GameMode */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Handles restart button click, starts a new game via GameMode" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameGameOver_OnRestartButtonClicked_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameGameOver, nullptr, "OnRestartButtonClicked", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameGameOver_OnRestartButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameGameOver_OnRestartButtonClicked_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameGameOver_OnRestartButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameGameOver_OnRestartButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameGameOver::execOnRestartButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnRestartButtonClicked();
	P_NATIVE_END;
}
// ********** End Class URunGameGameOver Function OnRestartButtonClicked ***************************

// ********** Begin Class URunGameGameOver Function UpdateScoreAndTime *****************************
struct Z_Construct_UFunction_URunGameGameOver_UpdateScoreAndTime_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "GameOver" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** \xe6\x9b\xb4\xe6\x96\xb0\xe5\x88\x86\xe6\x95\xb0\xe5\x92\x8c\xe6\x97\xb6\xe9\x95\xbf\xe7\x9a\x84\xe6\x98\xbe\xe7\xa4\xba\xef\xbc\x88\xe4\xbb\x8e PlayerState \xe5\x92\x8c TimerSubsystem \xe8\x8e\xb7\xe5\x8f\x96\xe6\x95\xb0\xe6\x8d\xae\xef\xbc\x89 */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x9b\xb4\xe6\x96\xb0\xe5\x88\x86\xe6\x95\xb0\xe5\x92\x8c\xe6\x97\xb6\xe9\x95\xbf\xe7\x9a\x84\xe6\x98\xbe\xe7\xa4\xba\xef\xbc\x88\xe4\xbb\x8e PlayerState \xe5\x92\x8c TimerSubsystem \xe8\x8e\xb7\xe5\x8f\x96\xe6\x95\xb0\xe6\x8d\xae\xef\xbc\x89" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameGameOver_UpdateScoreAndTime_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameGameOver, nullptr, "UpdateScoreAndTime", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameGameOver_UpdateScoreAndTime_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameGameOver_UpdateScoreAndTime_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameGameOver_UpdateScoreAndTime()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameGameOver_UpdateScoreAndTime_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameGameOver::execUpdateScoreAndTime)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->UpdateScoreAndTime();
	P_NATIVE_END;
}
// ********** End Class URunGameGameOver Function UpdateScoreAndTime *******************************

// ********** Begin Class URunGameGameOver *********************************************************
void URunGameGameOver::StaticRegisterNativesURunGameGameOver()
{
	UClass* Class = URunGameGameOver::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnMainMenuButtonClicked", &URunGameGameOver::execOnMainMenuButtonClicked },
		{ "OnRestartButtonClicked", &URunGameGameOver::execOnRestartButtonClicked },
		{ "UpdateScoreAndTime", &URunGameGameOver::execUpdateScoreAndTime },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_URunGameGameOver;
UClass* URunGameGameOver::GetPrivateStaticClass()
{
	using TClass = URunGameGameOver;
	if (!Z_Registration_Info_UClass_URunGameGameOver.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameGameOver"),
			Z_Registration_Info_UClass_URunGameGameOver.InnerSingleton,
			StaticRegisterNativesURunGameGameOver,
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
	return Z_Registration_Info_UClass_URunGameGameOver.InnerSingleton;
}
UClass* Z_Construct_UClass_URunGameGameOver_NoRegister()
{
	return URunGameGameOver::GetPrivateStaticClass();
}
struct Z_Construct_UClass_URunGameGameOver_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe6\xb8\xb8\xe6\x88\x8f\xe7\xbb\x93\xe6\x9d\x9f\xe7\x95\x8c\xe9\x9d\xa2\n * \xe9\x80\x9a\xe8\xbf\x87 GameState \xe8\x87\xaa\xe5\x8a\xa8\xe8\xa7\xa6\xe5\x8f\x91\xe6\x98\xbe\xe7\xa4\xba\xef\xbc\x8c\xe4\xbb\x8e PlayerState \xe5\x92\x8c TimerSubsystem \xe8\x8e\xb7\xe5\x8f\x96\xe6\x95\xb0\xe6\x8d\xae\n */" },
#endif
		{ "IncludePath", "HUD/RunGameGameOver.h" },
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xb8\xb8\xe6\x88\x8f\xe7\xbb\x93\xe6\x9d\x9f\xe7\x95\x8c\xe9\x9d\xa2\n\xe9\x80\x9a\xe8\xbf\x87 GameState \xe8\x87\xaa\xe5\x8a\xa8\xe8\xa7\xa6\xe5\x8f\x91\xe6\x98\xbe\xe7\xa4\xba\xef\xbc\x8c\xe4\xbb\x8e PlayerState \xe5\x92\x8c TimerSubsystem \xe8\x8e\xb7\xe5\x8f\x96\xe6\x95\xb0\xe6\x8d\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RestartButton_MetaData[] = {
		{ "BindWidget", "" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** UI \xe6\x8e\xa7\xe4\xbb\xb6\xe7\xbb\x91\xe5\xae\x9a - \xe9\x9c\x80\xe8\xa6\x81\xe5\x9c\xa8\xe8\x93\x9d\xe5\x9b\xbe\xe5\xad\x90\xe6\x8e\xa7\xe4\xbb\xb6\xe4\xb8\xad\xe5\x91\xbd\xe5\x90\x8d\xe4\xb8\x80\xe8\x87\xb4 */" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UI \xe6\x8e\xa7\xe4\xbb\xb6\xe7\xbb\x91\xe5\xae\x9a - \xe9\x9c\x80\xe8\xa6\x81\xe5\x9c\xa8\xe8\x93\x9d\xe5\x9b\xbe\xe5\xad\x90\xe6\x8e\xa7\xe4\xbb\xb6\xe4\xb8\xad\xe5\x91\xbd\xe5\x90\x8d\xe4\xb8\x80\xe8\x87\xb4" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MainMenuButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScoreText_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimeText_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameGameOver.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_RestartButton;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MainMenuButton;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ScoreText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TimeText;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_URunGameGameOver_OnMainMenuButtonClicked, "OnMainMenuButtonClicked" }, // 1857538248
		{ &Z_Construct_UFunction_URunGameGameOver_OnRestartButtonClicked, "OnRestartButtonClicked" }, // 1111610296
		{ &Z_Construct_UFunction_URunGameGameOver_UpdateScoreAndTime, "UpdateScoreAndTime" }, // 2302309439
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URunGameGameOver>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameGameOver_Statics::NewProp_RestartButton = { "RestartButton", nullptr, (EPropertyFlags)0x0124080000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameGameOver, RestartButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RestartButton_MetaData), NewProp_RestartButton_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameGameOver_Statics::NewProp_MainMenuButton = { "MainMenuButton", nullptr, (EPropertyFlags)0x0124080000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameGameOver, MainMenuButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MainMenuButton_MetaData), NewProp_MainMenuButton_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameGameOver_Statics::NewProp_ScoreText = { "ScoreText", nullptr, (EPropertyFlags)0x0124080000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameGameOver, ScoreText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScoreText_MetaData), NewProp_ScoreText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameGameOver_Statics::NewProp_TimeText = { "TimeText", nullptr, (EPropertyFlags)0x0124080000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameGameOver, TimeText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimeText_MetaData), NewProp_TimeText_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URunGameGameOver_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameGameOver_Statics::NewProp_RestartButton,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameGameOver_Statics::NewProp_MainMenuButton,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameGameOver_Statics::NewProp_ScoreText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameGameOver_Statics::NewProp_TimeText,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameGameOver_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_URunGameGameOver_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameGameOver_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_URunGameGameOver_Statics::ClassParams = {
	&URunGameGameOver::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_URunGameGameOver_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_URunGameGameOver_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameGameOver_Statics::Class_MetaDataParams), Z_Construct_UClass_URunGameGameOver_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_URunGameGameOver()
{
	if (!Z_Registration_Info_UClass_URunGameGameOver.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URunGameGameOver.OuterSingleton, Z_Construct_UClass_URunGameGameOver_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_URunGameGameOver.OuterSingleton;
}
URunGameGameOver::URunGameGameOver(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(URunGameGameOver);
URunGameGameOver::~URunGameGameOver() {}
// ********** End Class URunGameGameOver ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_URunGameGameOver, URunGameGameOver::StaticClass, TEXT("URunGameGameOver"), &Z_Registration_Info_UClass_URunGameGameOver, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URunGameGameOver), 3070125737U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h__Script_RunGame_3999174750(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameGameOver_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
