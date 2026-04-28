// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Actor/Floor/FloorBase.h"
#include "Engine/HitResult.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeFloorBase() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UArrowComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UBoxComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UPrimitiveComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FHitResult();
RUNGAME_API UClass* Z_Construct_UClass_AFloorBase();
RUNGAME_API UClass* Z_Construct_UClass_AFloorBase_NoRegister();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_FFloorType();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AFloorBase Function BoxOverlap *******************************************
struct Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics
{
	struct FloorBase_eventBoxOverlap_Parms
	{
		UPrimitiveComponent* OverlappedComponent;
		AActor* OtherActor;
		UPrimitiveComponent* OtherComp;
		int32 OtherBodyIndex;
		bool bFromSweep;
		FHitResult SweepResult;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Handles box overlap events, triggering next floor spawn and delayed recycling */" },
#endif
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Handles box overlap events, triggering next floor spawn and delayed recycling" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OverlappedComponent_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OtherComp_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SweepResult_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OverlappedComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OtherActor;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OtherComp;
	static const UECodeGen_Private::FIntPropertyParams NewProp_OtherBodyIndex;
	static void NewProp_bFromSweep_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFromSweep;
	static const UECodeGen_Private::FStructPropertyParams NewProp_SweepResult;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OverlappedComponent = { "OverlappedComponent", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FloorBase_eventBoxOverlap_Parms, OverlappedComponent), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OverlappedComponent_MetaData), NewProp_OverlappedComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OtherActor = { "OtherActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FloorBase_eventBoxOverlap_Parms, OtherActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OtherComp = { "OtherComp", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FloorBase_eventBoxOverlap_Parms, OtherComp), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OtherComp_MetaData), NewProp_OtherComp_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OtherBodyIndex = { "OtherBodyIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FloorBase_eventBoxOverlap_Parms, OtherBodyIndex), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_bFromSweep_SetBit(void* Obj)
{
	((FloorBase_eventBoxOverlap_Parms*)Obj)->bFromSweep = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_bFromSweep = { "bFromSweep", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FloorBase_eventBoxOverlap_Parms), &Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_bFromSweep_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_SweepResult = { "SweepResult", nullptr, (EPropertyFlags)0x0010008008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FloorBase_eventBoxOverlap_Parms, SweepResult), Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SweepResult_MetaData), NewProp_SweepResult_MetaData) }; // 267591329
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OverlappedComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OtherActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OtherComp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_OtherBodyIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_bFromSweep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::NewProp_SweepResult,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AFloorBase, nullptr, "BoxOverlap", Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::PropPointers), sizeof(Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::FloorBase_eventBoxOverlap_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00420400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::Function_MetaDataParams), Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::FloorBase_eventBoxOverlap_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AFloorBase_BoxOverlap()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AFloorBase_BoxOverlap_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AFloorBase::execBoxOverlap)
{
	P_GET_OBJECT(UPrimitiveComponent,Z_Param_OverlappedComponent);
	P_GET_OBJECT(AActor,Z_Param_OtherActor);
	P_GET_OBJECT(UPrimitiveComponent,Z_Param_OtherComp);
	P_GET_PROPERTY(FIntProperty,Z_Param_OtherBodyIndex);
	P_GET_UBOOL(Z_Param_bFromSweep);
	P_GET_STRUCT_REF(FHitResult,Z_Param_Out_SweepResult);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->BoxOverlap(Z_Param_OverlappedComponent,Z_Param_OtherActor,Z_Param_OtherComp,Z_Param_OtherBodyIndex,Z_Param_bFromSweep,Z_Param_Out_SweepResult);
	P_NATIVE_END;
}
// ********** End Class AFloorBase Function BoxOverlap *********************************************

// ********** Begin Class AFloorBase ***************************************************************
void AFloorBase::StaticRegisterNativesAFloorBase()
{
	UClass* Class = AFloorBase::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "BoxOverlap", &AFloorBase::execBoxOverlap },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_AFloorBase;
