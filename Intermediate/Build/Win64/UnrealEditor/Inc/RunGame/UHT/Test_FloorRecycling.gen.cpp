// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Test_FloorRecycling.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeTest_FloorRecycling() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
FUNCTIONALTESTING_API UClass* Z_Construct_UClass_AFunctionalTest();
RUNGAME_API UClass* Z_Construct_UClass_ATest_FloorRecycling();
RUNGAME_API UClass* Z_Construct_UClass_ATest_FloorRecycling_NoRegister();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ATest_FloorRecycling *****************************************************
void ATest_FloorRecycling::StaticRegisterNativesATest_FloorRecycling()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_ATest_FloorRecycling;
UClass* ATest_FloorRecycling::GetPrivateStaticClass()
{
	using TClass = ATest_FloorRecycling;
	if (!Z_Registration_Info_UClass_ATest_FloorRecycling.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("Test_FloorRecycling"),
			Z_Registration_Info_UClass_ATest_FloorRecycling.InnerSingleton,
			StaticRegisterNativesATest_FloorRecycling,
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
	return Z_Registration_Info_UClass_ATest_FloorRecycling.InnerSingleton;
}
UClass* Z_Construct_UClass_ATest_FloorRecycling_NoRegister()
{
	return ATest_FloorRecycling::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ATest_FloorRecycling_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Actor Input Rendering HLOD" },
		{ "IncludePath", "AFunctionalTest/Test_FloorRecycling.h" },
		{ "ModuleRelativePath", "AFunctionalTest/Test_FloorRecycling.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StraightFloorClasses_MetaData[] = {
		{ "Category", "Test|FloorConfig" },
		{ "ModuleRelativePath", "AFunctionalTest/Test_FloorRecycling.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TurnFloorClasses_MetaData[] = {
		{ "Category", "Test|FloorConfig" },
		{ "ModuleRelativePath", "AFunctionalTest/Test_FloorRecycling.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PreAllocateCount_MetaData[] = {
		{ "Category", "Test|FloorConfig" },
		{ "ModuleRelativePath", "AFunctionalTest/Test_FloorRecycling.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InitialStraightCount_MetaData[] = {
		{ "Category", "Test|SpawnConfig" },
		{ "ModuleRelativePath", "AFunctionalTest/Test_FloorRecycling.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InitialRandomCount_MetaData[] = {
		{ "Category", "Test|SpawnConfig" },
		{ "ModuleRelativePath", "AFunctionalTest/Test_FloorRecycling.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RecycleDistance_MetaData[] = {
		{ "Category", "Test|RecycleConfig" },
		{ "ModuleRelativePath", "AFunctionalTest/Test_FloorRecycling.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_StraightFloorClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_StraightFloorClasses;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_TurnFloorClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TurnFloorClasses;
	static const UECodeGen_Private::FIntPropertyParams NewProp_PreAllocateCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InitialStraightCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InitialRandomCount;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RecycleDistance;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATest_FloorRecycling>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_StraightFloorClasses_Inner = { "StraightFloorClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_StraightFloorClasses = { "StraightFloorClasses", nullptr, (EPropertyFlags)0x0014000000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATest_FloorRecycling, StraightFloorClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StraightFloorClasses_MetaData), NewProp_StraightFloorClasses_MetaData) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_TurnFloorClasses_Inner = { "TurnFloorClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_TurnFloorClasses = { "TurnFloorClasses", nullptr, (EPropertyFlags)0x0014000000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATest_FloorRecycling, TurnFloorClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TurnFloorClasses_MetaData), NewProp_TurnFloorClasses_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_PreAllocateCount = { "PreAllocateCount", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATest_FloorRecycling, PreAllocateCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PreAllocateCount_MetaData), NewProp_PreAllocateCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_InitialStraightCount = { "InitialStraightCount", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATest_FloorRecycling, InitialStraightCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InitialStraightCount_MetaData), NewProp_InitialStraightCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_InitialRandomCount = { "InitialRandomCount", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATest_FloorRecycling, InitialRandomCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InitialRandomCount_MetaData), NewProp_InitialRandomCount_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_RecycleDistance = { "RecycleDistance", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATest_FloorRecycling, RecycleDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RecycleDistance_MetaData), NewProp_RecycleDistance_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ATest_FloorRecycling_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_StraightFloorClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_StraightFloorClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_TurnFloorClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_TurnFloorClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_PreAllocateCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_InitialStraightCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_InitialRandomCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATest_FloorRecycling_Statics::NewProp_RecycleDistance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATest_FloorRecycling_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ATest_FloorRecycling_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AFunctionalTest,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATest_FloorRecycling_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ATest_FloorRecycling_Statics::ClassParams = {
	&ATest_FloorRecycling::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ATest_FloorRecycling_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ATest_FloorRecycling_Statics::PropPointers),
	0,
	0x009001A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATest_FloorRecycling_Statics::Class_MetaDataParams), Z_Construct_UClass_ATest_FloorRecycling_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ATest_FloorRecycling()
{
	if (!Z_Registration_Info_UClass_ATest_FloorRecycling.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATest_FloorRecycling.OuterSingleton, Z_Construct_UClass_ATest_FloorRecycling_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ATest_FloorRecycling.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ATest_FloorRecycling);
ATest_FloorRecycling::~ATest_FloorRecycling() {}
// ********** End Class ATest_FloorRecycling *******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_AFunctionalTest_Test_FloorRecycling_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ATest_FloorRecycling, ATest_FloorRecycling::StaticClass, TEXT("ATest_FloorRecycling"), &Z_Registration_Info_UClass_ATest_FloorRecycling, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATest_FloorRecycling), 4062430509U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_AFunctionalTest_Test_FloorRecycling_h__Script_RunGame_2403974653(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_AFunctionalTest_Test_FloorRecycling_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_AFunctionalTest_Test_FloorRecycling_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
