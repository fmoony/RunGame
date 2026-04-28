// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RunGameGameMode.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameGameMode() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
ENGINE_API UClass* Z_Construct_UClass_APawn_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameCharacter_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameDeathVolume_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameMode();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameGameMode_NoRegister();
RUNGAME_API UFunction* Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOnPlayerDeathDelegate ************************************************
struct Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics
{
	struct _Script_RunGame_eventOnPlayerDeathDelegate_Parms
	{
		ARunGameCharacter* PlayerCharacter;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "RunGameGameMode.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerCharacter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::NewProp_PlayerCharacter = { "PlayerCharacter", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_RunGame_eventOnPlayerDeathDelegate_Parms, PlayerCharacter), Z_Construct_UClass_ARunGameCharacter_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::NewProp_PlayerCharacter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_RunGame, nullptr, "OnPlayerDeathDelegate__DelegateSignature", Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::_Script_RunGame_eventOnPlayerDeathDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::_Script_RunGame_eventOnPlayerDeathDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnPlayerDeathDelegate_DelegateWrapper(const FMulticastScriptDelegate& OnPlayerDeathDelegate, ARunGameCharacter* PlayerCharacter)
{
	struct _Script_RunGame_eventOnPlayerDeathDelegate_Parms
	{
		ARunGameCharacter* PlayerCharacter;
	};
	_Script_RunGame_eventOnPlayerDeathDelegate_Parms Parms;
	Parms.PlayerCharacter=PlayerCharacter;
	OnPlayerDeathDelegate.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnPlayerDeathDelegate **************************************************

// ********** Begin Class ARunGameGameMode Function HandlePlayerDeath ******************************
struct Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics
{
	struct RunGameGameMode_eventHandlePlayerDeath_Parms
	{
		ARunGameDeathVolume* DeathVolume;
		ARunGameCharacter* PlayerCharacter;
		bool bImmediate;
		float Delay;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Death" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Processes player death -- broadcasts event, sets GameOver state, handles character destruction */// Input mode and view target handled by Character::Die()\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Processes player death -- broadcasts event, sets GameOver state, handles character destruction // Input mode and view target handled by Character::Die()" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DeathVolume;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerCharacter;
	static void NewProp_bImmediate_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediate;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Delay;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_DeathVolume = { "DeathVolume", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameMode_eventHandlePlayerDeath_Parms, DeathVolume), Z_Construct_UClass_ARunGameDeathVolume_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_PlayerCharacter = { "PlayerCharacter", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameMode_eventHandlePlayerDeath_Parms, PlayerCharacter), Z_Construct_UClass_ARunGameCharacter_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_bImmediate_SetBit(void* Obj)
{
	((RunGameGameMode_eventHandlePlayerDeath_Parms*)Obj)->bImmediate = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_bImmediate = { "bImmediate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(RunGameGameMode_eventHandlePlayerDeath_Parms), &Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_bImmediate_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_Delay = { "Delay", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameMode_eventHandlePlayerDeath_Parms, Delay), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_DeathVolume,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_PlayerCharacter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_bImmediate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::NewProp_Delay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameMode, nullptr, "HandlePlayerDeath", Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::RunGameGameMode_eventHandlePlayerDeath_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::RunGameGameMode_eventHandlePlayerDeath_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameMode::execHandlePlayerDeath)
{
	P_GET_OBJECT(ARunGameDeathVolume,Z_Param_DeathVolume);
	P_GET_OBJECT(ARunGameCharacter,Z_Param_PlayerCharacter);
	P_GET_UBOOL(Z_Param_bImmediate);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Delay);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandlePlayerDeath(Z_Param_DeathVolume,Z_Param_PlayerCharacter,Z_Param_bImmediate,Z_Param_Delay);
	P_NATIVE_END;
}
// ********** End Class ARunGameGameMode Function HandlePlayerDeath ********************************

// ********** Begin Class ARunGameGameMode Function InitializeGameTimer ****************************
struct Z_Construct_UFunction_ARunGameGameMode_InitializeGameTimer_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Game" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Deprecated -- timer now starts reactively via GameState state changes */" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Deprecated -- timer now starts reactively via GameState state changes" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameMode_InitializeGameTimer_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameMode, nullptr, "InitializeGameTimer", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_InitializeGameTimer_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameMode_InitializeGameTimer_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameGameMode_InitializeGameTimer()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameMode_InitializeGameTimer_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameMode::execInitializeGameTimer)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->InitializeGameTimer();
	P_NATIVE_END;
}
// ********** End Class ARunGameGameMode Function InitializeGameTimer ******************************

// ********** Begin Class ARunGameGameMode Function OnFloorSystemReadyCallback *********************
struct Z_Construct_UFunction_ARunGameGameMode_OnFloorSystemReadyCallback_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Callback when floor subsystem finishes async loading -- spawns initial floors */" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Callback when floor subsystem finishes async loading -- spawns initial floors" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameMode_OnFloorSystemReadyCallback_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameMode, nullptr, "OnFloorSystemReadyCallback", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_OnFloorSystemReadyCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameMode_OnFloorSystemReadyCallback_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameGameMode_OnFloorSystemReadyCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameMode_OnFloorSystemReadyCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameMode::execOnFloorSystemReadyCallback)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnFloorSystemReadyCallback();
	P_NATIVE_END;
}
// ********** End Class ARunGameGameMode Function OnFloorSystemReadyCallback ***********************

// ********** Begin Class ARunGameGameMode Function ResetGame **************************************
struct Z_Construct_UFunction_ARunGameGameMode_ResetGame_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Game" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Fully resets the game to initial state and returns to main menu */" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Fully resets the game to initial state and returns to main menu" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameMode_ResetGame_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameMode, nullptr, "ResetGame", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_ResetGame_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameMode_ResetGame_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameGameMode_ResetGame()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameMode_ResetGame_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameMode::execResetGame)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ResetGame();
	P_NATIVE_END;
}
// ********** End Class ARunGameGameMode Function ResetGame ****************************************

// ********** Begin Class ARunGameGameMode Function SpawnPlayer ************************************
struct Z_Construct_UFunction_ARunGameGameMode_SpawnPlayer_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Spawns and possesses the player character at the chosen player start */" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Spawns and possesses the player character at the chosen player start" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameMode_SpawnPlayer_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameMode, nullptr, "SpawnPlayer", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_SpawnPlayer_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameMode_SpawnPlayer_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameGameMode_SpawnPlayer()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameMode_SpawnPlayer_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameMode::execSpawnPlayer)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SpawnPlayer();
	P_NATIVE_END;
}
// ********** End Class ARunGameGameMode Function SpawnPlayer **************************************

