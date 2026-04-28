// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Actor/Volume/RunGameDeathVolume.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameDeathVolume() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_ARunGameDeathVolume();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameDeathVolume_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameInteractiveVolume();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ARunGameDeathVolume ******************************************************
void ARunGameDeathVolume::StaticRegisterNativesARunGameDeathVolume()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGameDeathVolume;
UClass* ARunGameDeathVolume::GetPrivateStaticClass()
{
	using TClass = ARunGameDeathVolume;
	if (!Z_Registration_Info_UClass_ARunGameDeathVolume.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameDeathVolume"),
			Z_Registration_Info_UClass_ARunGameDeathVolume.InnerSingleton,
			StaticRegisterNativesARunGameDeathVolume,
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
	return Z_Registration_Info_UClass_ARunGameDeathVolume.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGameDeathVolume_NoRegister()
{
	return ARunGameDeathVolume::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGameDeathVolume_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe6\xad\xbb\xe4\xba\xa1\xe4\xbd\x93\xe7\xa7\xaf - \xe5\xbd\x93\xe7\x8e\xa9\xe5\xae\xb6\xe8\xbf\x9b\xe5\x85\xa5\xe6\x97\xb6\xe8\xa7\xa6\xe5\x8f\x91\xe6\xb8\xb8\xe6\x88\x8f\xe7\xbb\x93\xe6\x9d\x9f\n * \xe7\x94\xa8\xe4\xba\x8e\xe6\xa3\x80\xe6\xb5\x8b\xe7\x8e\xa9\xe5\xae\xb6\xe6\x8e\x89\xe8\x90\xbd\xe5\x88\xb0\xe5\x9c\xb0\xe5\x9b\xbe\xe4\xb8\x8b\xe6\x96\xb9\n */" },
#endif
		{ "HideCategories", "Brush Physics Object Blueprint Display Rendering Physics Input" },
		{ "IncludePath", "Actor/Volume/RunGameDeathVolume.h" },
		{ "ModuleRelativePath", "Actor/Volume/RunGameDeathVolume.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xad\xbb\xe4\xba\xa1\xe4\xbd\x93\xe7\xa7\xaf - \xe5\xbd\x93\xe7\x8e\xa9\xe5\xae\xb6\xe8\xbf\x9b\xe5\x85\xa5\xe6\x97\xb6\xe8\xa7\xa6\xe5\x8f\x91\xe6\xb8\xb8\xe6\x88\x8f\xe7\xbb\x93\xe6\x9d\x9f\n\xe7\x94\xa8\xe4\xba\x8e\xe6\xa3\x80\xe6\xb5\x8b\xe7\x8e\xa9\xe5\xae\xb6\xe6\x8e\x89\xe8\x90\xbd\xe5\x88\xb0\xe5\x9c\xb0\xe5\x9b\xbe\xe4\xb8\x8b\xe6\x96\xb9" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bImmediateDeath_MetaData[] = {
		{ "Category", "RunGame|Death" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe8\x93\x9d\xe5\x9b\xbe\xe5\x8f\xaf\xe9\x85\x8d\xe7\xbd\xae\xe5\xb1\x9e\xe6\x80\xa7\n" },
#endif
		{ "ModuleRelativePath", "Actor/Volume/RunGameDeathVolume.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\x93\x9d\xe5\x9b\xbe\xe5\x8f\xaf\xe9\x85\x8d\xe7\xbd\xae\xe5\xb1\x9e\xe6\x80\xa7" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DeathDelay_MetaData[] = {
		{ "Category", "RunGame|Death" },
		{ "ModuleRelativePath", "Actor/Volume/RunGameDeathVolume.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bImmediateDeath_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediateDeath;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DeathDelay;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGameDeathVolume>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_ARunGameDeathVolume_Statics::NewProp_bImmediateDeath_SetBit(void* Obj)
{
	((ARunGameDeathVolume*)Obj)->bImmediateDeath = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ARunGameDeathVolume_Statics::NewProp_bImmediateDeath = { "bImmediateDeath", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ARunGameDeathVolume), &Z_Construct_UClass_ARunGameDeathVolume_Statics::NewProp_bImmediateDeath_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bImmediateDeath_MetaData), NewProp_bImmediateDeath_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARunGameDeathVolume_Statics::NewProp_DeathDelay = { "DeathDelay", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameDeathVolume, DeathDelay), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DeathDelay_MetaData), NewProp_DeathDelay_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGameDeathVolume_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameDeathVolume_Statics::NewProp_bImmediateDeath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameDeathVolume_Statics::NewProp_DeathDelay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameDeathVolume_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGameDeathVolume_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ARunGameInteractiveVolume,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameDeathVolume_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGameDeathVolume_Statics::ClassParams = {
	&ARunGameDeathVolume::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ARunGameDeathVolume_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameDeathVolume_Statics::PropPointers),
	0,
	0x009001A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameDeathVolume_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGameDeathVolume_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGameDeathVolume()
{
	if (!Z_Registration_Info_UClass_ARunGameDeathVolume.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGameDeathVolume.OuterSingleton, Z_Construct_UClass_ARunGameDeathVolume_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGameDeathVolume.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGameDeathVolume);
ARunGameDeathVolume::~ARunGameDeathVolume() {}
// ********** End Class ARunGameDeathVolume ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGameDeathVolume, ARunGameDeathVolume::StaticClass, TEXT("ARunGameDeathVolume"), &Z_Registration_Info_UClass_ARunGameDeathVolume, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGameDeathVolume), 4248315160U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h__Script_RunGame_1986983208(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Volume_RunGameDeathVolume_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
