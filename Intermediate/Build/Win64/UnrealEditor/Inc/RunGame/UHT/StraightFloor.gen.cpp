// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Actor/Floor/StraightFloor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeStraightFloor() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_AFloorBase();
RUNGAME_API UClass* Z_Construct_UClass_AStraightFloor();
RUNGAME_API UClass* Z_Construct_UClass_AStraightFloor_NoRegister();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AStraightFloor ***********************************************************
void AStraightFloor::StaticRegisterNativesAStraightFloor()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_AStraightFloor;
UClass* AStraightFloor::GetPrivateStaticClass()
{
	using TClass = AStraightFloor;
	if (!Z_Registration_Info_UClass_AStraightFloor.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("StraightFloor"),
			Z_Registration_Info_UClass_AStraightFloor.InnerSingleton,
			StaticRegisterNativesAStraightFloor,
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
	return Z_Registration_Info_UClass_AStraightFloor.InnerSingleton;
}
UClass* Z_Construct_UClass_AStraightFloor_NoRegister()
{
	return AStraightFloor::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AStraightFloor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Actor/Floor/StraightFloor.h" },
		{ "ModuleRelativePath", "Actor/Floor/StraightFloor.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AStraightFloor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AStraightFloor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AFloorBase,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AStraightFloor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AStraightFloor_Statics::ClassParams = {
	&AStraightFloor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009001A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AStraightFloor_Statics::Class_MetaDataParams), Z_Construct_UClass_AStraightFloor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AStraightFloor()
{
	if (!Z_Registration_Info_UClass_AStraightFloor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AStraightFloor.OuterSingleton, Z_Construct_UClass_AStraightFloor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AStraightFloor.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AStraightFloor);
AStraightFloor::~AStraightFloor() {}
// ********** End Class AStraightFloor *************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AStraightFloor, AStraightFloor::StaticClass, TEXT("AStraightFloor"), &Z_Registration_Info_UClass_AStraightFloor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AStraightFloor), 1055316607U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h__Script_RunGame_4197835583(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_StraightFloor_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
