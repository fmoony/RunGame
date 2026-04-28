// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RunGameHUD.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameHUD() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
ENGINE_API UClass* Z_Construct_UClass_AHUD();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameHUD();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameHUD_NoRegister();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState();
UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ARunGameHUD Function GetCurrentUI ****************************************
struct Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics
{
	struct RunGameHUD_eventGetCurrentUI_Parms
	{
		UUserWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UI" },
		{ "ModuleRelativePath", "RunGameHUD.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameHUD_eventGetCurrentUI_Parms, ReturnValue), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameHUD, nullptr, "GetCurrentUI", Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::RunGameHUD_eventGetCurrentUI_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::RunGameHUD_eventGetCurrentUI_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameHUD_GetCurrentUI()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameHUD_GetCurrentUI_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameHUD::execGetCurrentUI)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUserWidget**)Z_Param__Result=P_THIS->GetCurrentUI();
	P_NATIVE_END;
}
// ********** End Class ARunGameHUD Function GetCurrentUI ******************************************

// ********** Begin Class ARunGameHUD Function HideUI **********************************************
struct Z_Construct_UFunction_ARunGameHUD_HideUI_Statics
{
	struct RunGameHUD_eventHideUI_Parms
	{
		TSubclassOf<UUserWidget> UIClass;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UI" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Removes the specified UI widget from the viewport if currently displayed */" },
#endif
		{ "ModuleRelativePath", "RunGameHUD.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Removes the specified UI widget from the viewport if currently displayed" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_UIClass;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::NewProp_UIClass = { "UIClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameHUD_eventHideUI_Parms, UIClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::NewProp_UIClass,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameHUD, nullptr, "HideUI", Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::RunGameHUD_eventHideUI_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::RunGameHUD_eventHideUI_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameHUD_HideUI()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameHUD_HideUI_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameHUD::execHideUI)
{
	P_GET_OBJECT(UClass,Z_Param_UIClass);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HideUI(Z_Param_UIClass);
	P_NATIVE_END;
}
// ********** End Class ARunGameHUD Function HideUI ************************************************

// ********** Begin Class ARunGameHUD Function OnGameStateChangedCallback **************************
struct Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics
{
	struct RunGameHUD_eventOnGameStateChangedCallback_Parms
	{
		ERunGameGameState OldState;
		ERunGameGameState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Reactively updates the displayed UI when the game state changes */" },
#endif
		{ "ModuleRelativePath", "RunGameHUD.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reactively updates the displayed UI when the game state changes" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_OldState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_OldState;
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NewState;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_OldState = { "OldState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameHUD_eventOnGameStateChangedCallback_Parms, OldState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameHUD_eventOnGameStateChangedCallback_Parms, NewState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_OldState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameHUD, nullptr, "OnGameStateChangedCallback", Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::RunGameHUD_eventOnGameStateChangedCallback_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::RunGameHUD_eventOnGameStateChangedCallback_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameHUD::execOnGameStateChangedCallback)
{
	P_GET_ENUM(ERunGameGameState,Z_Param_OldState);
	P_GET_ENUM(ERunGameGameState,Z_Param_NewState);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnGameStateChangedCallback(ERunGameGameState(Z_Param_OldState),ERunGameGameState(Z_Param_NewState));
	P_NATIVE_END;
}
// ********** End Class ARunGameHUD Function OnGameStateChangedCallback ****************************

// ********** Begin Class ARunGameHUD Function ShowUI **********************************************
struct Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics
{
	struct RunGameHUD_eventShowUI_Parms
	{
		TSubclassOf<UUserWidget> UIClass;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UI" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Creates and displays the specified UI widget on the viewport */" },
#endif
		{ "ModuleRelativePath", "RunGameHUD.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Creates and displays the specified UI widget on the viewport" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_UIClass;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::NewProp_UIClass = { "UIClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameHUD_eventShowUI_Parms, UIClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::NewProp_UIClass,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameHUD, nullptr, "ShowUI", Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::RunGameHUD_eventShowUI_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::RunGameHUD_eventShowUI_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameHUD_ShowUI()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameHUD_ShowUI_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameHUD::execShowUI)
{
	P_GET_OBJECT(UClass,Z_Param_UIClass);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ShowUI(Z_Param_UIClass);
	P_NATIVE_END;
}
// ********** End Class ARunGameHUD Function ShowUI ************************************************

// ********** Begin Class ARunGameHUD **************************************************************
void ARunGameHUD::StaticRegisterNativesARunGameHUD()
{
	UClass* Class = ARunGameHUD::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetCurrentUI", &ARunGameHUD::execGetCurrentUI },
		{ "HideUI", &ARunGameHUD::execHideUI },
		{ "OnGameStateChangedCallback", &ARunGameHUD::execOnGameStateChangedCallback },
		{ "ShowUI", &ARunGameHUD::execShowUI },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGameHUD;
UClass* ARunGameHUD::GetPrivateStaticClass()
{
	using TClass = ARunGameHUD;
	if (!Z_Registration_Info_UClass_ARunGameHUD.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameHUD"),
			Z_Registration_Info_UClass_ARunGameHUD.InnerSingleton,
			StaticRegisterNativesARunGameHUD,
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
	return Z_Registration_Info_UClass_ARunGameHUD.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGameHUD_NoRegister()
{
	return ARunGameHUD::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGameHUD_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Rendering Actor Input Replication" },
		{ "IncludePath", "RunGameHUD.h" },
		{ "ModuleRelativePath", "RunGameHUD.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentUIMap_MetaData[] = {
		{ "Category", "UI Config" },
		{ "ModuleRelativePath", "RunGameHUD.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentActiveWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "RunGameHUD.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_CurrentUIMap_ValueProp;
	static const UECodeGen_Private::FBytePropertyParams NewProp_CurrentUIMap_Key_KeyProp_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_CurrentUIMap_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_CurrentUIMap;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CurrentActiveWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ARunGameHUD_GetCurrentUI, "GetCurrentUI" }, // 4118096714
		{ &Z_Construct_UFunction_ARunGameHUD_HideUI, "HideUI" }, // 2560064030
		{ &Z_Construct_UFunction_ARunGameHUD_OnGameStateChangedCallback, "OnGameStateChangedCallback" }, // 1303117307
		{ &Z_Construct_UFunction_ARunGameHUD_ShowUI, "ShowUI" }, // 3737669855
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGameHUD>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap_ValueProp = { "CurrentUIMap", nullptr, (EPropertyFlags)0x0004000000000001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UClass, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap_Key_KeyProp_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap_Key_KeyProp = { "CurrentUIMap_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap = { "CurrentUIMap", nullptr, (EPropertyFlags)0x0024080000000015, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameHUD, CurrentUIMap), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentUIMap_MetaData), NewProp_CurrentUIMap_MetaData) }; // 1491870806
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentActiveWidget = { "CurrentActiveWidget", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameHUD, CurrentActiveWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentActiveWidget_MetaData), NewProp_CurrentActiveWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGameHUD_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap_Key_KeyProp_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentUIMap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameHUD_Statics::NewProp_CurrentActiveWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameHUD_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGameHUD_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AHUD,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameHUD_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGameHUD_Statics::ClassParams = {
	&ARunGameHUD::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ARunGameHUD_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameHUD_Statics::PropPointers),
	0,
	0x009003ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameHUD_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGameHUD_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGameHUD()
{
	if (!Z_Registration_Info_UClass_ARunGameHUD.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGameHUD.OuterSingleton, Z_Construct_UClass_ARunGameHUD_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGameHUD.OuterSingleton;
}
ARunGameHUD::ARunGameHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGameHUD);
ARunGameHUD::~ARunGameHUD() {}
// ********** End Class ARunGameHUD ****************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameHUD_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGameHUD, ARunGameHUD::StaticClass, TEXT("ARunGameHUD"), &Z_Registration_Info_UClass_ARunGameHUD, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGameHUD), 980427284U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameHUD_h__Script_RunGame_3079553867(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameHUD_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameHUD_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