// ********** Begin Class ARunGameGameMode Function StartGameCountDown *****************************
struct Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics
{
	struct RunGameGameMode_eventStartGameCountDown_Parms
	{
		int32 CountdownSeconds;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Flow" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Initiates the game countdown sequence and sets game state to CountDown */" },
#endif
		{ "CPP_Default_CountdownSeconds", "3" },
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Initiates the game countdown sequence and sets game state to CountDown" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_CountdownSeconds;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::NewProp_CountdownSeconds = { "CountdownSeconds", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameGameMode_eventStartGameCountDown_Parms, CountdownSeconds), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::NewProp_CountdownSeconds,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameMode, nullptr, "StartGameCountDown", Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::RunGameGameMode_eventStartGameCountDown_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::RunGameGameMode_eventStartGameCountDown_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameMode::execStartGameCountDown)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_CountdownSeconds);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StartGameCountDown(Z_Param_CountdownSeconds);
	P_NATIVE_END;
}
// ********** End Class ARunGameGameMode Function StartGameCountDown *******************************

// ********** Begin Class ARunGameGameMode Function StartNewGame ***********************************
struct Z_Construct_UFunction_ARunGameGameMode_StartNewGame_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Game" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Restarts the game directly, bypassing the main menu */" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Restarts the game directly, bypassing the main menu" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameGameMode_StartNewGame_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameGameMode, nullptr, "StartNewGame", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameGameMode_StartNewGame_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameGameMode_StartNewGame_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameGameMode_StartNewGame()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameGameMode_StartNewGame_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameGameMode::execStartNewGame)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StartNewGame();
	P_NATIVE_END;
}
// ********** End Class ARunGameGameMode Function StartNewGame *************************************

