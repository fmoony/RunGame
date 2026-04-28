// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RunGameType.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeRunGameType() {}

// ********** Begin Cross Module References ********************************************************
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState();
RUNGAME_API UEnum* Z_Construct_UEnum_RunGame_FFloorType();
UPackage* Z_Construct_UPackage__Script_RunGame();
// ********** End Cross Module References **********************************************************

// ********** Begin Enum FFloorType ****************************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_FFloorType;
static UEnum* FFloorType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_FFloorType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_FFloorType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_RunGame_FFloorType, (UObject*)Z_Construct_UPackage__Script_RunGame(), TEXT("FFloorType"));
	}
	return Z_Registration_Info_UEnum_FFloorType.OuterSingleton;
}
template<> RUNGAME_API UEnum* StaticEnum<FFloorType>()
{
	return FFloorType_StaticEnum();
}
struct Z_Construct_UEnum_RunGame_FFloorType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "MAX.Name", "FFloorType::MAX" },
		{ "ModuleRelativePath", "RunGameType.h" },
		{ "StraightFloor.Name", "FFloorType::StraightFloor" },
		{ "TurnFloor.Name", "FFloorType::TurnFloor" },
		{ "UpAndDownFloor.Name", "FFloorType::UpAndDownFloor" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "FFloorType::StraightFloor", (int64)FFloorType::StraightFloor },
		{ "FFloorType::TurnFloor", (int64)FFloorType::TurnFloor },
		{ "FFloorType::UpAndDownFloor", (int64)FFloorType::UpAndDownFloor },
		{ "FFloorType::MAX", (int64)FFloorType::MAX },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_RunGame_FFloorType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_RunGame,
	nullptr,
	"FFloorType",
	"FFloorType",
	Z_Construct_UEnum_RunGame_FFloorType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_RunGame_FFloorType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_RunGame_FFloorType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_RunGame_FFloorType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_RunGame_FFloorType()
{
	if (!Z_Registration_Info_UEnum_FFloorType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_FFloorType.InnerSingleton, Z_Construct_UEnum_RunGame_FFloorType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_FFloorType.InnerSingleton;
}
// ********** End Enum FFloorType ******************************************************************

// ********** Begin Enum ERunGameGameState *********************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_ERunGameGameState;
static UEnum* ERunGameGameState_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_ERunGameGameState.OuterSingleton)
	{
		Z_Registration_Info_UEnum_ERunGameGameState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_RunGame_ERunGameGameState, (UObject*)Z_Construct_UPackage__Script_RunGame(), TEXT("ERunGameGameState"));
	}
	return Z_Registration_Info_UEnum_ERunGameGameState.OuterSingleton;
}
template<> RUNGAME_API UEnum* StaticEnum<ERunGameGameState>()
{
	return ERunGameGameState_StaticEnum();
}
struct Z_Construct_UEnum_RunGame_ERunGameGameState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "CountDown.Name", "ERunGameGameState::CountDown" },
		{ "GameOver.Name", "ERunGameGameState::GameOver" },
		{ "InGame.Name", "ERunGameGameState::InGame" },
		{ "MainMenu.Name", "ERunGameGameState::MainMenu" },
		{ "MAX.Name", "ERunGameGameState::MAX" },
		{ "ModuleRelativePath", "RunGameType.h" },
		{ "Pause.Name", "ERunGameGameState::Pause" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "ERunGameGameState::MainMenu", (int64)ERunGameGameState::MainMenu },
		{ "ERunGameGameState::CountDown", (int64)ERunGameGameState::CountDown },
		{ "ERunGameGameState::InGame", (int64)ERunGameGameState::InGame },
		{ "ERunGameGameState::Pause", (int64)ERunGameGameState::Pause },
		{ "ERunGameGameState::GameOver", (int64)ERunGameGameState::GameOver },
		{ "ERunGameGameState::MAX", (int64)ERunGameGameState::MAX },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_RunGame_ERunGameGameState_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_RunGame,
	nullptr,
	"ERunGameGameState",
	"ERunGameGameState",
	Z_Construct_UEnum_RunGame_ERunGameGameState_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_RunGame_ERunGameGameState_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_RunGame_ERunGameGameState_Statics::Enum_MetaDataParams), Z_Construct_UEnum_RunGame_ERunGameGameState_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_RunGame_ERunGameGameState()
{
	if (!Z_Registration_Info_UEnum_ERunGameGameState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ERunGameGameState.InnerSingleton, Z_Construct_UEnum_RunGame_ERunGameGameState_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_ERunGameGameState.InnerSingleton;
}
// ********** End Enum ERunGameGameState ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameType_h__Script_RunGame_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ FFloorType_StaticEnum, TEXT("FFloorType"), &Z_Registration_Info_UEnum_FFloorType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 471990832U) },
		{ ERunGameGameState_StaticEnum, TEXT("ERunGameGameState"), &Z_Registration_Info_UEnum_ERunGameGameState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1491870806U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameType_h__Script_RunGame_881594856(TEXT("/Script/RunGame"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameType_h__Script_RunGame_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RunGame_Source_RunGame_RunGameType_h__Script_RunGame_Statics::EnumInfo));
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
