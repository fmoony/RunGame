// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RunGamePlayerController.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGamePlayerController() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerController();
RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerController_NoRegister();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState();
UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ARunGamePlayerController Function OnGameStateChangedCallback *************
struct Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics
{
	struct RunGamePlayerController_eventOnGameStateChangedCallback_Parms
	{
		ERunGameGameState OldState;
		ERunGameGameState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Reactively manages input mode and view target based on game state changes */" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reactively manages input mode and view target based on game state changes" },
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
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_OldState = { "OldState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGamePlayerController_eventOnGameStateChangedCallback_Parms, OldState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGamePlayerController_eventOnGameStateChangedCallback_Parms, NewState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_OldState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerController, nullptr, "OnGameStateChangedCallback", Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::RunGamePlayerController_eventOnGameStateChangedCallback_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::RunGamePlayerController_eventOnGameStateChangedCallback_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerController::execOnGameStateChangedCallback)
{
	P_GET_ENUM(ERunGameGameState,Z_Param_OldState);
	P_GET_ENUM(ERunGameGameState,Z_Param_NewState);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnGameStateChangedCallback(ERunGameGameState(Z_Param_OldState),ERunGameGameState(Z_Param_NewState));
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerController Function OnGameStateChangedCallback ***************

// ********** Begin Class ARunGamePlayerController Function SetInputModeToUIOnly *******************
struct Z_Construct_UFunction_ARunGamePlayerController_SetInputModeToUIOnly_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Switch to UI input mode: show mouse cursor, disable character control */" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Switch to UI input mode: show mouse cursor, disable character control" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerController_SetInputModeToUIOnly_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerController, nullptr, "SetInputModeToUIOnly", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerController_SetInputModeToUIOnly_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerController_SetInputModeToUIOnly_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGamePlayerController_SetInputModeToUIOnly()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerController_SetInputModeToUIOnly_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerController::execSetInputModeToUIOnly)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetInputModeToUIOnly();
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerController Function SetInputModeToUIOnly *********************

// ********** Begin Class ARunGamePlayerController *************************************************
void ARunGamePlayerController::StaticRegisterNativesARunGamePlayerController()
{
	UClass* Class = ARunGamePlayerController::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnGameStateChangedCallback", &ARunGamePlayerController::execOnGameStateChangedCallback },
		{ "SetInputModeToUIOnly", &ARunGamePlayerController::execSetInputModeToUIOnly },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGamePlayerController;
UClass* ARunGamePlayerController::GetPrivateStaticClass()
{
	using TClass = ARunGamePlayerController;
	if (!Z_Registration_Info_UClass_ARunGamePlayerController.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGamePlayerController"),
			Z_Registration_Info_UClass_ARunGamePlayerController.InnerSingleton,
			StaticRegisterNativesARunGamePlayerController,
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
	return Z_Registration_Info_UClass_ARunGamePlayerController.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGamePlayerController_NoRegister()
{
	return ARunGamePlayerController::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGamePlayerController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  Basic PlayerController class for a third person game\n *  Manages input mappings\n */" },
#endif
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "RunGamePlayerController.h" },
		{ "ModuleRelativePath", "RunGamePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Basic PlayerController class for a third person game\nManages input mappings" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultMappingContexts_MetaData[] = {
		{ "Category", "Input|Input Mappings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Input Mapping Contexts */" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Input Mapping Contexts" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MobileExcludedMappingContexts_MetaData[] = {
		{ "Category", "Input|Input Mappings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Input Mapping Contexts */" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Input Mapping Contexts" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MobileControlsWidgetClass_MetaData[] = {
		{ "Category", "Input|Touch Controls" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Mobile controls widget to spawn */" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Mobile controls widget to spawn" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MainMenuCameraTag_MetaData[] = {
		{ "Category", "RunGame|Camera" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Tag used to find the MainMenuCamera placed in the level */" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Tag used to find the MainMenuCamera placed in the level" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DefaultMappingContexts_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_DefaultMappingContexts;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MobileExcludedMappingContexts_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_MobileExcludedMappingContexts;
	static const UECodeGen_Private::FClassPropertyParams NewProp_MobileControlsWidgetClass;
	static const UECodeGen_Private::FNamePropertyParams NewProp_MainMenuCameraTag;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ARunGamePlayerController_OnGameStateChangedCallback, "OnGameStateChangedCallback" }, // 296998745
		{ &Z_Construct_UFunction_ARunGamePlayerController_SetInputModeToUIOnly, "SetInputModeToUIOnly" }, // 2250968191
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGamePlayerController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_DefaultMappingContexts_Inner = { "DefaultMappingContexts", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_DefaultMappingContexts = { "DefaultMappingContexts", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGamePlayerController, DefaultMappingContexts), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultMappingContexts_MetaData), NewProp_DefaultMappingContexts_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MobileExcludedMappingContexts_Inner = { "MobileExcludedMappingContexts", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MobileExcludedMappingContexts = { "MobileExcludedMappingContexts", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGamePlayerController, MobileExcludedMappingContexts), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MobileExcludedMappingContexts_MetaData), NewProp_MobileExcludedMappingContexts_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MobileControlsWidgetClass = { "MobileControlsWidgetClass", nullptr, (EPropertyFlags)0x0024080000000001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGamePlayerController, MobileControlsWidgetClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MobileControlsWidgetClass_MetaData), NewProp_MobileControlsWidgetClass_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MainMenuCameraTag = { "MainMenuCameraTag", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGamePlayerController, MainMenuCameraTag), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MainMenuCameraTag_MetaData), NewProp_MainMenuCameraTag_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGamePlayerController_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_DefaultMappingContexts_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_DefaultMappingContexts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MobileExcludedMappingContexts_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MobileExcludedMappingContexts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MobileControlsWidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerController_Statics::NewProp_MainMenuCameraTag,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerController_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGamePlayerController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGamePlayerController_Statics::ClassParams = {
	&ARunGamePlayerController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ARunGamePlayerController_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerController_Statics::PropPointers),
	0,
	0x008003A5u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerController_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGamePlayerController_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGamePlayerController()
{
	if (!Z_Registration_Info_UClass_ARunGamePlayerController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGamePlayerController.OuterSingleton, Z_Construct_UClass_ARunGamePlayerController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGamePlayerController.OuterSingleton;
}
ARunGamePlayerController::ARunGamePlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGamePlayerController);
ARunGamePlayerController::~ARunGamePlayerController() {}
// ********** End Class ARunGamePlayerController ***************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerController_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGamePlayerController, ARunGamePlayerController::StaticClass, TEXT("ARunGamePlayerController"), &Z_Registration_Info_UClass_ARunGamePlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGamePlayerController), 538278119U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerController_h__Script_RunGame_4179724189(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerController_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerController_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
