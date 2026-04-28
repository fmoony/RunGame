// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "WorldSubsystem/RunGameFloorSubsystem.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameFloorSubsystem() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UWorldSubsystem();
RUNGAME_API UClass* Z_Construct_UClass_AFloorBase_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_URunGameFloorSubsystem();
RUNGAME_API UClass* Z_Construct_UClass_URunGameFloorSubsystem_NoRegister();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOnFloorSystemReadyDelegate *******************************************
struct Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnFloorSystemReadyDelegate__DelegateSignature", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnFloorSystemReadyDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnFloorSystemReadyDelegate)
{
	OnFloorSystemReadyDelegate.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FOnFloorSystemReadyDelegate *********************************************

// ********** Begin Class URunGameFloorSubsystem Function ClearAllFloors ***************************
struct Z_Construct_UFunction_URunGameFloorSubsystem_ClearAllFloors_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_ClearAllFloors_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "ClearAllFloors", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_ClearAllFloors_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_ClearAllFloors_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_ClearAllFloors()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_ClearAllFloors_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execClearAllFloors)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ClearAllFloors();
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function ClearAllFloors *****************************

// ********** Begin Class URunGameFloorSubsystem Function GetActiveFloorCount **********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics
{
	struct RunGameFloorSubsystem_eventGetActiveFloorCount_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventGetActiveFloorCount_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "GetActiveFloorCount", Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::RunGameFloorSubsystem_eventGetActiveFloorCount_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::RunGameFloorSubsystem_eventGetActiveFloorCount_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execGetActiveFloorCount)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->GetActiveFloorCount();
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function GetActiveFloorCount ************************

// ********** Begin Class URunGameFloorSubsystem Function GetNextSpawnTransform ********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics
{
	struct RunGameFloorSubsystem_eventGetNextSpawnTransform_Parms
	{
		FTransform ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ===== \xe6\x9f\xa5\xe8\xaf\xa2 =====\n" },
#endif
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "===== \xe6\x9f\xa5\xe8\xaf\xa2 =====" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventGetNextSpawnTransform_Parms, ReturnValue), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "GetNextSpawnTransform", Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::RunGameFloorSubsystem_eventGetNextSpawnTransform_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::RunGameFloorSubsystem_eventGetNextSpawnTransform_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execGetNextSpawnTransform)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FTransform*)Z_Param__Result=P_THIS->GetNextSpawnTransform();
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function GetNextSpawnTransform **********************

// ********** Begin Class URunGameFloorSubsystem Function GetPooledFloorCount **********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics
{
	struct RunGameFloorSubsystem_eventGetPooledFloorCount_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventGetPooledFloorCount_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "GetPooledFloorCount", Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::RunGameFloorSubsystem_eventGetPooledFloorCount_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::RunGameFloorSubsystem_eventGetPooledFloorCount_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execGetPooledFloorCount)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->GetPooledFloorCount();
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function GetPooledFloorCount ************************

// ********** Begin Class URunGameFloorSubsystem Function HideAllActiveFloors **********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_HideAllActiveFloors_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_HideAllActiveFloors_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "HideAllActiveFloors", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_HideAllActiveFloors_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_HideAllActiveFloors_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_HideAllActiveFloors()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_HideAllActiveFloors_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execHideAllActiveFloors)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HideAllActiveFloors();
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function HideAllActiveFloors ************************

