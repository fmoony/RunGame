// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoinSpawnerComponent.generated.h"

class ACoin;
class USplineComponent;

/** Fallback pattern when spline has no points */
UENUM()
enum class ECoinSpawnPattern : uint8
{
	StraightLine,
	DiagonalLine,
	Parabolic,
	MAX,
};

/** Per-floor-class coin spawn configuration, driven by data asset */
USTRUCT(BlueprintType)
struct FCoinSpawnConfig
{
	GENERATED_BODY()

	/** Coin class to spawn. nullptr = no coins */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	TSubclassOf<ACoin> CoinClass;

	/** Coins per row */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	int32 CoinCount = 3;

	/** Number of parallel rows (left/center/right lanes) */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	int32 RowCount = 1;

	/** Lateral spacing between rows in world units */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	float RowSpacing = 100.0f;

	/** Probability each non-center row spawns (0-1, 1 = always all rows) */
	UPROPERTY(EditDefaultsOnly, Category = "Coin", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RowSpawnChance = 1.0f;

	/** Fallback pattern when spline is empty */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	ECoinSpawnPattern SpawnPattern = ECoinSpawnPattern::StraightLine;

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector StartOffset = FVector(0.0f, 0.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector LineDirection = FVector(100.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector EndOffset = FVector(200.0f, 100.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	float ParabolaHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	float CoinSpacing = 80.0f;

	bool HasValidCoinClass() const;
};

UCLASS(ClassGroup = (RunGame), meta = (BlueprintSpawnableComponent))
class RUNGAME_API UCoinSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoinSpawnerComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** Applies coin spawn configuration from the data asset */
	void ApplyConfig(const FCoinSpawnConfig& Config);

	/** Acquires coins from subsystem and places them on this floor */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Coin")
	void SpawnCoins();

	/** Returns all managed coins to the pool */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Coin")
	void DespawnCoins();

protected:
	TSubclassOf<ACoin> CoinClass;

	int32 CoinCount = 3;

	int32 RowCount = 1;

	float RowSpacing = 100.0f;

	float RowSpawnChance = 1.0f;

	ECoinSpawnPattern SpawnPattern = ECoinSpawnPattern::StraightLine;

	FVector StartOffset = FVector(0.0f, 0.0f, 100.0f);

	FVector LineDirection = FVector(100.0f, 0.0f, 0.0f);

	FVector EndOffset = FVector(200.0f, 100.0f, 0.0f);

	float ParabolaHeight = 200.0f;

	float CoinSpacing = 80.0f;

	bool bEnableCoinSpawn = true;

private:
	/** Computes world-space transforms: spline first, enum fallback, multi-row */
	TArray<FTransform> CalculateCoinTransforms() const;

	/** Applies row offset perpendicular to the given local direction */
	void ApplyRowOffset(FVector& InOutWorldPos, const FVector& PerpendicularWorldDir, int32 RowIndex) const;

	/** Computes world-space transforms along the spline for one row */
	TArray<FTransform> CalculateSplineTransforms() const;

	void CalculateStraightLine(TArray<FTransform>& OutTransforms) const;
	void CalculateDiagonalLine(TArray<FTransform>& OutTransforms) const;
	void CalculateParabolic(TArray<FTransform>& OutTransforms) const;

	/** Coins currently managed by this component */
	UPROPERTY()
	TArray<TObjectPtr<ACoin>> SpawnedCoins;

	/** Cached spline from owner, populated in BeginPlay */
	UPROPERTY()
	TObjectPtr<USplineComponent> CachedSpline;
};
