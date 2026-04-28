// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RunGamePlayerState.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGamePlayerState() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_APlayerState();
RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerState();
RUNGAME_API UClass* Z_Construct_UClass_ARunGamePlayerState_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_URunGameTimerSubsystem_NoRegister();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOnScoreChangedDelegate ***********************************************
struct Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics
{
	struct _Script_RunGame_eventOnScoreChangedDelegate_Parms
	{
		int64 NewScore;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_NewScore;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::NewProp_NewScore = { "NewScore", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnScoreChangedDelegate_Parms, NewScore), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::NewProp_NewScore,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnScoreChangedDelegate__DelegateSignature", Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::_Script_RunGame_eventOnScoreChangedDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::_Script_RunGame_eventOnScoreChangedDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnScoreChangedDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnScoreChangedDelegate, int64 NewScore)
{
	struct _Script_RunGame_eventOnScoreChangedDelegate_Parms
	{
		int64 NewScore;
	};
	_Script_RunGame_eventOnScoreChangedDelegate_Parms Parms;
	Parms.NewScore=NewScore;
	OnScoreChangedDelegate.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnScoreChangedDelegate *************************************************

// ********** Begin Class ARunGamePlayerState Function AddScore ************************************
struct Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics
{
	struct RunGamePlayerState_eventAddScore_Parms
	{
		int64 Value;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Score" },
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_Value;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGamePlayerState_eventAddScore_Parms, Value), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::NewProp_Value,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerState, nullptr, "AddScore", Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::RunGamePlayerState_eventAddScore_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::RunGamePlayerState_eventAddScore_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGamePlayerState_AddScore()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerState_AddScore_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerState::execAddScore)
{
	P_GET_PROPERTY(FInt64Property,Z_Param_Value);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddScore(Z_Param_Value);
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerState Function AddScore **************************************

// ********** Begin Class ARunGamePlayerState Function CalculateScoreProcess ***********************
struct Z_Construct_UFunction_ARunGamePlayerState_CalculateScoreProcess_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// 0.1s \xe5\xbe\xaa\xe7\x8e\xaf\xe5\xae\x9a\xe6\x97\xb6\xe5\x99\xa8\xe5\x9b\x9e\xe8\xb0\x83\n" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "0.1s \xe5\xbe\xaa\xe7\x8e\xaf\xe5\xae\x9a\xe6\x97\xb6\xe5\x99\xa8\xe5\x9b\x9e\xe8\xb0\x83" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerState_CalculateScoreProcess_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerState, nullptr, "CalculateScoreProcess", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_CalculateScoreProcess_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerState_CalculateScoreProcess_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGamePlayerState_CalculateScoreProcess()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerState_CalculateScoreProcess_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerState::execCalculateScoreProcess)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CalculateScoreProcess();
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerState Function CalculateScoreProcess *************************

// ********** Begin Class ARunGamePlayerState Function GetRunGameScore *****************************
struct Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics
{
	struct RunGamePlayerState_eventGetRunGameScore_Parms
	{
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Score" },
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGamePlayerState_eventGetRunGameScore_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerState, nullptr, "GetRunGameScore", Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::RunGamePlayerState_eventGetRunGameScore_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::RunGamePlayerState_eventGetRunGameScore_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerState::execGetRunGameScore)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=P_THIS->GetRunGameScore();
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerState Function GetRunGameScore *******************************

// ********** Begin Class ARunGamePlayerState Function OnGameStateChangedCallback ******************
struct Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics
{
	struct RunGamePlayerState_eventOnGameStateChangedCallback_Parms
	{
		ERunGameGameState OldState;
		ERunGameGameState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x93\x8d\xe5\xba\x94\xe6\xb8\xb8\xe6\x88\x8f\xe7\x8a\xb6\xe6\x80\x81\xe5\x8f\x98\xe5\x8c\x96\xef\xbc\x8c\xe5\x90\xaf\xe5\x81\x9c\xe7\xae\x97\xe5\x88\x86\xe5\xae\x9a\xe6\x97\xb6\xe5\x99\xa8\n" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x93\x8d\xe5\xba\x94\xe6\xb8\xb8\xe6\x88\x8f\xe7\x8a\xb6\xe6\x80\x81\xe5\x8f\x98\xe5\x8c\x96\xef\xbc\x8c\xe5\x90\xaf\xe5\x81\x9c\xe7\xae\x97\xe5\x88\x86\xe5\xae\x9a\xe6\x97\xb6\xe5\x99\xa8" },
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
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_OldState = { "OldState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGamePlayerState_eventOnGameStateChangedCallback_Parms, OldState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGamePlayerState_eventOnGameStateChangedCallback_Parms, NewState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_OldState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerState, nullptr, "OnGameStateChangedCallback", Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::RunGamePlayerState_eventOnGameStateChangedCallback_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::RunGamePlayerState_eventOnGameStateChangedCallback_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerState::execOnGameStateChangedCallback)
{
	P_GET_ENUM(ERunGameGameState,Z_Param_OldState);
	P_GET_ENUM(ERunGameGameState,Z_Param_NewState);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnGameStateChangedCallback(ERunGameGameState(Z_Param_OldState),ERunGameGameState(Z_Param_NewState));
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerState Function OnGameStateChangedCallback ********************

// ********** Begin Class ARunGamePlayerState Function SetRunGameScore *****************************
struct Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics
{
	struct RunGamePlayerState_eventSetRunGameScore_Parms
	{
		int64 NewScore;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Score" },
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FInt64PropertyParams NewProp_NewScore;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::NewProp_NewScore = { "NewScore", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGamePlayerState_eventSetRunGameScore_Parms, NewScore), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::NewProp_NewScore,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerState, nullptr, "SetRunGameScore", Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::RunGamePlayerState_eventSetRunGameScore_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::RunGamePlayerState_eventSetRunGameScore_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerState::execSetRunGameScore)
{
	P_GET_PROPERTY(FInt64Property,Z_Param_NewScore);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetRunGameScore(Z_Param_NewScore);
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerState Function SetRunGameScore *******************************

// ********** Begin Class ARunGamePlayerState Function SetScoringActive ****************************
struct Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics
{
	struct RunGamePlayerState_eventSetScoringActive_Parms
	{
		bool bActive;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Score" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xa4\x96\xe9\x83\xa8\xe6\x8e\xa7\xe5\x88\xb6\xe7\xae\x97\xe5\x88\x86\xe5\x90\xaf\xe5\x81\x9c\xef\xbc\x88\xe4\xbe\x9b GameState \xe7\x8a\xb6\xe6\x80\x81\xe6\x9c\xba\xe8\xb0\x83\xe7\x94\xa8\xef\xbc\x89\n" },
#endif
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xa4\x96\xe9\x83\xa8\xe6\x8e\xa7\xe5\x88\xb6\xe7\xae\x97\xe5\x88\x86\xe5\x90\xaf\xe5\x81\x9c\xef\xbc\x88\xe4\xbe\x9b GameState \xe7\x8a\xb6\xe6\x80\x81\xe6\x9c\xba\xe8\xb0\x83\xe7\x94\xa8\xef\xbc\x89" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bActive_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bActive;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::NewProp_bActive_SetBit(void* Obj)
{
	((RunGamePlayerState_eventSetScoringActive_Parms*)Obj)->bActive = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::NewProp_bActive = { "bActive", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(RunGamePlayerState_eventSetScoringActive_Parms), &Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::NewProp_bActive_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::NewProp_bActive,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGamePlayerState, nullptr, "SetScoringActive", Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::RunGamePlayerState_eventSetScoringActive_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::RunGamePlayerState_eventSetScoringActive_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGamePlayerState::execSetScoringActive)
{
	P_GET_UBOOL(Z_Param_bActive);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetScoringActive(Z_Param_bActive);
	P_NATIVE_END;
}
// ********** End Class ARunGamePlayerState Function SetScoringActive ******************************

// ********** Begin Class ARunGamePlayerState ******************************************************
void ARunGamePlayerState::StaticRegisterNativesARunGamePlayerState()
{
	UClass* Class = ARunGamePlayerState::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "AddScore", &ARunGamePlayerState::execAddScore },
		{ "CalculateScoreProcess", &ARunGamePlayerState::execCalculateScoreProcess },
		{ "GetRunGameScore", &ARunGamePlayerState::execGetRunGameScore },
		{ "OnGameStateChangedCallback", &ARunGamePlayerState::execOnGameStateChangedCallback },
		{ "SetRunGameScore", &ARunGamePlayerState::execSetRunGameScore },
		{ "SetScoringActive", &ARunGamePlayerState::execSetScoringActive },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGamePlayerState;
UClass* ARunGamePlayerState::GetPrivateStaticClass()
{
	using TClass = ARunGamePlayerState;
	if (!Z_Registration_Info_UClass_ARunGamePlayerState.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGamePlayerState"),
			Z_Registration_Info_UClass_ARunGamePlayerState.InnerSingleton,
			StaticRegisterNativesARunGamePlayerState,
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
	return Z_Registration_Info_UClass_ARunGamePlayerState.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGamePlayerState_NoRegister()
{
	return ARunGamePlayerState::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGamePlayerState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe5\x88\x86\xe6\x95\xb0\xe7\x94\xb1 0.1s \xe5\x90\x8e\xe5\x8f\xb0\xe5\xae\x9a\xe6\x97\xb6\xe5\x99\xa8\xe9\xa9\xb1\xe5\x8a\xa8\xef\xbc\x8c\xe6\x9d\x9c\xe7\xbb\x9d Tick \xe5\xbc\x80\xe9\x94\x80\xe3\x80\x82\n * \xe5\x80\x8d\xe7\x8e\x87\xe4\xbd\xbf\xe7\x94\xa8\xe4\xb8\x89\xe6\xac\xa1\xe5\xa4\x9a\xe9\xa1\xb9\xe5\xbc\x8f (Cubic Polynomial) \xe5\xb9\xb3\xe6\xbb\x91\xe7\x88\x86\xe5\x8f\x91\xef\xbc\x8c\xe9\x81\xbf\xe5\x85\x8d\xe6\x8c\x87\xe6\x95\xb0\xe6\xba\xa2\xe5\x87\xba\xe3\x80\x82\n * \xe5\x88\x86\xe6\x95\xb0\xe5\xad\x98\xe5\x82\xa8\xe5\x92\x8c\xe8\xbf\x90\xe7\xae\x97\xe5\x85\xa8\xe9\x83\xa8\xe4\xbd\xbf\xe7\x94\xa8 int64\xef\xbc\x8c\xe5\xae\x89\xe5\x85\xa8\xe6\x89\xbf\xe8\xbd\xbd\xe6\xb8\xb8\xe6\x88\x8f\xe5\x90\x8e\xe6\x9c\x9f\xe6\x9e\x81\xe9\xab\x98\xe5\x88\x86\xe6\x95\xb0\xe3\x80\x82\n */" },
#endif
		{ "HideCategories", "Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "RunGamePlayerState.h" },
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x88\x86\xe6\x95\xb0\xe7\x94\xb1 0.1s \xe5\x90\x8e\xe5\x8f\xb0\xe5\xae\x9a\xe6\x97\xb6\xe5\x99\xa8\xe9\xa9\xb1\xe5\x8a\xa8\xef\xbc\x8c\xe6\x9d\x9c\xe7\xbb\x9d Tick \xe5\xbc\x80\xe9\x94\x80\xe3\x80\x82\n\xe5\x80\x8d\xe7\x8e\x87\xe4\xbd\xbf\xe7\x94\xa8\xe4\xb8\x89\xe6\xac\xa1\xe5\xa4\x9a\xe9\xa1\xb9\xe5\xbc\x8f (Cubic Polynomial) \xe5\xb9\xb3\xe6\xbb\x91\xe7\x88\x86\xe5\x8f\x91\xef\xbc\x8c\xe9\x81\xbf\xe5\x85\x8d\xe6\x8c\x87\xe6\x95\xb0\xe6\xba\xa2\xe5\x87\xba\xe3\x80\x82\n\xe5\x88\x86\xe6\x95\xb0\xe5\xad\x98\xe5\x82\xa8\xe5\x92\x8c\xe8\xbf\x90\xe7\xae\x97\xe5\x85\xa8\xe9\x83\xa8\xe4\xbd\xbf\xe7\x94\xa8 int64\xef\xbc\x8c\xe5\xae\x89\xe5\x85\xa8\xe6\x89\xbf\xe8\xbd\xbd\xe6\xb8\xb8\xe6\x88\x8f\xe5\x90\x8e\xe6\x9c\x9f\xe6\x9e\x81\xe9\xab\x98\xe5\x88\x86\xe6\x95\xb0\xe3\x80\x82" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnScoreChanged_MetaData[] = {
		{ "Category", "RunGame|Score" },
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RunGameScore_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "RunGame|Score" },
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerSubsystem_MetaData[] = {
		{ "ModuleRelativePath", "RunGamePlayerState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnScoreChanged;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_RunGameScore;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TimerSubsystem;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ARunGamePlayerState_AddScore, "AddScore" }, // 19787034
		{ &Z_Construct_UFunction_ARunGamePlayerState_CalculateScoreProcess, "CalculateScoreProcess" }, // 2849025690
		{ &Z_Construct_UFunction_ARunGamePlayerState_GetRunGameScore, "GetRunGameScore" }, // 1226617115
		{ &Z_Construct_UFunction_ARunGamePlayerState_OnGameStateChangedCallback, "OnGameStateChangedCallback" }, // 1502724539
		{ &Z_Construct_UFunction_ARunGamePlayerState_SetRunGameScore, "SetRunGameScore" }, // 1566910724
		{ &Z_Construct_UFunction_ARunGamePlayerState_SetScoringActive, "SetScoringActive" }, // 3251585120
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGamePlayerState>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ARunGamePlayerState_Statics::NewProp_OnScoreChanged = { "OnScoreChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGamePlayerState, OnScoreChanged), Z_Construct_UDelegateFunction_RunGame_OnScoreChangedDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnScoreChanged_MetaData), NewProp_OnScoreChanged_MetaData) }; // 1429249325
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UClass_ARunGamePlayerState_Statics::NewProp_RunGameScore = { "RunGameScore", nullptr, (EPropertyFlags)0x0020080000020015, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGamePlayerState, RunGameScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RunGameScore_MetaData), NewProp_RunGameScore_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGamePlayerState_Statics::NewProp_TimerSubsystem = { "TimerSubsystem", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGamePlayerState, TimerSubsystem), Z_Construct_UClass_URunGameTimerSubsystem_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerSubsystem_MetaData), NewProp_TimerSubsystem_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGamePlayerState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerState_Statics::NewProp_OnScoreChanged,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerState_Statics::NewProp_RunGameScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGamePlayerState_Statics::NewProp_TimerSubsystem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerState_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGamePlayerState_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerState,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerState_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGamePlayerState_Statics::ClassParams = {
	&ARunGamePlayerState::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ARunGamePlayerState_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerState_Statics::PropPointers),
	0,
	0x009003A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGamePlayerState_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGamePlayerState_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGamePlayerState()
{
	if (!Z_Registration_Info_UClass_ARunGamePlayerState.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGamePlayerState.OuterSingleton, Z_Construct_UClass_ARunGamePlayerState_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGamePlayerState.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGamePlayerState);
ARunGamePlayerState::~ARunGamePlayerState() {}
// ********** End Class ARunGamePlayerState ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerState_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGamePlayerState, ARunGamePlayerState::StaticClass, TEXT("ARunGamePlayerState"), &Z_Registration_Info_UClass_ARunGamePlayerState, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGamePlayerState), 2390731764U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerState_h__Script_RunGame_735411331(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerState_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGamePlayerState_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