// ********** Begin Class URunGameFloorSubsystem Function InitializeFloorSystem ********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics
{
	struct RunGameFloorSubsystem_eventInitializeFloorSystem_Parms
	{
		TArray<TSoftClassPtr<AActor> > InStraightClasses;
		TArray<TSoftClassPtr<AActor> > InTurnClasses;
		int32 InPreAllocateCount;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ===== \xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96 =====\n" },
#endif
		{ "CPP_Default_InPreAllocateCount", "10" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "===== \xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96 =====" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InStraightClasses_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InTurnClasses_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_InStraightClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InStraightClasses;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_InTurnClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InTurnClasses;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InPreAllocateCount;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InStraightClasses_Inner = { "InStraightClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InStraightClasses = { "InStraightClasses", nullptr, (EPropertyFlags)0x0014000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventInitializeFloorSystem_Parms, InStraightClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InStraightClasses_MetaData), NewProp_InStraightClasses_MetaData) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InTurnClasses_Inner = { "InTurnClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InTurnClasses = { "InTurnClasses", nullptr, (EPropertyFlags)0x0014000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventInitializeFloorSystem_Parms, InTurnClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InTurnClasses_MetaData), NewProp_InTurnClasses_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InPreAllocateCount = { "InPreAllocateCount", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventInitializeFloorSystem_Parms, InPreAllocateCount), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InStraightClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InStraightClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InTurnClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InTurnClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::NewProp_InPreAllocateCount,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "InitializeFloorSystem", Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::RunGameFloorSubsystem_eventInitializeFloorSystem_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::RunGameFloorSubsystem_eventInitializeFloorSystem_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execInitializeFloorSystem)
{
	P_GET_TARRAY_REF(TSoftClassPtr<AActor> ,Z_Param_Out_InStraightClasses);
	P_GET_TARRAY_REF(TSoftClassPtr<AActor> ,Z_Param_Out_InTurnClasses);
	P_GET_PROPERTY(FIntProperty,Z_Param_InPreAllocateCount);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->InitializeFloorSystem(Z_Param_Out_InStraightClasses,Z_Param_Out_InTurnClasses,Z_Param_InPreAllocateCount);
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function InitializeFloorSystem **********************

// ********** Begin Class URunGameFloorSubsystem Function OnFloorClassesLoaded *********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_OnFloorClassesLoaded_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_OnFloorClassesLoaded_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "OnFloorClassesLoaded", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_OnFloorClassesLoaded_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_OnFloorClassesLoaded_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_OnFloorClassesLoaded()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_OnFloorClassesLoaded_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execOnFloorClassesLoaded)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnFloorClassesLoaded();
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function OnFloorClassesLoaded ***********************

// ********** Begin Class URunGameFloorSubsystem Function RecycleDistantFloors *********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics
{
	struct RunGameFloorSubsystem_eventRecycleDistantFloors_Parms
	{
		FVector PlayerLocation;
		float MaxDistance;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "CPP_Default_MaxDistance", "3000.000000" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PlayerLocation_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_PlayerLocation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxDistance;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::NewProp_PlayerLocation = { "PlayerLocation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventRecycleDistantFloors_Parms, PlayerLocation), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PlayerLocation_MetaData), NewProp_PlayerLocation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::NewProp_MaxDistance = { "MaxDistance", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventRecycleDistantFloors_Parms, MaxDistance), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::NewProp_PlayerLocation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::NewProp_MaxDistance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "RecycleDistantFloors", Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::RunGameFloorSubsystem_eventRecycleDistantFloors_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::RunGameFloorSubsystem_eventRecycleDistantFloors_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execRecycleDistantFloors)
{
	P_GET_STRUCT_REF(FVector,Z_Param_Out_PlayerLocation);
	P_GET_PROPERTY(FFloatProperty,Z_Param_MaxDistance);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RecycleDistantFloors(Z_Param_Out_PlayerLocation,Z_Param_MaxDistance);
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function RecycleDistantFloors ***********************

// ********** Begin Class URunGameFloorSubsystem Function RequestFloorAt ***************************
struct Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics
{
	struct RunGameFloorSubsystem_eventRequestFloorAt_Parms
	{
		TSubclassOf<AActor> InClass;
		FVector Location;
		FRotator Rotation;
		AFloorBase* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Location_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Rotation_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_InClass;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Rotation;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_InClass = { "InClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventRequestFloorAt_Parms, InClass), Z_Construct_UClass_UClass, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventRequestFloorAt_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Location_MetaData), NewProp_Location_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_Rotation = { "Rotation", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventRequestFloorAt_Parms, Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Rotation_MetaData), NewProp_Rotation_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventRequestFloorAt_Parms, ReturnValue), Z_Construct_UClass_AFloorBase_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_InClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_Location,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_Rotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "RequestFloorAt", Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::RunGameFloorSubsystem_eventRequestFloorAt_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::RunGameFloorSubsystem_eventRequestFloorAt_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execRequestFloorAt)
{
	P_GET_OBJECT(UClass,Z_Param_InClass);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_Location);
	P_GET_STRUCT_REF(FRotator,Z_Param_Out_Rotation);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AFloorBase**)Z_Param__Result=P_THIS->RequestFloorAt(Z_Param_InClass,Z_Param_Out_Location,Z_Param_Out_Rotation);
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function RequestFloorAt *****************************

// ********** Begin Class URunGameFloorSubsystem Function RequestNextFloor *************************
struct Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics
{
	struct RunGameFloorSubsystem_eventRequestNextFloor_Parms
	{
		AFloorBase* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventRequestNextFloor_Parms, ReturnValue), Z_Construct_UClass_AFloorBase_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "RequestNextFloor", Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::RunGameFloorSubsystem_eventRequestNextFloor_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::RunGameFloorSubsystem_eventRequestNextFloor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execRequestNextFloor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AFloorBase**)Z_Param__Result=P_THIS->RequestNextFloor();
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function RequestNextFloor ***************************

// ********** Begin Class URunGameFloorSubsystem Function ReturnFloor ******************************
struct Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics
{
	struct RunGameFloorSubsystem_eventReturnFloor_Parms
	{
		AFloorBase* Floor;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ===== \xe5\x9c\xb0\xe6\x9d\xbf\xe5\x9b\x9e\xe6\x94\xb6 =====\n" },
#endif
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "===== \xe5\x9c\xb0\xe6\x9d\xbf\xe5\x9b\x9e\xe6\x94\xb6 =====" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Floor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::NewProp_Floor = { "Floor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventReturnFloor_Parms, Floor), Z_Construct_UClass_AFloorBase_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::NewProp_Floor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "ReturnFloor", Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::RunGameFloorSubsystem_eventReturnFloor_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::RunGameFloorSubsystem_eventReturnFloor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execReturnFloor)
{
	P_GET_OBJECT(AFloorBase,Z_Param_Floor);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ReturnFloor(Z_Param_Floor);
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function ReturnFloor ********************************

// ********** Begin Class URunGameFloorSubsystem Function SpawnInitialFloors ***********************
struct Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics
{
	struct RunGameFloorSubsystem_eventSpawnInitialFloors_Parms
	{
		FTransform StartTransform;
		int32 StraightCount;
		int32 RandomCount;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|FloorSystem" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ===== \xe5\x9c\xb0\xe6\x9d\xbf\xe7\x94\x9f\xe6\x88\x90 =====\n" },
#endif
		{ "CPP_Default_RandomCount", "15" },
		{ "CPP_Default_StraightCount", "5" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "===== \xe5\x9c\xb0\xe6\x9d\xbf\xe7\x94\x9f\xe6\x88\x90 =====" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartTransform_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_StartTransform;
	static const UECodeGen_Private::FIntPropertyParams NewProp_StraightCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_RandomCount;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::NewProp_StartTransform = { "StartTransform", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventSpawnInitialFloors_Parms, StartTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartTransform_MetaData), NewProp_StartTransform_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::NewProp_StraightCount = { "StraightCount", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventSpawnInitialFloors_Parms, StraightCount), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::NewProp_RandomCount = { "RandomCount", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameFloorSubsystem_eventSpawnInitialFloors_Parms, RandomCount), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::NewProp_StartTransform,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::NewProp_StraightCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::NewProp_RandomCount,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameFloorSubsystem, nullptr, "SpawnInitialFloors", Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::RunGameFloorSubsystem_eventSpawnInitialFloors_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::RunGameFloorSubsystem_eventSpawnInitialFloors_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameFloorSubsystem::execSpawnInitialFloors)
{
	P_GET_STRUCT_REF(FTransform,Z_Param_Out_StartTransform);
	P_GET_PROPERTY(FIntProperty,Z_Param_StraightCount);
	P_GET_PROPERTY(FIntProperty,Z_Param_RandomCount);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SpawnInitialFloors(Z_Param_Out_StartTransform,Z_Param_StraightCount,Z_Param_RandomCount);
	P_NATIVE_END;
}
// ********** End Class URunGameFloorSubsystem Function SpawnInitialFloors *************************

// ********** Begin Class URunGameFloorSubsystem ***************************************************
void URunGameFloorSubsystem::StaticRegisterNativesURunGameFloorSubsystem()
{
	UClass* Class = URunGameFloorSubsystem::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ClearAllFloors", &URunGameFloorSubsystem::execClearAllFloors },
		{ "GetActiveFloorCount", &URunGameFloorSubsystem::execGetActiveFloorCount },
		{ "GetNextSpawnTransform", &URunGameFloorSubsystem::execGetNextSpawnTransform },
		{ "GetPooledFloorCount", &URunGameFloorSubsystem::execGetPooledFloorCount },
		{ "HideAllActiveFloors", &URunGameFloorSubsystem::execHideAllActiveFloors },
		{ "InitializeFloorSystem", &URunGameFloorSubsystem::execInitializeFloorSystem },
		{ "OnFloorClassesLoaded", &URunGameFloorSubsystem::execOnFloorClassesLoaded },
		{ "RecycleDistantFloors", &URunGameFloorSubsystem::execRecycleDistantFloors },
		{ "RequestFloorAt", &URunGameFloorSubsystem::execRequestFloorAt },
		{ "RequestNextFloor", &URunGameFloorSubsystem::execRequestNextFloor },
		{ "ReturnFloor", &URunGameFloorSubsystem::execReturnFloor },
		{ "SpawnInitialFloors", &URunGameFloorSubsystem::execSpawnInitialFloors },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_URunGameFloorSubsystem;
UClass* URunGameFloorSubsystem::GetPrivateStaticClass()
{
	using TClass = URunGameFloorSubsystem;
	if (!Z_Registration_Info_UClass_URunGameFloorSubsystem.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameFloorSubsystem"),
			Z_Registration_Info_UClass_URunGameFloorSubsystem.InnerSingleton,
			StaticRegisterNativesURunGameFloorSubsystem,
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
	return Z_Registration_Info_UClass_URunGameFloorSubsystem.InnerSingleton;
}
UClass* Z_Construct_UClass_URunGameFloorSubsystem_NoRegister()
{
	return URunGameFloorSubsystem::GetPrivateStaticClass();
}
struct Z_Construct_UClass_URunGameFloorSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnFloorSystemReady_MetaData[] = {
		{ "Category", "RunGame|FloorSystem" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ===== \xe5\xbc\x82\xe6\xad\xa5\xe5\x8a\xa0\xe8\xbd\xbd\xe5\xb0\xb1\xe7\xbb\xaa\xe4\xba\x8b\xe4\xbb\xb6 =====\n" },
#endif
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "===== \xe5\xbc\x82\xe6\xad\xa5\xe5\x8a\xa0\xe8\xbd\xbd\xe5\xb0\xb1\xe7\xbb\xaa\xe4\xba\x8b\xe4\xbb\xb6 =====" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LoadedStraightClasses_MetaData[] = {
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LoadedTurnClasses_MetaData[] = {
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveFloors_MetaData[] = {
		{ "ModuleRelativePath", "WorldSubsystem/RunGameFloorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnFloorSystemReady;
	static const UECodeGen_Private::FClassPropertyParams NewProp_LoadedStraightClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_LoadedStraightClasses;
	static const UECodeGen_Private::FClassPropertyParams NewProp_LoadedTurnClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_LoadedTurnClasses;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ActiveFloors_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ActiveFloors;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_ClearAllFloors, "ClearAllFloors" }, // 1522264240
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_GetActiveFloorCount, "GetActiveFloorCount" }, // 3650085621
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_GetNextSpawnTransform, "GetNextSpawnTransform" }, // 1990040255
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_GetPooledFloorCount, "GetPooledFloorCount" }, // 1294560631
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_HideAllActiveFloors, "HideAllActiveFloors" }, // 798140802
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_InitializeFloorSystem, "InitializeFloorSystem" }, // 2795690889
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_OnFloorClassesLoaded, "OnFloorClassesLoaded" }, // 817852243
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_RecycleDistantFloors, "RecycleDistantFloors" }, // 163119645
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_RequestFloorAt, "RequestFloorAt" }, // 2715406005
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_RequestNextFloor, "RequestNextFloor" }, // 2068644370
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_ReturnFloor, "ReturnFloor" }, // 273815995
		{ &Z_Construct_UFunction_URunGameFloorSubsystem_SpawnInitialFloors, "SpawnInitialFloors" }, // 3809549750
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URunGameFloorSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_OnFloorSystemReady = { "OnFloorSystemReady", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameFloorSubsystem, OnFloorSystemReady), Z_Construct_UDelegateFunction_RunGame_OnFloorSystemReadyDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnFloorSystemReady_MetaData), NewProp_OnFloorSystemReady_MetaData) }; // 4227961251
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedStraightClasses_Inner = { "LoadedStraightClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedStraightClasses = { "LoadedStraightClasses", nullptr, (EPropertyFlags)0x0044000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameFloorSubsystem, LoadedStraightClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LoadedStraightClasses_MetaData), NewProp_LoadedStraightClasses_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedTurnClasses_Inner = { "LoadedTurnClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedTurnClasses = { "LoadedTurnClasses", nullptr, (EPropertyFlags)0x0044000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameFloorSubsystem, LoadedTurnClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LoadedTurnClasses_MetaData), NewProp_LoadedTurnClasses_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_ActiveFloors_Inner = { "ActiveFloors", nullptr, (EPropertyFlags)0x0104000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AFloorBase_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_ActiveFloors = { "ActiveFloors", nullptr, (EPropertyFlags)0x0144000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameFloorSubsystem, ActiveFloors), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveFloors_MetaData), NewProp_ActiveFloors_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URunGameFloorSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_OnFloorSystemReady,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedStraightClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedStraightClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedTurnClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_LoadedTurnClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_ActiveFloors_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameFloorSubsystem_Statics::NewProp_ActiveFloors,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameFloorSubsystem_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_URunGameFloorSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWorldSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameFloorSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_URunGameFloorSubsystem_Statics::ClassParams = {
	&URunGameFloorSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_URunGameFloorSubsystem_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_URunGameFloorSubsystem_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameFloorSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_URunGameFloorSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_URunGameFloorSubsystem()
{
	if (!Z_Registration_Info_UClass_URunGameFloorSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URunGameFloorSubsystem.OuterSingleton, Z_Construct_UClass_URunGameFloorSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_URunGameFloorSubsystem.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(URunGameFloorSubsystem);
URunGameFloorSubsystem::~URunGameFloorSubsystem() {}
// ********** End Class URunGameFloorSubsystem *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_URunGameFloorSubsystem, URunGameFloorSubsystem::StaticClass, TEXT("URunGameFloorSubsystem"), &Z_Registration_Info_UClass_URunGameFloorSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URunGameFloorSubsystem), 782202269U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h__Script_RunGame_3738889164(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameFloorSubsystem_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
