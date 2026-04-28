// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "HUD/RunGamePause.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGamePause() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_URunGamePause();
RUNGAME_API UClass* Z_Construct_UClass_URunGamePause_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class URunGamePause ************************************************************
void URunGamePause::StaticRegisterNativesURunGamePause()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_URunGamePause;
UClass* URunGamePause::GetPrivateStaticClass()
{
	using TClass = URunGamePause;
	if (!Z_Registration_Info_UClass_URunGamePause.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGamePause"),
			Z_Registration_Info_UClass_URunGamePause.InnerSingleton,
			StaticRegisterNativesURunGamePause,
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
	return Z_Registration_Info_UClass_URunGamePause.InnerSingleton;
}
UClass* Z_Construct_UClass_URunGamePause_NoRegister()
{
	return URunGamePause::GetPrivateStaticClass();
}
struct Z_Construct_UClass_URunGamePause_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "HUD/RunGamePause.h" },
		{ "ModuleRelativePath", "HUD/RunGamePause.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URunGamePause>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_URunGamePause_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGamePause_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_URunGamePause_Statics::ClassParams = {
	&URunGamePause::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_URunGamePause_Statics::Class_MetaDataParams), Z_Construct_UClass_URunGamePause_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_URunGamePause()
{
	if (!Z_Registration_Info_UClass_URunGamePause.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URunGamePause.OuterSingleton, Z_Construct_UClass_URunGamePause_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_URunGamePause.OuterSingleton;
}
URunGamePause::URunGamePause(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(URunGamePause);
URunGamePause::~URunGamePause() {}
// ********** End Class URunGamePause **************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGamePause_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_URunGamePause, URunGamePause::StaticClass, TEXT("URunGamePause"), &Z_Registration_Info_UClass_URunGamePause, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URunGamePause), 1098773396U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGamePause_h__Script_RunGame_2536179775(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGamePause_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_HUD_RunGamePause_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
