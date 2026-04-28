// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "WorldSubsystem/RunGameTimerSubsystem.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameTimerSubsystem() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_UWorldSubsystem();
RUNGAME_API UClass* Z_Construct_UClass_URunGameTimerSubsystem();
RUNGAME_API UClass* Z_Construct_UClass_URunGameTimerSubsystem_NoRegister();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOnCountdownCompleteDelegate ******************************************
struct Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnCountdownCompleteDelegate__DelegateSignature", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnCountdownCompleteDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnCountdownCompleteDelegate)
{
	OnCountdownCompleteDelegate.ProcessMulticastDelegate<UObject>(NULL);
}
// ********** End Delegate FOnCountdownCompleteDelegate ********************************************

// ********** Begin Delegate FOnTimeChangedDelegate ************************************************
struct Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics
{
	struct _Script_RunGame_eventOnTimeChangedDelegate_Parms
	{
		float NewTime;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::NewProp_NewTime = { "NewTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnTimeChangedDelegate_Parms, NewTime), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::NewProp_NewTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnTimeChangedDelegate__DelegateSignature", Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::_Script_RunGame_eventOnTimeChangedDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::_Script_RunGame_eventOnTimeChangedDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnTimeChangedDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnTimeChangedDelegate, float NewTime)
{
	struct _Script_RunGame_eventOnTimeChangedDelegate_Parms
	{
		float NewTime;
	};
	_Script_RunGame_eventOnTimeChangedDelegate_Parms Parms;
	Parms.NewTime=NewTime;
	OnTimeChangedDelegate.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnTimeChangedDelegate **************************************************

// ********** Begin Class URunGameTimerSubsystem Function GetTotalTimeSeconds **********************
struct Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics
{
	struct RunGameTimerSubsystem_eventGetTotalTimeSeconds_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Timer" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameTimerSubsystem_eventGetTotalTimeSeconds_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameTimerSubsystem, nullptr, "GetTotalTimeSeconds", Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::RunGameTimerSubsystem_eventGetTotalTimeSeconds_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::RunGameTimerSubsystem_eventGetTotalTimeSeconds_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameTimerSubsystem::execGetTotalTimeSeconds)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->GetTotalTimeSeconds();
	P_NATIVE_END;
}
// ********** End Class URunGameTimerSubsystem Function GetTotalTimeSeconds ************************

// ********** Begin Class URunGameTimerSubsystem Function IsTimerRunning ***************************
struct Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics
{
	struct RunGameTimerSubsystem_eventIsTimerRunning_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Timer" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((RunGameTimerSubsystem_eventIsTimerRunning_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(RunGameTimerSubsystem_eventIsTimerRunning_Parms), &Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameTimerSubsystem, nullptr, "IsTimerRunning", Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::RunGameTimerSubsystem_eventIsTimerRunning_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::RunGameTimerSubsystem_eventIsTimerRunning_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameTimerSubsystem::execIsTimerRunning)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsTimerRunning();
	P_NATIVE_END;
}
// ********** End Class URunGameTimerSubsystem Function IsTimerRunning *****************************

// ********** Begin Class URunGameTimerSubsystem Function OnGameStateChangedCallback ***************
struct Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics
{
	struct RunGameTimerSubsystem_eventOnGameStateChangedCallback_Parms
	{
		ERunGameGameState OldState;
		ERunGameGameState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Reactively starts/stops timers based on the new game state */" },
#endif
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reactively starts/stops timers based on the new game state" },
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
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_OldState = { "OldState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameTimerSubsystem_eventOnGameStateChangedCallback_Parms, OldState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameTimerSubsystem_eventOnGameStateChangedCallback_Parms, NewState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_OldState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_URunGameTimerSubsystem, nullptr, "OnGameStateChangedCallback", Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::PropPointers), sizeof(Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::RunGameTimerSubsystem_eventOnGameStateChangedCallback_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::RunGameTimerSubsystem_eventOnGameStateChangedCallback_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(URunGameTimerSubsystem::execOnGameStateChangedCallback)
{
	P_GET_ENUM(ERunGameGameState,Z_Param_OldState);
	P_GET_ENUM(ERunGameGameState,Z_Param_NewState);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnGameStateChangedCallback(ERunGameGameState(Z_Param_OldState),ERunGameGameState(Z_Param_NewState));
	P_NATIVE_END;
}
// ********** End Class URunGameTimerSubsystem Function OnGameStateChangedCallback *****************

// ********** Begin Class URunGameTimerSubsystem ***************************************************
void URunGameTimerSubsystem::StaticRegisterNativesURunGameTimerSubsystem()
{
	UClass* Class = URunGameTimerSubsystem::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetTotalTimeSeconds", &URunGameTimerSubsystem::execGetTotalTimeSeconds },
		{ "IsTimerRunning", &URunGameTimerSubsystem::execIsTimerRunning },
		{ "OnGameStateChangedCallback", &URunGameTimerSubsystem::execOnGameStateChangedCallback },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_URunGameTimerSubsystem;
UClass* URunGameTimerSubsystem::GetPrivateStaticClass()
{
	using TClass = URunGameTimerSubsystem;
	if (!Z_Registration_Info_UClass_URunGameTimerSubsystem.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameTimerSubsystem"),
			Z_Registration_Info_UClass_URunGameTimerSubsystem.InnerSingleton,
			StaticRegisterNativesURunGameTimerSubsystem,
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
	return Z_Registration_Info_UClass_URunGameTimerSubsystem.InnerSingleton;
}
UClass* Z_Construct_UClass_URunGameTimerSubsystem_NoRegister()
{
	return URunGameTimerSubsystem::GetPrivateStaticClass();
}
struct Z_Construct_UClass_URunGameTimerSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xe7\xba\xaf\xe7\xb2\xb9\xe7\x9a\x84\xe8\xae\xa1\xe6\x97\xb6\xe5\xad\x90\xe7\xb3\xbb\xe7\xbb\x9f\n * \xe5\x80\x92\xe8\xae\xa1\xe6\x97\xb6\xef\xbc\x9a\xe4\xbb\x8e DefaultCountdownSeconds \xe5\x80\x92\xe6\x95\xb0\xe8\x87\xb3 0\xef\xbc\x8c\xe6\xaf\x8f\xe7\xa7\x92 Tick \xe4\xb8\x80\xe6\xac\xa1\xe3\x80\x82\n * \xe6\xad\xa3\xe5\x90\x91\xe8\xae\xa1\xe6\x97\xb6\xef\xbc\x9a\xe4\xbb\x8e 0.0 \xe5\x90\x91\xe4\xb8\x8a\xe7\xb4\xaf\xe8\xae\xa1\xef\xbc\x8c\xe6\xaf\x8f\xe5\xb8\xa7 Tick\xe3\x80\x82\n * \xe4\xb8\x8d\xe6\x8c\x81\xe6\x9c\x89\xe9\x85\x8d\xe7\xbd\xae\xe6\x95\xb0\xe6\x8d\xae\xe6\x88\x96\xe5\x88\x86\xe6\x95\xb0\xe9\x80\xbb\xe8\xbe\x91\xef\xbc\x8c\xe5\x90\xaf\xe5\x81\x9c\xe5\xae\x8c\xe5\x85\xa8\xe7\x94\xb1 GameState \xe7\x8a\xb6\xe6\x80\x81\xe6\x9c\xba\xe9\xa9\xb1\xe5\x8a\xa8\xe3\x80\x82\n */" },
#endif
		{ "IncludePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xba\xaf\xe7\xb2\xb9\xe7\x9a\x84\xe8\xae\xa1\xe6\x97\xb6\xe5\xad\x90\xe7\xb3\xbb\xe7\xbb\x9f\n\xe5\x80\x92\xe8\xae\xa1\xe6\x97\xb6\xef\xbc\x9a\xe4\xbb\x8e DefaultCountdownSeconds \xe5\x80\x92\xe6\x95\xb0\xe8\x87\xb3 0\xef\xbc\x8c\xe6\xaf\x8f\xe7\xa7\x92 Tick \xe4\xb8\x80\xe6\xac\xa1\xe3\x80\x82\n\xe6\xad\xa3\xe5\x90\x91\xe8\xae\xa1\xe6\x97\xb6\xef\xbc\x9a\xe4\xbb\x8e 0.0 \xe5\x90\x91\xe4\xb8\x8a\xe7\xb4\xaf\xe8\xae\xa1\xef\xbc\x8c\xe6\xaf\x8f\xe5\xb8\xa7 Tick\xe3\x80\x82\n\xe4\xb8\x8d\xe6\x8c\x81\xe6\x9c\x89\xe9\x85\x8d\xe7\xbd\xae\xe6\x95\xb0\xe6\x8d\xae\xe6\x88\x96\xe5\x88\x86\xe6\x95\xb0\xe9\x80\xbb\xe8\xbe\x91\xef\xbc\x8c\xe5\x90\xaf\xe5\x81\x9c\xe5\xae\x8c\xe5\x85\xa8\xe7\x94\xb1 GameState \xe7\x8a\xb6\xe6\x80\x81\xe6\x9c\xba\xe9\xa9\xb1\xe5\x8a\xa8\xe3\x80\x82" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnCountdownComplete_MetaData[] = {
		{ "Category", "RunGame|Timer" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnTimeChanged_MetaData[] = {
		{ "Category", "RunGame|Timer" },
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TotalTimeSeconds_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "RunGame|Timer" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Accumulated forward time in seconds, incremented each tick */" },
#endif
		{ "ModuleRelativePath", "WorldSubsystem/RunGameTimerSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Accumulated forward time in seconds, incremented each tick" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnCountdownComplete;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnTimeChanged;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_TotalTimeSeconds;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_URunGameTimerSubsystem_GetTotalTimeSeconds, "GetTotalTimeSeconds" }, // 1648838463
		{ &Z_Construct_UFunction_URunGameTimerSubsystem_IsTimerRunning, "IsTimerRunning" }, // 1496671197
		{ &Z_Construct_UFunction_URunGameTimerSubsystem_OnGameStateChangedCallback, "OnGameStateChangedCallback" }, // 4071531946
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URunGameTimerSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_URunGameTimerSubsystem_Statics::NewProp_OnCountdownComplete = { "OnCountdownComplete", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameTimerSubsystem, OnCountdownComplete), Z_Construct_UDelegateFunction_RunGame_OnCountdownCompleteDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnCountdownComplete_MetaData), NewProp_OnCountdownComplete_MetaData) }; // 2599232798
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_URunGameTimerSubsystem_Statics::NewProp_OnTimeChanged = { "OnTimeChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameTimerSubsystem, OnTimeChanged), Z_Construct_UDelegateFunction_RunGame_OnTimeChangedDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnTimeChanged_MetaData), NewProp_OnTimeChanged_MetaData) }; // 3557279904
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_URunGameTimerSubsystem_Statics::NewProp_TotalTimeSeconds = { "TotalTimeSeconds", nullptr, (EPropertyFlags)0x0040000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(URunGameTimerSubsystem, TotalTimeSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TotalTimeSeconds_MetaData), NewProp_TotalTimeSeconds_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URunGameTimerSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameTimerSubsystem_Statics::NewProp_OnCountdownComplete,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameTimerSubsystem_Statics::NewProp_OnTimeChanged,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URunGameTimerSubsystem_Statics::NewProp_TotalTimeSeconds,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameTimerSubsystem_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_URunGameTimerSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWorldSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameTimerSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_URunGameTimerSubsystem_Statics::ClassParams = {
	&URunGameTimerSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_URunGameTimerSubsystem_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_URunGameTimerSubsystem_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_URunGameTimerSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_URunGameTimerSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_URunGameTimerSubsystem()
{
	if (!Z_Registration_Info_UClass_URunGameTimerSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URunGameTimerSubsystem.OuterSingleton, Z_Construct_UClass_URunGameTimerSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_URunGameTimerSubsystem.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(URunGameTimerSubsystem);
URunGameTimerSubsystem::~URunGameTimerSubsystem() {}
// ********** End Class URunGameTimerSubsystem *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_URunGameTimerSubsystem, URunGameTimerSubsystem::StaticClass, TEXT("URunGameTimerSubsystem"), &Z_Registration_Info_UClass_URunGameTimerSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URunGameTimerSubsystem), 3114621966U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h__Script_RunGame_671310214(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_WorldSubsystem_RunGameTimerSubsystem_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
