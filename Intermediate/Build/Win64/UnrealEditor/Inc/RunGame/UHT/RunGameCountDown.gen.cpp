// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "HUD/RunGameCountDown.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameCountDown() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_URunGameCountDown();
RUNGAME_API UClass* Z_Construct_UClass_URunGameCountDown_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class URunGameCountDown Function OnCountdownReceived ***************************
struct Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics
{
	struct RunGameCountDown_eventOnCountdownReceived_Parms
	{
		int32 CurrentTime;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Callback when countdown time updates, received from GameState broadcast */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameCountDown.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Callback when countdown time updates, received from GameState broadcast" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_CurrentTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::NewProp_CurrentTime = { "CurrentTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCountDown_eventOnCountdownReceived_Parms, CurrentTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::NewProp_CurrentTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameCountDown, nullptr, "OnCountdownReceived", Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::RunGameCountDown_eventOnCountdownReceived_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::RunGameCountDown_eventOnCountdownReceived_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameCountDown::execOnCountdownReceived)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_CurrentTime);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnCountdownReceived(Z_Param_CurrentTime);
	P_NATIVE_END;
}
// ********** End Class URunGameCountDown Function OnCountdownReceived *****************************

// ********** Begin Class URunGameCountDown Function PlayNumberAnimation ***************************
struct RunGameCountDown_eventPlayNumberAnimation_Parms
{
	int32 CurrentTime;
};
static FName NAME_URunGameCountDown_PlayNumberAnimation = FName(TEXT("PlayNumberAnimation"));
void URunGameCountDown::PlayNumberAnimation(int32 CurrentTime)
{
	RunGameCountDown_eventPlayNumberAnimation_Parms Parms;
	Parms.CurrentTime=CurrentTime;
	UFunction* Func = FindFunctionChecked(NAME_URunGameCountDown_PlayNumberAnimation);
	ProcessEvent(Func,&Parms);
}
struct Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Animation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Triggers the number animation in blueprint for the current countdown value */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameCountDown.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Triggers the number animation in blueprint for the current countdown value" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_CurrentTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::NewProp_CurrentTime = { "CurrentTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCountDown_eventPlayNumberAnimation_Parms, CurrentTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::NewProp_CurrentTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameCountDown, nullptr, "PlayNumberAnimation", Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::PropPointers), sizeof(RunGameCountDown_eventPlayNumberAnimation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(RunGameCountDown_eventPlayNumberAnimation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class URunGameCountDown Function PlayNumberAnimation *****************************

// ********** Begin Class URunGameCountDown ********************************************************
void URunGameCountDown::StaticRegisterNativesURunGameCountDown()
{
	UClass* Class = URunGameCountDown::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnCountdownReceived", &URunGameCountDown::execOnCountdownReceived },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_URunGameCountDown;
UClass* URunGameCountDown::GetPrivateStaticClass()
{
	using TClass = URunGameCountDown;
	if (!Z_Registration_Info_UClass_URunGameCountDown.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameCountDown"),
			Z_Registration_Info_UClass_URunGameCountDown.InnerSingleton,
			StaticRegisterNativesURunGameCountDown,
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
	return Z_Registration_Info_UClass_URunGameCountDown.InnerSingleton;
}
UClass* Z_Construct_UClass_URunGameCountDown_NoRegister()
{
	return URunGameCountDown::GetPrivateStaticClass();
}
struct Z_Construct_UClass_URunGameCountDown_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "HUD/RunGameCountDown.h" },
		{ "ModuleRelativePath", "HUD/RunGameCountDown.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_URunGameCountDown_OnCountdownReceived, "OnCountdownReceived" }, // 2382771046
		{ &Z_Construct_UFunction_URunGameCountDown_PlayNumberAnimation, "PlayNumberAnimation" }, // 1931304189
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URunGameCountDown>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_URunGameCountDown_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameCountDown_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_URunGameCountDown_Statics::ClassParams = {
	&URunGameCountDown::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameCountDown_Statics::Class_MetaDataParams), Z_Construct_UClass_URunGameCountDown_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_URunGameCountDown()
{
	if (!Z_Registration_Info_UClass_URunGameCountDown.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URunGameCountDown.OuterSingleton, Z_Construct_UClass_URunGameCountDown_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_URunGameCountDown.OuterSingleton;
}
URunGameCountDown::URunGameCountDown(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(URunGameCountDown);
URunGameCountDown::~URunGameCountDown() {}
// ********** End Class URunGameCountDown **********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_URunGameCountDown, URunGameCountDown::StaticClass, TEXT("URunGameCountDown"), &Z_Registration_Info_UClass_URunGameCountDown, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URunGameCountDown), 969404906U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h__Script_RunGame_1189689201(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameCountDown_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
