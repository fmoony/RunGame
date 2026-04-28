// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RunGameCharacter.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameCharacter() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_ACharacter();
ENGINE_API UClass* Z_Construct_UClass_UAnimMontage_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USpringArmComponent_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameCharacter();
RUNGAME_API UClass* Z_Construct_UClass_ARunGameCharacter_NoRegister();
RUNGAME_API UClass* Z_Construct_UClass_URunGameTimerSubsystem_NoRegister();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ARunGameCharacter Function Die *******************************************
struct Z_Construct_UFunction_ARunGameCharacter_Die_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "RunGame|Death" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Character death cleanup \xe2\x80\x94 detaches camera, disables input/collision */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Character death cleanup \xe2\x80\x94 detaches camera, disables input/collision" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameCharacter_Die_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameCharacter, nullptr, "Die", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_Die_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameCharacter_Die_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameCharacter_Die()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameCharacter_Die_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameCharacter::execDie)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Die();
	P_NATIVE_END;
}
// ********** End Class ARunGameCharacter Function Die *********************************************

// ********** Begin Class ARunGameCharacter Function DoJumpEnd *************************************
struct Z_Construct_UFunction_ARunGameCharacter_DoJumpEnd_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Handles jump pressed inputs from either controls or UI interfaces */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Handles jump pressed inputs from either controls or UI interfaces" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameCharacter_DoJumpEnd_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameCharacter, nullptr, "DoJumpEnd", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoJumpEnd_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameCharacter_DoJumpEnd_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameCharacter_DoJumpEnd()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameCharacter_DoJumpEnd_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameCharacter::execDoJumpEnd)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->DoJumpEnd();
	P_NATIVE_END;
}
// ********** End Class ARunGameCharacter Function DoJumpEnd ***************************************

// ********** Begin Class ARunGameCharacter Function DoJumpStart ***********************************
struct Z_Construct_UFunction_ARunGameCharacter_DoJumpStart_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Handles jump pressed inputs from either controls or UI interfaces */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Handles jump pressed inputs from either controls or UI interfaces" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameCharacter_DoJumpStart_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameCharacter, nullptr, "DoJumpStart", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoJumpStart_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameCharacter_DoJumpStart_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_ARunGameCharacter_DoJumpStart()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameCharacter_DoJumpStart_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameCharacter::execDoJumpStart)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->DoJumpStart();
	P_NATIVE_END;
}
// ********** End Class ARunGameCharacter Function DoJumpStart *************************************