UClass* AFloorBase::GetPrivateStaticClass()
{
	using TClass = AFloorBase;
	if (!Z_Registration_Info_UClass_AFloorBase.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("FloorBase"),
			Z_Registration_Info_UClass_AFloorBase.InnerSingleton,
			StaticRegisterNativesAFloorBase,
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
	return Z_Registration_Info_UClass_AFloorBase.InnerSingleton;
}
UClass* Z_Construct_UClass_AFloorBase_NoRegister()
{
	return AFloorBase::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AFloorBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Actor/Floor/FloorBase.h" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScenceComponent_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "C_J" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FloorRootComponent_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "C_J" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoxComponent_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "C_J" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BoxMesh_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "C_J" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpawnPointMiddle_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "C_J" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpawnPointRight_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "C_J" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpawnPointLeft_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "C_J" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FloorType_MetaData[] = {
		{ "Category", "FloorType" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RecycleDelayTime_MetaData[] = {
		{ "Category", "FloorRecycleTime" },
		{ "ModuleRelativePath", "Actor/Floor/FloorBase.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ScenceComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FloorRootComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_BoxComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_BoxMesh;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SpawnPointMiddle;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SpawnPointRight;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SpawnPointLeft;
	static const UECodeGen_Private::FBytePropertyParams NewProp_FloorType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_FloorType;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RecycleDelayTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AFloorBase_BoxOverlap, "BoxOverlap" }, // 3133962003
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AFloorBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_ScenceComponent = { "ScenceComponent", nullptr, (EPropertyFlags)0x01240800000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, ScenceComponent), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScenceComponent_MetaData), NewProp_ScenceComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_FloorRootComponent = { "FloorRootComponent", nullptr, (EPropertyFlags)0x01240800000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, FloorRootComponent), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FloorRootComponent_MetaData), NewProp_FloorRootComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_BoxComponent = { "BoxComponent", nullptr, (EPropertyFlags)0x01240800000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, BoxComponent), Z_Construct_UClass_UBoxComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoxComponent_MetaData), NewProp_BoxComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_BoxMesh = { "BoxMesh", nullptr, (EPropertyFlags)0x01240800000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, BoxMesh), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BoxMesh_MetaData), NewProp_BoxMesh_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_SpawnPointMiddle = { "SpawnPointMiddle", nullptr, (EPropertyFlags)0x01240800000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, SpawnPointMiddle), Z_Construct_UClass_UArrowComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpawnPointMiddle_MetaData), NewProp_SpawnPointMiddle_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_SpawnPointRight = { "SpawnPointRight", nullptr, (EPropertyFlags)0x01240800000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, SpawnPointRight), Z_Construct_UClass_UArrowComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpawnPointRight_MetaData), NewProp_SpawnPointRight_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_SpawnPointLeft = { "SpawnPointLeft", nullptr, (EPropertyFlags)0x01240800000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, SpawnPointLeft), Z_Construct_UClass_UArrowComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpawnPointLeft_MetaData), NewProp_SpawnPointLeft_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_FloorType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_FloorType = { "FloorType", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, FloorType), Z_Construct_UEnum_RunGame_FFloorType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FloorType_MetaData), NewProp_FloorType_MetaData) }; // 471990832
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AFloorBase_Statics::NewProp_RecycleDelayTime = { "RecycleDelayTime", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AFloorBase, RecycleDelayTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RecycleDelayTime_MetaData), NewProp_RecycleDelayTime_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AFloorBase_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_ScenceComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_FloorRootComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_BoxComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_BoxMesh,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_SpawnPointMiddle,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_SpawnPointRight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_SpawnPointLeft,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_FloorType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_FloorType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AFloorBase_Statics::NewProp_RecycleDelayTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AFloorBase_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AFloorBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AFloorBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AFloorBase_Statics::ClassParams = {
	&AFloorBase::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AFloorBase_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AFloorBase_Statics::PropPointers),
	0,
	0x009001A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AFloorBase_Statics::Class_MetaDataParams), Z_Construct_UClass_AFloorBase_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AFloorBase()
{
	if (!Z_Registration_Info_UClass_AFloorBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AFloorBase.OuterSingleton, Z_Construct_UClass_AFloorBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AFloorBase.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AFloorBase);
AFloorBase::~AFloorBase() {}
// ********** End Class AFloorBase *****************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AFloorBase, AFloorBase::StaticClass, TEXT("AFloorBase"), &Z_Registration_Info_UClass_AFloorBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AFloorBase), 1587253150U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h__Script_RunGame_1737786822(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_Actor_Floor_FloorBase_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
