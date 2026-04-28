// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "HUD/RunGameMainMenu.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameMainMenu() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_URunGameMainMenu();
RUNGAME_API UClass* Z_Construct_UClass_URunGameMainMenu_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class URunGameMainMenu Function OnQuitClicked **********************************
struct Z_Construct_UFunction_URunGameMainMenu_OnQuitClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "HUD/RunGameMainMenu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameMainMenu_OnQuitClicked_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameMainMenu, nullptr, "OnQuitClicked", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameMainMenu_OnQuitClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameMainMenu_OnQuitClicked_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameMainMenu_OnQuitClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameMainMenu_OnQuitClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameMainMenu::execOnQuitClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnQuitClicked();
	P_NATIVE_END;
}
// ********** End Class URunGameMainMenu Function OnQuitClicked ************************************

// ********** Begin Class URunGameMainMenu Function OnStartClicked *********************************
struct Z_Construct_UFunction_URunGameMainMenu_OnStartClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "HUD/RunGameMainMenu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameMainMenu_OnStartClicked_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameMainMenu, nullptr, "OnStartClicked", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameMainMenu_OnStartClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameMainMenu_OnStartClicked_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameMainMenu_OnStartClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameMainMenu_OnStartClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameMainMenu::execOnStartClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnStartClicked();
	P_NATIVE_END;
}
// ********** End Class URunGameMainMenu Function OnStartClicked ***********************************

// ********** Begin Class URunGameMainMenu Function QuitGame ***************************************
struct Z_Construct_UFunction_URunGameMainMenu_QuitGame_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Main Menu" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Quit the game, called by the quit button in the main menu */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameMainMenu.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Quit the game, called by the quit button in the main menu" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameMainMenu_QuitGame_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameMainMenu, nullptr, "QuitGame", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameMainMenu_QuitGame_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameMainMenu_QuitGame_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameMainMenu_QuitGame()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameMainMenu_QuitGame_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameMainMenu::execQuitGame)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->QuitGame();
	P_NATIVE_END;
}
// ********** End Class URunGameMainMenu Function QuitGame *****************************************

// ********** Begin Class URunGameMainMenu Function StartGame **************************************
struct Z_Construct_UFunction_URunGameMainMenu_StartGame_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Main Menu" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Start the game, called by the start button in the main menu */" },
#endif
		{ "ModuleRelativePath", "HUD/RunGameMainMenu.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Start the game, called by the start button in the main menu" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameMainMenu_StartGame_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameMainMenu, nullptr, "StartGame", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameMainMenu_StartGame_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameMainMenu_StartGame_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameMainMenu_StartGame()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameMainMenu_StartGame_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameMainMenu::execStartGame)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StartGame();
	P_NATIVE_END;
}
// ********** End Class URunGameMainMenu Function StartGame ****************************************

// ********** Begin Class URunGameMainMenu *********************************************************
void URunGameMainMenu::StaticRegisterNativesURunGameMainMenu()
{
	UClass* Class = URunGameMainMenu::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnQuitClicked", &URunGameMainMenu::execOnQuitClicked },
		{ "OnStartClicked", &URunGameMainMenu::execOnStartClicked },
		{ "QuitGame", &URunGameMainMenu::execQuitGame },
		{ "StartGame", &URunGameMainMenu::execStartGame },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_URunGameMainMenu;
UClass* URunGameMainMenu::GetPrivateStaticClass()
{
	using TClass = URunGameMainMenu;
	if (!Z_Registration_Info_UClass_URunGameMainMenu.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameMainMenu"),
			Z_Registration_Info_UClass_URunGameMainMenu.InnerSingleton,
			StaticRegisterNativesURunGameMainMenu,
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
	return Z_Registration_Info_UClass_URunGameMainMenu.InnerSingleton;
}
UClass* Z_Construct_UClass_URunGameMainMenu_NoRegister()
{
	return URunGameMainMenu::GetPrivateStaticClass();
}
struct Z_Construct_UClass_URunGameMainMenu_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "HUD/RunGameMainMenu.h" },
		{ "ModuleRelativePath", "HUD/RunGameMainMenu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameMainMenu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_QuitButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "HUD/RunGameMainMenu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_StartButton;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_QuitButton;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_URunGameMainMenu_OnQuitClicked, "OnQuitClicked" }, // 742499464
		{ &Z_Construct_UFunction_URunGameMainMenu_OnStartClicked, "OnStartClicked" }, // 2894922430
		{ &Z_Construct_UFunction_URunGameMainMenu_QuitGame, "QuitGame" }, // 808682387
		{ &Z_Construct_UFunction_URunGameMainMenu_StartGame, "StartGame" }, // 1593413597
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URunGameMainMenu>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameMainMenu_Statics::NewProp_StartButton = { "StartButton", nullptr, (EPropertyFlags)0x0114000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameMainMenu, StartButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartButton_MetaData), NewProp_StartButton_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameMainMenu_Statics::NewProp_QuitButton = { "QuitButton", nullptr, (EPropertyFlags)0x0114000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameMainMenu, QuitButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_QuitButton_MetaData), NewProp_QuitButton_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URunGameMainMenu_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameMainMenu_Statics::NewProp_StartButton,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameMainMenu_Statics::NewProp_QuitButton,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameMainMenu_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_URunGameMainMenu_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameMainMenu_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_URunGameMainMenu_Statics::ClassParams = {
	&URunGameMainMenu::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_URunGameMainMenu_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_URunGameMainMenu_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameMainMenu_Statics::Class_MetaDataParams), Z_Construct_UClass_URunGameMainMenu_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_URunGameMainMenu()
{
	if (!Z_Registration_Info_UClass_URunGameMainMenu.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URunGameMainMenu.OuterSingleton, Z_Construct_UClass_URunGameMainMenu_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_URunGameMainMenu.OuterSingleton;
}
URunGameMainMenu::URunGameMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(URunGameMainMenu);
URunGameMainMenu::~URunGameMainMenu() {}
// ********** End Class URunGameMainMenu ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_URunGameMainMenu, URunGameMainMenu::StaticClass, TEXT("URunGameMainMenu"), &Z_Registration_Info_UClass_URunGameMainMenu, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URunGameMainMenu), 1109111912U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h__Script_RunGame_1475282506(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGameMainMenu_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
