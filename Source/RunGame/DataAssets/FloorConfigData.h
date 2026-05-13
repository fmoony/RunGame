#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RunGameType.h"
#include "Actor/Component/CoinSpawnerComponent.h"
#include "FloorConfigData.generated.h"

USTRUCT(BlueprintType)
struct FFloorClassEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "FloorClass")
	TSoftClassPtr<AActor> FloorClass;

	UPROPERTY(EditDefaultsOnly, Category = "FloorClass")
	float SpawnWeight = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "FloorClass")
	int32 InitialGuaranteedCount = 0;

	/** Whether coins spawn on this floor class */
	UPROPERTY(EditDefaultsOnly, Category = "FloorClass")
	bool bEnableCoinSpawn = true;
};

UCLASS(BlueprintType)
class RUNGAME_API UFloorConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Floor|Classes")
	TArray<FFloorClassEntry> FloorClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Floor|Spawn")
	FVector SpawnStartOffset = FVector(-200.0f, 200.0f, -200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Floor|Spawn")
	FVector FloorScale = FVector(10.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Floor|Spawn")
	int32 InitialRandomFloorCount = 15;

	UPROPERTY(EditDefaultsOnly, Category = "Floor|Pool")
	int32 PreAllocateCount = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Floor|Pool")
	FVector PoolHideLocation = FVector(0.0f, 0.0f, -100000.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Floor|Recycle")
	float RecycleMaxDistance = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Floor|Recycle")
	float FloorRecycleDelay = 2.2f;

	/** Shared coin spawn configuration applied to all floor classes */
	UPROPERTY(EditDefaultsOnly, Category = "Floor|Coin")
	FCoinSpawnConfig GlobalCoinConfig;
};