// ********** Begin Class ARunGameCharacter Function DoLook ****************************************
struct Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics
{
	struct RunGameCharacter_eventDoLook_Parms
	{
		float Yaw;
		float Pitch;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Handles look inputs from either controls or UI interfaces */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Handles look inputs from either controls or UI interfaces" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Yaw;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Pitch;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::NewProp_Yaw = { "Yaw", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCharacter_eventDoLook_Parms, Yaw), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::NewProp_Pitch = { "Pitch", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCharacter_eventDoLook_Parms, Pitch), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::NewProp_Yaw,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::NewProp_Pitch,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameCharacter, nullptr, "DoLook", Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::RunGameCharacter_eventDoLook_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::RunGameCharacter_eventDoLook_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameCharacter_DoLook()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameCharacter_DoLook_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameCharacter::execDoLook)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_Yaw);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Pitch);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->DoLook(Z_Param_Yaw,Z_Param_Pitch);
	P_NATIVE_END;
}
// ********** End Class ARunGameCharacter Function DoLook ******************************************

// ********** Begin Class ARunGameCharacter Function DoMove ****************************************
struct Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics
{
	struct RunGameCharacter_eventDoMove_Parms
	{
		float Right;
		float Forward;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Handles move inputs from either controls or UI interfaces */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Handles move inputs from either controls or UI interfaces" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Right;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Forward;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::NewProp_Right = { "Right", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCharacter_eventDoMove_Parms, Right), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::NewProp_Forward = { "Forward", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCharacter_eventDoMove_Parms, Forward), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::NewProp_Right,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::NewProp_Forward,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameCharacter, nullptr, "DoMove", Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::RunGameCharacter_eventDoMove_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::RunGameCharacter_eventDoMove_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameCharacter_DoMove()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameCharacter_DoMove_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameCharacter::execDoMove)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_Right);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Forward);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->DoMove(Z_Param_Right,Z_Param_Forward);
	P_NATIVE_END;
}
// ********** End Class ARunGameCharacter Function DoMove ******************************************

// ********** Begin Class ARunGameCharacter Function OnGameStateChangedCallback ********************
struct Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics
{
	struct RunGameCharacter_eventOnGameStateChangedCallback_Parms
	{
		ERunGameGameState OldState;
		ERunGameGameState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Reacts to game state changes -- self-destructs when returning to MainMenu */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reacts to game state changes -- self-destructs when returning to MainMenu" },
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
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_OldState = { "OldState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCharacter_eventOnGameStateChangedCallback_Parms, OldState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCharacter_eventOnGameStateChangedCallback_Parms, NewState), Z_Construct_UEnum_RunGame_ERunGameGameState, METADATA_PARAMS(0, nullptr) }; // 1491870806
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_OldState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_OldState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameCharacter, nullptr, "OnGameStateChangedCallback", Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::RunGameCharacter_eventOnGameStateChangedCallback_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::RunGameCharacter_eventOnGameStateChangedCallback_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameCharacter::execOnGameStateChangedCallback)
{
	P_GET_ENUM(ERunGameGameState,Z_Param_OldState);
	P_GET_ENUM(ERunGameGameState,Z_Param_NewState);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnGameStateChangedCallback(ERunGameGameState(Z_Param_OldState),ERunGameGameState(Z_Param_NewState));
	P_NATIVE_END;
}
// ********** End Class ARunGameCharacter Function OnGameStateChangedCallback **********************

// ********** Begin Class ARunGameCharacter Function OnSlideBlendingOut ****************************
struct Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics
{
	struct RunGameCharacter_eventOnSlideBlendingOut_Parms
	{
		UAnimMontage* Montage;
		bool bInterrupted;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Callback when slide montage finishes blending out -- triggers EndSlide cleanup */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Callback when slide montage finishes blending out -- triggers EndSlide cleanup" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Montage;
	static void NewProp_bInterrupted_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bInterrupted;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::NewProp_Montage = { "Montage", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(RunGameCharacter_eventOnSlideBlendingOut_Parms, Montage), Z_Construct_UClass_UAnimMontage_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::NewProp_bInterrupted_SetBit(void* Obj)
{
	((RunGameCharacter_eventOnSlideBlendingOut_Parms*)Obj)->bInterrupted = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::NewProp_bInterrupted = { "bInterrupted", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(RunGameCharacter_eventOnSlideBlendingOut_Parms), &Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::NewProp_bInterrupted_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::NewProp_Montage,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::NewProp_bInterrupted,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_ARunGameCharacter, nullptr, "OnSlideBlendingOut", Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::PropPointers), sizeof(Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::RunGameCharacter_eventOnSlideBlendingOut_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::Function_MetaDataParams), Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::RunGameCharacter_eventOnSlideBlendingOut_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ARunGameCharacter::execOnSlideBlendingOut)
{
	P_GET_OBJECT(UAnimMontage,Z_Param_Montage);
	P_GET_UBOOL(Z_Param_bInterrupted);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSlideBlendingOut(Z_Param_Montage,Z_Param_bInterrupted);
	P_NATIVE_END;
}
// ********** End Class ARunGameCharacter Function OnSlideBlendingOut ******************************

// ********** Begin Class ARunGameCharacter ********************************************************
void ARunGameCharacter::StaticRegisterNativesARunGameCharacter()
{
	UClass* Class = ARunGameCharacter::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "Die", &ARunGameCharacter::execDie },
		{ "DoJumpEnd", &ARunGameCharacter::execDoJumpEnd },
		{ "DoJumpStart", &ARunGameCharacter::execDoJumpStart },
		{ "DoLook", &ARunGameCharacter::execDoLook },
		{ "DoMove", &ARunGameCharacter::execDoMove },
		{ "OnGameStateChangedCallback", &ARunGameCharacter::execOnGameStateChangedCallback },
		{ "OnSlideBlendingOut", &ARunGameCharacter::execOnSlideBlendingOut },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_ARunGameCharacter;
UClass* ARunGameCharacter::GetPrivateStaticClass()
{
	using TClass = ARunGameCharacter;
	if (!Z_Registration_Info_UClass_ARunGameCharacter.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("RunGameCharacter"),
			Z_Registration_Info_UClass_ARunGameCharacter.InnerSingleton,
			StaticRegisterNativesARunGameCharacter,
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
	return Z_Registration_Info_UClass_ARunGameCharacter.InnerSingleton;
}
UClass* Z_Construct_UClass_ARunGameCharacter_NoRegister()
{
	return ARunGameCharacter::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ARunGameCharacter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  A simple player-controllable third person character\n *  Implements a controllable orbiting camera\n */" },
#endif
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "RunGameCharacter.h" },
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "A simple player-controllable third person character\nImplements a controllable orbiting camera" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CameraBoom_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Components" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Camera boom positioning the camera behind the character */" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Camera boom positioning the camera behind the character" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FollowCamera_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Components" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Follow camera */" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Follow camera" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_JumpAction_MetaData[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Jump Input Action */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Jump Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MoveAction_MetaData[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Move Input Action */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Move Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SlideAction_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "RunGameCharacter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LookAction_MetaData[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Look Input Action */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Look Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MouseLookAction_MetaData[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Mouse Look Input Action */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Mouse Look Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SlideMontage_MetaData[] = {
		{ "Category", "RunGame|Movement" },
		{ "ModuleRelativePath", "RunGameCharacter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RootMotionScale_MetaData[] = {
		{ "Category", "RunGame|Movement" },
		{ "ModuleRelativePath", "RunGameCharacter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MontagePlayRate_MetaData[] = {
		{ "Category", "RunGame|Movement" },
		{ "ModuleRelativePath", "RunGameCharacter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxSpeedCurve_MetaData[] = {
		{ "Category", "RunGame|Movement" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Float curve that maps elapsed game time to max walk speed */" },
#endif
		{ "ModuleRelativePath", "RunGameCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Float curve that maps elapsed game time to max walk speed" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimerSubsystem_MetaData[] = {
		{ "ModuleRelativePath", "RunGameCharacter.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraBoom;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FollowCamera;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_JumpAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MoveAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SlideAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_LookAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MouseLookAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SlideMontage;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RootMotionScale;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MontagePlayRate;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MaxSpeedCurve;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TimerSubsystem;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ARunGameCharacter_Die, "Die" }, // 530900464
		{ &Z_Construct_UFunction_ARunGameCharacter_DoJumpEnd, "DoJumpEnd" }, // 3942027537
		{ &Z_Construct_UFunction_ARunGameCharacter_DoJumpStart, "DoJumpStart" }, // 2471751175
		{ &Z_Construct_UFunction_ARunGameCharacter_DoLook, "DoLook" }, // 1301823953
		{ &Z_Construct_UFunction_ARunGameCharacter_DoMove, "DoMove" }, // 264681967
		{ &Z_Construct_UFunction_ARunGameCharacter_OnGameStateChangedCallback, "OnGameStateChangedCallback" }, // 950788639
		{ &Z_Construct_UFunction_ARunGameCharacter_OnSlideBlendingOut, "OnSlideBlendingOut" }, // 401145059
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARunGameCharacter>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_CameraBoom = { "CameraBoom", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, CameraBoom), Z_Construct_UClass_USpringArmComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CameraBoom_MetaData), NewProp_CameraBoom_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_FollowCamera = { "FollowCamera", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, FollowCamera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FollowCamera_MetaData), NewProp_FollowCamera_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_JumpAction = { "JumpAction", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, JumpAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_JumpAction_MetaData), NewProp_JumpAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MoveAction = { "MoveAction", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, MoveAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MoveAction_MetaData), NewProp_MoveAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_SlideAction = { "SlideAction", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, SlideAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SlideAction_MetaData), NewProp_SlideAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_LookAction = { "LookAction", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, LookAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LookAction_MetaData), NewProp_LookAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MouseLookAction = { "MouseLookAction", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, MouseLookAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MouseLookAction_MetaData), NewProp_MouseLookAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_SlideMontage = { "SlideMontage", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, SlideMontage), Z_Construct_UClass_UAnimMontage_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SlideMontage_MetaData), NewProp_SlideMontage_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_RootMotionScale = { "RootMotionScale", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, RootMotionScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RootMotionScale_MetaData), NewProp_RootMotionScale_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MontagePlayRate = { "MontagePlayRate", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, MontagePlayRate), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MontagePlayRate_MetaData), NewProp_MontagePlayRate_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MaxSpeedCurve = { "MaxSpeedCurve", nullptr, (EPropertyFlags)0x0114000000010001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, MaxSpeedCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxSpeedCurve_MetaData), NewProp_MaxSpeedCurve_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_TimerSubsystem = { "TimerSubsystem", nullptr, (EPropertyFlags)0x0114000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ARunGameCharacter, TimerSubsystem), Z_Construct_UClass_URunGameTimerSubsystem_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimerSubsystem_MetaData), NewProp_TimerSubsystem_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARunGameCharacter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_CameraBoom,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_FollowCamera,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_JumpAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MoveAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_SlideAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_LookAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MouseLookAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_SlideMontage,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_RootMotionScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MontagePlayRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_MaxSpeedCurve,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARunGameCharacter_Statics::NewProp_TimerSubsystem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameCharacter_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ARunGameCharacter_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACharacter,
	(UObject* (*)())Z_Construct_UPackage__Script_RunGame,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameCharacter_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ARunGameCharacter_Statics::ClassParams = {
	&ARunGameCharacter::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ARunGameCharacter_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameCharacter_Statics::PropPointers),
	0,
	0x008001A5u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ARunGameCharacter_Statics::Class_MetaDataParams), Z_Construct_UClass_ARunGameCharacter_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ARunGameCharacter()
{
	if (!Z_Registration_Info_UClass_ARunGameCharacter.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARunGameCharacter.OuterSingleton, Z_Construct_UClass_ARunGameCharacter_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ARunGameCharacter.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ARunGameCharacter);
ARunGameCharacter::~ARunGameCharacter() {}
// ********** End Class ARunGameCharacter **********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameCharacter_h__Script_RunGame_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ARunGameCharacter, ARunGameCharacter::StaticClass, TEXT("ARunGameCharacter"), &Z_Registration_Info_UClass_ARunGameCharacter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARunGameCharacter), 145057384U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameCharacter_h__Script_RunGame_2189918842(TEXT("/Script/RunGame"),
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameCharacter_h__Script_RunGame_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameCharacter_h__Script_RunGame_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
