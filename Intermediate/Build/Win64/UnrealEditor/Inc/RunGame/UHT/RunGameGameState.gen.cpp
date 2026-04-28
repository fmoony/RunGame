// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RunGameGameState.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameGameState() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AGameStateBase();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameState();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameState_NoRegister();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOnCharacterDeathDelegate *********************************************
struct Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnCharacterDeathDelegate__DelegateSignature", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnCharacterDeathDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnCharacterDeathDelegate)
{
	OnCharacterDeathDelegate.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FOnCharacterDeathDelegate ***********************************************

// ********** Begin Delegate FOnGameStateChangedSignature ******************************************
struct Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics
{
	struct _Script_RunGame_eventOnGameStateChangedSignature_Parms
	{
		ERunGameGameState OldGameState;
		ERunGameGameState NewGameState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_OldGameState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_OldGameState;
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewGameState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NewGameState;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_OldGameState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_OldGameState = { "OldGameState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnGameStateChangedSignature_Parms, OldGameState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_NewGameState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_NewGameState = { "NewGameState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnGameStateChangedSignature_Parms, NewGameState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_OldGameState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_OldGameState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_NewGameState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::NewProp_NewGameState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnGameStateChangedSignature__DelegateSignature", Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::_Script_RunGame_eventOnGameStateChangedSignature_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::_Script_RunGame_eventOnGameStateChangedSignature_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnGameStateChangedSignature_DelegateWrapper(const FMulticastScriptDelegate& OnGameStateChangedSignature, ERunGameGameState OldGameState, ERunGameGameState NewGameState)
{
	struct _Script_RunGame_eventOnGameStateChangedSignature_Parms
	{
		ERunGameGameState OldGameState;
		ERunGameGameState NewGameState;
	};
	_Script_RunGame_eventOnGameStateChangedSignature_Parms Parms;
	Parms.OldGameState=OldGameState;
	Parms.NewGameState=NewGameState;
	OnGameStateChangedSignature.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnGameStateChangedSignature ********************************************

// ********** Begin Delegate FOnCountdownUpdatedSignature ******************************************
struct Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics
{
	struct _Script_RunGame_eventOnCountdownUpdatedSignature_Parms
	{
		int32 CountdownSeconds;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_CountdownSeconds;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::NewProp_CountdownSeconds = { "CountdownSeconds", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnCountdownUpdatedSignature_Parms, CountdownSeconds), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::NewProp_CountdownSeconds,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnCountdownUpdatedSignature__DelegateSignature", Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::_Script_RunGame_eventOnCountdownUpdatedSignature_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::_Script_RunGame_eventOnCountdownUpdatedSignature_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnCountdownUpdatedSignature_DelegateWrapper(const FMulticastScriptDelegate& OnCountdownUpdatedSignature, int32 CountdownSeconds)
{
	struct _Script_RunGame_eventOnCountdownUpdatedSignature_Parms
	{
		int32 CountdownSeconds;
	};
	_Script_RunGame_eventOnCountdownUpdatedSignature_Parms Parms;
	Parms.CountdownSeconds=CountdownSeconds;
	OnCountdownUpdatedSignature.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnCountdownUpdatedSignature ********************************************

// ********** Begin Class ARunGameGameState Function GetCountdownSeconds ***************************
struct Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics
{
	struct RunGameGameState_eventGetCountdownSeconds_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Countdown" },
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameState_eventGetCountdownSeconds_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameState, nullptr, "GetCountdownSeconds", Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::RunGameGameState_eventGetCountdownSeconds_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::RunGameGameState_eventGetCountdownSeconds_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameState::execGetCountdownSeconds)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->GetCountdownSeconds();
	P_NATIVE_END;
}
// ********** End Class ARunGameGameState Function GetCountdownSeconds *****************************

// ********** Begin Class ARunGameGameState Function GetCurrentState *******************************
struct Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics
{
	struct RunGameGameState_eventGetCurrentState_Parms
	{
		ERunGameGameState ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|State" },
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameState_eventGetCurrentState_Parms, ReturnValue), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameState, nullptr, "GetCurrentState", Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::RunGameGameState_eventGetCurrentState_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::RunGameGameState_eventGetCurrentState_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameGameState_GetCurrentState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameState_GetCurrentState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameState::execGetCurrentState)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(ERunGameGameState*)Z_Param__Result=P_THIS->GetCurrentState();
	P_NATIVE_END;
}
// ********** End Class ARunGameGameState Function GetCurrentState *********************************

// ********** Begin Class ARunGameGameState Function SetCountdownSeconds ***************************
struct Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics
{
	struct RunGameGameState_eventSetCountdownSeconds_Parms
	{
		int32 NewCountdownSeconds;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Countdown" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ---- \xe5\x80\x92\xe8\xae\xa1\xe6\x97\xb6\xe6\x95\xb0\xe6\x8d\xae (\xe5\x8d\x95\xe4\xb8\x80\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90) ----\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "---- \xe5\x80\x92\xe8\xae\xa1\xe6\x97\xb6\xe6\x95\xb0\xe6\x8d\xae (\xe5\x8d\x95\xe4\xb8\x80\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90) ----" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_NewCountdownSeconds;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::NewProp_NewCountdownSeconds = { "NewCountdownSeconds", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameState_eventSetCountdownSeconds_Parms, NewCountdownSeconds), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::NewProp_NewCountdownSeconds,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameState, nullptr, "SetCountdownSeconds", Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::RunGameGameState_eventSetCountdownSeconds_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::RunGameGameState_eventSetCountdownSeconds_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameState::execSetCountdownSeconds)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_NewCountdownSeconds);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetCountdownSeconds(Z_Param_NewCountdownSeconds);
	P_NATIVE_END;
}
// ********** End Class ARunGameGameState Function SetCountdownSeconds *****************************

// ********** Begin Class ARunGameGameState Function SetGameState **********************************
struct Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics
{
	struct RunGameGameState_eventSetGameState_Parms
	{
		ERunGameGameState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|State" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ---- \xe6\xb8\xb8\xe6\x88\x8f\xe7\x8a\xb6\xe6\x80\x81\xe6\x8e\xa7\xe5\x88\xb6 ----\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "---- \xe6\xb8\xb8\xe6\x88\x8f\xe7\x8a\xb6\xe6\x80\x81\xe6\x8e\xa7\xe5\x88\xb6 ----" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NewState;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameState_eventSetGameState_Parms, NewState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameState, nullptr, "SetGameState", Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::RunGameGameState_eventSetGameState_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::RunGameGameState_eventSetGameState_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameGameState_SetGameState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameState_SetGameState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameState::execSetGameState)
{
	P_GET_ENUM(ERunGameGameState,Z_Param_NewState);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetGameState(ERunGameGameState(Z_Param_NewState));
	P_NATIVE_END;
}
// ********** End Class ARunGameGameState Function SetGameState ************************************

// ********** Begin Class ARunGameGameState ********************************************************
void ARunGameGameState::StaticRegisterNativesARunGameGameState()
{
	UClass* Class = ARunGameGameState::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetCountdownSeconds", &ARunGameGameState::execGetCountdownSeconds },
		{ "GetCurrentState", &ARunGameGameState::execGetCurrentState },
		{ "SetCountdownSeconds", &ARunGameGameState::execSetCountdownSeconds },
		{ "SetGameState", &ARunGameGameState::execSetGameState },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGameGameState;
UClass* ARunGameGameState::GetPrivateStaticClass()
{
	using TClass = ARunGameGameState;
	if (!Z_Registration_Info_UClass_ARunGameGameState.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameGameState"),
			Z_Registration_Info_UClass_ARunGameGameState.InnerSingleton,
			StaticRegisterNativesARunGameGameState,
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
	return Z_Registration_Info_UClass_ARunGameGameState.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGameGameState_NoRegister()
{
	return ARunGameGameState::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGameGameState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe5\x85\xa8\xe6\xb8\xb8\xe6\x88\x8f\xe5\x94\xaf\xe4\xb8\x80\xe6\x95\xb0\xe6\x8d\xae\xe4\xb8\xad\xe5\xbf\x83 (Single Source of Truth)\n * \xe7\x8a\xb6\xe6\x80\x81\xe3\x80\x81\xe5\x80\x92\xe8\xae\xa1\xe6\x97\xb6\xe7\x9a\x84\xe7\xa7\x92\xe6\x95\xb0\xe5\x8f\x8a\xe5\x8f\xaf\xe9\x85\x8d\xe7\xbd\xae\xe9\xbb\x98\xe8\xae\xa4\xe5\x80\xbc\xe5\x9d\x87\xe7\x94\xb1 GameState \xe9\x9b\x86\xe4\xb8\xad\xe7\xae\xa1\xe7\x90\x86\xe3\x80\x82\n * \xe6\xb8\xb8\xe6\x88\x8f\xe5\x86\x85\xe8\xae\xa1\xe6\x97\xb6\xe4\xb8\xba\xe6\xad\xa3\xe5\x90\x91\xe7\xb4\xaf\xe8\xae\xa1\xef\xbc\x88\xe4\xbb\x8e 0 \xe5\x90\x91\xe4\xb8\x8a\xe9\x80\x92\xe5\xa2\x9e\xef\xbc\x89\xef\xbc\x8c\xe7\x94\xb1 TimerSubsystem \xe9\xa9\xb1\xe5\x8a\xa8\xe3\x80\x82\n */" },
#endif
		{ "HideCategories", "Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "RunGameGameState.h" },
		{ "ModuleRelativePath", "RunGameGameState.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x85\xa8\xe6\xb8\xb8\xe6\x88\x8f\xe5\x94\xaf\xe4\xb8\x80\xe6\x95\xb0\xe6\x8d\xae\xe4\xb8\xad\xe5\xbf\x83 (Single Source of Truth)\n\xe7\x8a\xb6\xe6\x80\x81\xe3\x80\x81\xe5\x80\x92\xe8\xae\xa1\xe6\x97\xb6\xe7\x9a\x84\xe7\xa7\x92\xe6\x95\xb0\xe5\x8f\x8a\xe5\x8f\xaf\xe9\x85\x8d\xe7\xbd\xae\xe9\xbb\x98\xe8\xae\xa4\xe5\x80\xbc\xe5\x9d\x87\xe7\x94\xb1 GameState \xe9\x9b\x86\xe4\xb8\xad\xe7\xae\xa1\xe7\x90\x86\xe3\x80\x82\n\xe6\xb8\xb8\xe6\x88\x8f\xe5\x86\x85\xe8\xae\xa1\xe6\x97\xb6\xe4\xb8\xba\xe6\xad\xa3\xe5\x90\x91\xe7\xb4\xaf\xe8\xae\xa1\xef\xbc\x88\xe4\xbb\x8e 0 \xe5\x90\x91\xe4\xb8\x8a\xe9\x80\x92\xe5\xa2\x9e\xef\xbc\x89\xef\xbc\x8c\xe7\x94\xb1 TimerSubsystem \xe9\xa9\xb1\xe5\x8a\xa8\xe3\x80\x82" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultCountdownSeconds_MetaData[] = {
		{ "Category", "RunGame|Config" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ---- \xe5\x8f\xaf\xe9\x85\x8d\xe7\xbd\xae\xe9\xbb\x98\xe8\xae\xa4\xe5\x80\xbc (\xe4\xbe\x9b Subsystem \xe5\x93\x8d\xe5\xba\x94\xe5\xbc\x8f\xe8\xaf\xbb\xe5\x8f\x96) ----\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "---- \xe5\x8f\xaf\xe9\x85\x8d\xe7\xbd\xae\xe9\xbb\x98\xe8\xae\xa4\xe5\x80\xbc (\xe4\xbe\x9b Subsystem \xe5\x93\x8d\xe5\xba\x94\xe5\xbc\x8f\xe8\xaf\xbb\xe5\x8f\x96) ----" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultGameTotalTime_MetaData[] = {
		{ "Category", "RunGame|Config" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xad\xa3\xe5\x90\x91\xe8\xae\xa1\xe6\x97\xb6\xe7\x9a\x84\xe6\x97\xb6\xe9\x97\xb4\xe4\xb8\x8a\xe9\x99\x90\xef\xbc\x88""0.0 \xe8\xa1\xa8\xe7\xa4\xba\xe6\x97\xa0\xe4\xb8\x8a\xe9\x99\x90\xef\xbc\x8c\xe4\xbb\x85\xe6\xad\xa3\xe5\x90\x91\xe7\xb4\xaf\xe8\xae\xa1\xef\xbc\x89\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xad\xa3\xe5\x90\x91\xe8\xae\xa1\xe6\x97\xb6\xe7\x9a\x84\xe6\x97\xb6\xe9\x97\xb4\xe4\xb8\x8a\xe9\x99\x90\xef\xbc\x88""0.0 \xe8\xa1\xa8\xe7\xa4\xba\xe6\x97\xa0\xe4\xb8\x8a\xe9\x99\x90\xef\xbc\x8c\xe4\xbb\x85\xe6\xad\xa3\xe5\x90\x91\xe7\xb4\xaf\xe8\xae\xa1\xef\xbc\x89" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnGameStateChanged_MetaData[] = {
		{ "Category", "RunGame|Events" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// ---- \xe5\xa7\x94\xe6\x89\x98 ----\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameState.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "---- \xe5\xa7\x94\xe6\x89\x98 ----" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnCountdownUpdated_MetaData[] = {
		{ "Category", "RunGame|Events" },
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnCharacterDeath_MetaData[] = {
		{ "Category", "RunGame|Death" },
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentState_MetaData[] = {
		{ "Category", "RunGame|State" },
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CountdownSeconds_MetaData[] = {
		{ "Category", "RunGame|State" },
		{ "ModuleRelativePath", "RunGameGameState.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_DefaultCountdownSeconds;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DefaultGameTotalTime;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnGameStateChanged;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnCountdownUpdated;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnCharacterDeath;
	static const UECodeGen_Private::FBytePropertyParams NewProp_CurrentState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_CurrentState;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CountdownSeconds;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ARunGameGameState_GetCountdownSeconds, "GetCountdownSeconds" }, // 3144311212
		{ &Z_Construct_UFunction_ARunGameGameState_GetCurrentState, "GetCurrentState" }, // 2888377528
		{ &Z_Construct_UFunction_ARunGameGameState_SetCountdownSeconds, "SetCountdownSeconds" }, // 1038435954
		{ &Z_Construct_UFunction_ARunGameGameState_SetGameState, "SetGameState" }, // 3951321136
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGameGameState>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_DefaultCountdownSeconds = { "DefaultCountdownSeconds", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameState, DefaultCountdownSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultCountdownSeconds_MetaData), NewProp_DefaultCountdownSeconds_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_DefaultGameTotalTime = { "DefaultGameTotalTime", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameState, DefaultGameTotalTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultGameTotalTime_MetaData), NewProp_DefaultGameTotalTime_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_OnGameStateChanged = { "OnGameStateChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameState, OnGameStateChanged), Z_Construct_UDelegateFunction_RunGame_OnGameStateChangedSignature__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnGameStateChanged_MetaData), NewProp_OnGameStateChanged_MetaData) }; // 4116426122
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_OnCountdownUpdated = { "OnCountdownUpdated", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameState, OnCountdownUpdated), Z_Construct_UDelegateFunction_RunGame_OnCountdownUpdatedSignature__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnCountdownUpdated_MetaData), NewProp_OnCountdownUpdated_MetaData) }; // 115797559
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_OnCharacterDeath = { "OnCharacterDeath", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameState, OnCharacterDeath), Z_Construct_UDelegateFunction_RunGame_OnCharacterDeathDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnCharacterDeath_MetaData), NewProp_OnCharacterDeath_MetaData) }; // 3817926357
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_CurrentState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_CurrentState = { "CurrentState", nullptr, (EPropertyFlags)0x0040000000020001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameState, CurrentState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentState_MetaData), NewProp_CurrentState_MetaData) }; // 1491870806
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARunGameGameState_Statics::NewProp_CountdownSeconds = { "CountdownSeconds", nullptr, (EPropertyFlags)0x0040000000020001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameState, CountdownSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CountdownSeconds_MetaData), NewProp_CountdownSeconds_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGameGameState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_DefaultCountdownSeconds,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_DefaultGameTotalTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_OnGameStateChanged,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_OnCountdownUpdated,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_OnCharacterDeath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_CurrentState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_CurrentState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameState_Statics::NewProp_CountdownSeconds,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameState_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGameGameState_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameStateBase,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameState_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGameGameState_Statics::ClassParams = {
	&ARunGameGameState::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ARunGameGameState_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameState_Statics::PropPointers),
	0,
	0x009003A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameState_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGameGameState_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGameGameState()
{
	if (!Z_Registration_Info_UClass_ARunGameGameState.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGameGameState.OuterSingleton, Z_Construct_UClass_ARunGameGameState_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGameGameState.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGameGameState);
ARunGameGameState::~ARunGameGameState() {}
// ********** End Class ARunGameGameState **********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameState_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGameGameState, ARunGameGameState::StaticClass, TEXT("ARunGameGameState"), &Z_Registration_Info_UClass_ARunGameGameState, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGameGameState), 2146795763U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameState_h__Script_RunGame_2694528870(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameState_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameState_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
