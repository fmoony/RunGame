// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Actor/Volume/RunGameInteractiveVolume.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameInteractiveVolume() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AVolume();
ENGINE_API UClass* Z_Construct_UClass_UBoxComponent_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameCharacter_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameInteractiveVolume();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameInteractiveVolume_NoRegister();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOnInteractionBegin ***************************************************
struct Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics
{
	struct _Script_RunGame_eventOnInteractionBegin_Parms
	{
		ARunGameCharacter* PlayerCharacter;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x8a\xa8\xe6\x80\x81\xe5\xa4\x9a\xe6\x92\xad\xe5\xa7\x94\xe6\x89\x98\xef\xbc\x88\xe6\x96\xb9\xe4\xbe\xbf\xe8\x93\x9d\xe5\x9b\xbe\xe7\xbb\x91\xe5\xae\x9a\xef\xbc\x89\n" },
#endif
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x8a\xa8\xe6\x80\x81\xe5\xa4\x9a\xe6\x92\xad\xe5\xa7\x94\xe6\x89\x98\xef\xbc\x88\xe6\x96\xb9\xe4\xbe\xbf\xe8\x93\x9d\xe5\x9b\xbe\xe7\xbb\x91\xe5\xae\x9a\xef\xbc\x89" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerCharacter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::NewProp_PlayerCharacter = { "PlayerCharacter", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnInteractionBegin_Parms, PlayerCharacter), Z_Construct_UClass_ARunGameCharacter_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::NewProp_PlayerCharacter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnInteractionBegin__DelegateSignature", Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::_Script_RunGame_eventOnInteractionBegin_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::_Script_RunGame_eventOnInteractionBegin_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnInteractionBegin_DelegateWrapper(const FMulticastScriptDelegate& OnInteractionBegin, ARunGameCharacter* PlayerCharacter)
{
	struct _Script_RunGame_eventOnInteractionBegin_Parms
	{
		ARunGameCharacter* PlayerCharacter;
	};
	_Script_RunGame_eventOnInteractionBegin_Parms Parms;
	Parms.PlayerCharacter=PlayerCharacter;
	OnInteractionBegin.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnInteractionBegin *****************************************************

// ********** Begin Delegate FOnInteractionEnd *****************************************************
struct Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics
{
	struct _Script_RunGame_eventOnInteractionEnd_Parms
	{
		ARunGameCharacter* PlayerCharacter;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerCharacter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::NewProp_PlayerCharacter = { "PlayerCharacter", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnInteractionEnd_Parms, PlayerCharacter), Z_Construct_UClass_ARunGameCharacter_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::NewProp_PlayerCharacter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnInteractionEnd__DelegateSignature", Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::_Script_RunGame_eventOnInteractionEnd_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::_Script_RunGame_eventOnInteractionEnd_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnInteractionEnd_DelegateWrapper(const FMulticastScriptDelegate& OnInteractionEnd, ARunGameCharacter* PlayerCharacter)
{
	struct _Script_RunGame_eventOnInteractionEnd_Parms
	{
		ARunGameCharacter* PlayerCharacter;
	};
	_Script_RunGame_eventOnInteractionEnd_Parms Parms;
	Parms.PlayerCharacter=PlayerCharacter;
	OnInteractionEnd.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnInteractionEnd *******************************************************

// ********** Begin Class ARunGameInteractiveVolume Function OnPlayerEnter *************************
struct RunGameInteractiveVolume_eventOnPlayerEnter_Parms
{
	ARunGameCharacter* PlayerCharacter;
};
static FName NAME_ARunGameInteractiveVolume_OnPlayerEnter = FName(TEXT("OnPlayerEnter"));
void ARunGameInteractiveVolume::OnPlayerEnter(ARunGameCharacter* PlayerCharacter)
{
	UFunction* Func = FindFunctionChecked(NAME_ARunGameInteractiveVolume_OnPlayerEnter);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		RunGameInteractiveVolume_eventOnPlayerEnter_Parms Parms;
		Parms.PlayerCharacter=PlayerCharacter;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		OnPlayerEnter_Implementation(PlayerCharacter);
	}
}
struct Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Interaction" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** BlueprintNativeEvent: called when a RunGame character enters this volume */" },
#endif
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "BlueprintNativeEvent: called when a RunGame character enters this volume" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerCharacter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::NewProp_PlayerCharacter = { "PlayerCharacter", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameInteractiveVolume_eventOnPlayerEnter_Parms, PlayerCharacter), Z_Construct_UClass_ARunGameCharacter_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::NewProp_PlayerCharacter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameInteractiveVolume, nullptr, "OnPlayerEnter", Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::PropPointers), sizeof(RunGameInteractiveVolume_eventOnPlayerEnter_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(RunGameInteractiveVolume_eventOnPlayerEnter_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameInteractiveVolume::execOnPlayerEnter)
{
	P_GET_OBJECT(ARunGameCharacter,Z_Param_PlayerCharacter);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnPlayerEnter_Implementation(Z_Param_PlayerCharacter);
	P_NATIVE_END;
}
// ********** End Class ARunGameInteractiveVolume Function OnPlayerEnter ***************************

// ********** Begin Class ARunGameInteractiveVolume Function OnPlayerLeave *************************
struct RunGameInteractiveVolume_eventOnPlayerLeave_Parms
{
	ARunGameCharacter* PlayerCharacter;
};
static FName NAME_ARunGameInteractiveVolume_OnPlayerLeave = FName(TEXT("OnPlayerLeave"));
void ARunGameInteractiveVolume::OnPlayerLeave(ARunGameCharacter* PlayerCharacter)
{
	UFunction* Func = FindFunctionChecked(NAME_ARunGameInteractiveVolume_OnPlayerLeave);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		RunGameInteractiveVolume_eventOnPlayerLeave_Parms Parms;
		Parms.PlayerCharacter=PlayerCharacter;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		OnPlayerLeave_Implementation(PlayerCharacter);
	}
}
struct Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Interaction" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** BlueprintNativeEvent: called when a RunGame character leaves this volume */" },
#endif
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "BlueprintNativeEvent: called when a RunGame character leaves this volume" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerCharacter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::NewProp_PlayerCharacter = { "PlayerCharacter", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameInteractiveVolume_eventOnPlayerLeave_Parms, PlayerCharacter), Z_Construct_UClass_ARunGameCharacter_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::NewProp_PlayerCharacter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameInteractiveVolume, nullptr, "OnPlayerLeave", Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::PropPointers), sizeof(RunGameInteractiveVolume_eventOnPlayerLeave_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(RunGameInteractiveVolume_eventOnPlayerLeave_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameInteractiveVolume::execOnPlayerLeave)
{
	P_GET_OBJECT(ARunGameCharacter,Z_Param_PlayerCharacter);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnPlayerLeave_Implementation(Z_Param_PlayerCharacter);
	P_NATIVE_END;
}
// ********** End Class ARunGameInteractiveVolume Function OnPlayerLeave ***************************

// ********** Begin Class ARunGameInteractiveVolume ************************************************
void ARunGameInteractiveVolume::StaticRegisterNativesARunGameInteractiveVolume()
{
	UClass* Class = ARunGameInteractiveVolume::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnPlayerEnter", &ARunGameInteractiveVolume::execOnPlayerEnter },
		{ "OnPlayerLeave", &ARunGameInteractiveVolume::execOnPlayerLeave },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGameInteractiveVolume;
UClass* ARunGameInteractiveVolume::GetPrivateStaticClass()
{
	using TClass = ARunGameInteractiveVolume;
	if (!Z_Registration_Info_UClass_ARunGameInteractiveVolume.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameInteractiveVolume"),
			Z_Registration_Info_UClass_ARunGameInteractiveVolume.InnerSingleton,
			StaticRegisterNativesARunGameInteractiveVolume,
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
	return Z_Registration_Info_UClass_ARunGameInteractiveVolume.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGameInteractiveVolume_NoRegister()
{
	return ARunGameInteractiveVolume::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGameInteractiveVolume_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe6\x9e\x81\xe7\xae\x80\xe9\x80\x9a\xe7\x94\xa8\xe4\xba\xa4\xe4\xba\x92\xe4\xbd\x93\xe7\xa7\xaf\xe5\x9f\xba\xe7\xb1\xbb - \xe4\xb8\x93\xe4\xb8\xba\xe8\xb7\x91\xe9\x85\xb7\xe6\xb8\xb8\xe6\x88\x8f\xe8\xae\xbe\xe8\xae\xa1\n */" },
#endif
		{ "HideCategories", "Brush Physics Object Blueprint Display Rendering Physics Input" },
		{ "IncludePath", "Actor/Volume/RunGameInteractiveVolume.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x9e\x81\xe7\xae\x80\xe9\x80\x9a\xe7\x94\xa8\xe4\xba\xa4\xe4\xba\x92\xe4\xbd\x93\xe7\xa7\xaf\xe5\x9f\xba\xe7\xb1\xbb - \xe4\xb8\x93\xe4\xb8\xba\xe8\xb7\x91\xe9\x85\xb7\xe6\xb8\xb8\xe6\x88\x8f\xe8\xae\xbe\xe8\xae\xa1" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CollisionComp_MetaData[] = {
		{ "Category", "Collision" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPlayerEnterDelegate_MetaData[] = {
		{ "Category", "RunGame|Interaction" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Delegate fired when a player interacts with this volume */" },
#endif
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Delegate fired when a player interacts with this volume" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPlayerLeaveDelegate_MetaData[] = {
		{ "Category", "RunGame|Interaction" },
		{ "ModuleRelativePath", "Actor/Volume/RunGameInteractiveVolume.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CollisionComp;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPlayerEnterDelegate;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPlayerLeaveDelegate;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerEnter, "OnPlayerEnter" }, // 1251551351
		{ &Z_Construct_UFunction_ARunGameInteractiveVolume_OnPlayerLeave, "OnPlayerLeave" }, // 4208200118
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGameInteractiveVolume>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameInteractiveVolume_Statics::NewProp_CollisionComp = { "CollisionComp", nullptr, (EPropertyFlags)0x00200800000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameInteractiveVolume, CollisionComp), Z_Construct_UClass_UBoxComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CollisionComp_MetaData), NewProp_CollisionComp_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ARunGameInteractiveVolume_Statics::NewProp_OnPlayerEnterDelegate = { "OnPlayerEnterDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameInteractiveVolume, OnPlayerEnterDelegate), Z_Construct_UDelegateFunction_RunGame_OnInteractionBegin__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPlayerEnterDelegate_MetaData), NewProp_OnPlayerEnterDelegate_MetaData) }; // 2504382934
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ARunGameInteractiveVolume_Statics::NewProp_OnPlayerLeaveDelegate = { "OnPlayerLeaveDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameInteractiveVolume, OnPlayerLeaveDelegate), Z_Construct_UDelegateFunction_RunGame_OnInteractionEnd__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPlayerLeaveDelegate_MetaData), NewProp_OnPlayerLeaveDelegate_MetaData) }; // 2439878579
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGameInteractiveVolume_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameInteractiveVolume_Statics::NewProp_CollisionComp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameInteractiveVolume_Statics::NewProp_OnPlayerEnterDelegate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameInteractiveVolume_Statics::NewProp_OnPlayerLeaveDelegate,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameInteractiveVolume_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGameInteractiveVolume_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AVolume,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameInteractiveVolume_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGameInteractiveVolume_Statics::ClassParams = {
	&ARunGameInteractiveVolume::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ARunGameInteractiveVolume_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameInteractiveVolume_Statics::PropPointers),
	0,
	0x009001A5u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameInteractiveVolume_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGameInteractiveVolume_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGameInteractiveVolume()
{
	if (!Z_Registration_Info_UClass_ARunGameInteractiveVolume.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGameInteractiveVolume.OuterSingleton, Z_Construct_UClass_ARunGameInteractiveVolume_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGameInteractiveVolume.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGameInteractiveVolume);
ARunGameInteractiveVolume::~ARunGameInteractiveVolume() {}
// ********** End Class ARunGameInteractiveVolume **************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGameInteractiveVolume, ARunGameInteractiveVolume::StaticClass, TEXT("ARunGameInteractiveVolume"), &Z_Registration_Info_UClass_ARunGameInteractiveVolume, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGameInteractiveVolume), 765866603U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h__Script_RunGame_1378207270(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameInteractiveVolume_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