// ********** Begin Class ARunGameGameMode *********************************************************
void ARunGameGameMode::StaticRegisterNativesARunGameGameMode()
{
	UClass* Class = ARunGameGameMode::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "HandlePlayerDeath", &ARunGameGameMode::execHandlePlayerDeath },
		{ "InitializeGameTimer", &ARunGameGameMode::execInitializeGameTimer },
		{ "OnFloorSystemReadyCallback", &ARunGameGameMode::execOnFloorSystemReadyCallback },
		{ "ResetGame", &ARunGameGameMode::execResetGame },
		{ "SpawnPlayer", &ARunGameGameMode::execSpawnPlayer },
		{ "StartGameCountDown", &ARunGameGameMode::execStartGameCountDown },
		{ "StartNewGame", &ARunGameGameMode::execStartNewGame },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGameGameMode;
UClass* ARunGameGameMode::GetPrivateStaticClass()
{
	using TClass = ARunGameGameMode;
	if (!Z_Registration_Info_UClass_ARunGameGameMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameGameMode"),
			Z_Registration_Info_UClass_ARunGameGameMode.InnerSingleton,
			StaticRegisterNativesARunGameGameMode,
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
	return Z_Registration_Info_UClass_ARunGameGameMode.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGameGameMode_NoRegister()
{
	return ARunGameGameMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGameGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  Simple GameMode for a third person game\n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "RunGameGameMode.h" },
		{ "ModuleRelativePath", "RunGameGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Simple GameMode for a third person game" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GameCharacterClass_MetaData[] = {
		{ "Category", "Classes" },
		{ "ModuleRelativePath", "RunGameGameMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StraightFloorClasses_MetaData[] = {
		{ "Category", "RunGame|FloorSystem" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\x9c\xb0\xe6\x9d\xbf\xe8\x93\x9d\xe5\x9b\xbe\xe7\xb1\xbb\xe9\x85\x8d\xe7\xbd\xae\xef\xbc\x88\xe7\x94\xa8\xe4\xba\x8e\xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96 FloorSubsystem\xef\xbc\x89\n" },
#endif
		{ "DisplayName", "Straight Floor Classes" },
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\x9c\xb0\xe6\x9d\xbf\xe8\x93\x9d\xe5\x9b\xbe\xe7\xb1\xbb\xe9\x85\x8d\xe7\xbd\xae\xef\xbc\x88\xe7\x94\xa8\xe4\xba\x8e\xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96 FloorSubsystem\xef\xbc\x89" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TurnFloorClasses_MetaData[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "DisplayName", "Turn Floor Classes" },
		{ "ModuleRelativePath", "RunGameGameMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PreAllocateFloorCount_MetaData[] = {
		{ "Category", "RunGame|FloorSystem" },
		{ "DisplayName", "Pre-Allocate Count" },
		{ "ModuleRelativePath", "RunGameGameMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GameTotalTime_MetaData[] = {
		{ "Category", "RunGame|Game" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe6\xb8\xb8\xe6\x88\x8f\xe5\x88\x9d\xe5\xa7\x8b\xe6\x97\xb6\xe9\x97\xb4\xe8\xae\xbe\xe7\xbd\xae\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xb8\xb8\xe6\x88\x8f\xe5\x88\x9d\xe5\xa7\x8b\xe6\x97\xb6\xe9\x97\xb4\xe8\xae\xbe\xe7\xbd\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnPlayerDeath_MetaData[] = {
		{ "Category", "RunGame|Death" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x8e\xa9\xe5\xae\xb6\xe6\xad\xbb\xe4\xba\xa1\xe7\x9b\xb8\xe5\x85\xb3\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x8e\xa9\xe5\xae\xb6\xe6\xad\xbb\xe4\xba\xa1\xe7\x9b\xb8\xe5\x85\xb3" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PlayerDeathDelay_MetaData[] = {
		{ "Category", "RunGame|Death" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe7\x8e\xa9\xe5\xae\xb6\xe6\xad\xbb\xe4\xba\xa1\xe5\xbb\xb6\xe8\xbf\x9f\xe5\xa4\x84\xe7\x90\x86\n" },
#endif
		{ "ModuleRelativePath", "RunGameGameMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\x8e\xa9\xe5\xae\xb6\xe6\xad\xbb\xe4\xba\xa1\xe5\xbb\xb6\xe8\xbf\x9f\xe5\xa4\x84\xe7\x90\x86" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_GameCharacterClass;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_StraightFloorClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_StraightFloorClasses;
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_TurnFloorClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TurnFloorClasses;
	static const UECodeGen_Private::FIntPropertyParams NewProp_PreAllocateFloorCount;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GameTotalTime;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnPlayerDeath;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PlayerDeathDelay;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ARunGameGameMode_HandlePlayerDeath, "HandlePlayerDeath" }, // 4107966430
		{ &Z_Construct_UFunction_ARunGameGameMode_InitializeGameTimer, "InitializeGameTimer" }, // 3209263031
		{ &Z_Construct_UFunction_ARunGameGameMode_OnFloorSystemReadyCallback, "OnFloorSystemReadyCallback" }, // 2458518703
		{ &Z_Construct_UFunction_ARunGameGameMode_ResetGame, "ResetGame" }, // 3966647415
		{ &Z_Construct_UFunction_ARunGameGameMode_SpawnPlayer, "SpawnPlayer" }, // 3221122360
		{ &Z_Construct_UFunction_ARunGameGameMode_StartGameCountDown, "StartGameCountDown" }, // 307963375
		{ &Z_Construct_UFunction_ARunGameGameMode_StartNewGame, "StartNewGame" }, // 2086802522
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGameGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_GameCharacterClass = { "GameCharacterClass", nullptr, (EPropertyFlags)0x0014000000010001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameMode, GameCharacterClass), Z_Construct_UClass_UClass, Z_Construct_UClass_APawn_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GameCharacterClass_MetaData), NewProp_GameCharacterClass_MetaData) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_StraightFloorClasses_Inner = { "StraightFloorClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_StraightFloorClasses = { "StraightFloorClasses", nullptr, (EPropertyFlags)0x0014000000010001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameMode, StraightFloorClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StraightFloorClasses_MetaData), NewProp_StraightFloorClasses_MetaData) };
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_TurnFloorClasses_Inner = { "TurnFloorClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_TurnFloorClasses = { "TurnFloorClasses", nullptr, (EPropertyFlags)0x0014000000010001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameMode, TurnFloorClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TurnFloorClasses_MetaData), NewProp_TurnFloorClasses_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_PreAllocateFloorCount = { "PreAllocateFloorCount", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameMode, PreAllocateFloorCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PreAllocateFloorCount_MetaData), NewProp_PreAllocateFloorCount_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_GameTotalTime = { "GameTotalTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameMode, GameTotalTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GameTotalTime_MetaData), NewProp_GameTotalTime_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_OnPlayerDeath = { "OnPlayerDeath", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameMode, OnPlayerDeath), Z_Construct_UDelegateFunction_RunGame_OnPlayerDeathDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnPlayerDeath_MetaData), NewProp_OnPlayerDeath_MetaData) }; // 1410918457
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_PlayerDeathDelay = { "PlayerDeathDelay", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameGameMode, PlayerDeathDelay), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PlayerDeathDelay_MetaData), NewProp_PlayerDeathDelay_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGameGameMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_GameCharacterClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_StraightFloorClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_StraightFloorClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_TurnFloorClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_TurnFloorClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_PreAllocateFloorCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_GameTotalTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_OnPlayerDeath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameGameMode_Statics::NewProp_PlayerDeathDelay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameMode_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGameGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGameGameMode_Statics::ClassParams = {
	&ARunGameGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ARunGameGameMode_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameMode_Statics::PropPointers),
	0,
	0x008003ADu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGameGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGameGameMode()
{
	if (!Z_Registration_Info_UClass_ARunGameGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGameGameMode.OuterSingleton, Z_Construct_UClass_ARunGameGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGameGameMode.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGameGameMode);
ARunGameGameMode::~ARunGameGameMode() {}
// ********** End Class ARunGameGameMode ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameMode_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGameGameMode, ARunGameGameMode::StaticClass, TEXT("ARunGameGameMode"), &Z_Registration_Info_UClass_ARunGameGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGameGameMode), 319562006U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameMode_h__Script_RunGame_1241766356(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameMode_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameGameMode_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
