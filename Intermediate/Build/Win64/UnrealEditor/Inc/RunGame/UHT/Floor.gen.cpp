// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Actor/Floor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeFloor() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UClass* Z_Construct_UClass_AFloor();
RUNGAME_API UClass* Z_Construct_UClass_AFloor_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_AFloorBase();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AFloor *******************************************************************
void AFloor::StaticRegisterNativesAFloor()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_AFloor;
UClass* AFloor::GetPrivateStaticClass()
{
	using TClass = AFloor;
	if (!Z_Registration_Info_UClass_AFloor.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("Floor"),
			Z_Registration_Info_UClass_AFloor.InnerSingleton,
			StaticRegisterNativesAFloor,
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
	return Z_Registration_Info_UClass_AFloor.InnerSingleton;
}
UClass* Z_Construct_UClass_AFloor_NoRegister()
{
	return AFloor::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AFloor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe4\xbf\x9d\xe7\x95\x99 AFloor \xe4\xbd\x9c\xe4\xb8\xba\xe5\x85\xbc\xe5\xae\xb9\xe5\xa3\xb3\xef\xbc\x9a\xe7\xbb\xa7\xe6\x89\xbf AFloorBase\xef\xbc\x8c\xe6\x97\xa0\xe9\xa2\x9d\xe5\xa4\x96\xe9\x80\xbb\xe8\xbe\x91\xe3\x80\x82\n * \xe6\x96\xb0\xe5\x9c\xb0\xe6\x9d\xbf\xe7\xb1\xbb\xe5\x9e\x8b\xe8\xaf\xb7\xe4\xbd\xbf\xe7\x94\xa8 AStraightFloor / ATurnFloor\xe3\x80\x82\n */" },
#endif
		{ "IncludePath", "Actor/Floor.h" },
		{ "ModuleRelativePath", "Actor/Floor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe4\xbf\x9d\xe7\x95\x99 AFloor \xe4\xbd\x9c\xe4\xb8\xba\xe5\x85\xbc\xe5\xae\xb9\xe5\xa3\xb3\xef\xbc\x9a\xe7\xbb\xa7\xe6\x89\xbf AFloorBase\xef\xbc\x8c\xe6\x97\xa0\xe9\xa2\x9d\xe5\xa4\x96\xe9\x80\xbb\xe8\xbe\x91\xe3\x80\x82\n\xe6\x96\xb0\xe5\x9c\xb0\xe6\x9d\xbf\xe7\xb1\xbb\xe5\x9e\x8b\xe8\xaf\xb7\xe4\xbd\xbf\xe7\x94\xa8 AStraightFloor / ATurnFloor\xe3\x80\x82" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AFloor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AFloor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AFloorBase,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AFloor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AFloor_Statics::ClassParams = {
	&AFloor::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AFloor_Statics::Class_MetaDataParams), Z_Construct_UClass_AFloor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AFloor()
{
	if (!Z_Registration_Info_UClass_AFloor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AFloor.OuterSingleton, Z_Construct_UClass_AFloor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AFloor.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AFloor);
AFloor::~AFloor() {}
// ********** End Class AFloor *********************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AFloor, AFloor::StaticClass, TEXT("AFloor"), &Z_Registration_Info_UClass_AFloor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AFloor), 1629047482U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_h__Script_RunGame_727469575(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
