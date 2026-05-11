// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoinSpawnerComponent.generated.h"

class ACoin;

/** Fallback pattern when spline has no points */
UENUM()
enum class ECoinSpawnPattern : uint8
{
	StraightLine,
	DiagonalLine,
	Parabolic,
	MAX,
};

UCLASS(ClassGroup = (RunGame), meta = (BlueprintSpawnableComponent))
class RUNGAME_API UCoinSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoinSpawnerComponent();

	/** Acquires coins from subsystem and places them on this floor */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Coin")
	void SpawnCoins();

	/** Returns all managed coins to the pool */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Coin")
	void DespawnCoins();

	bool HasValidCoinClass() const { return CoinClass != nullptr; }

protected:
	/** Coin class to spawn. nullptr = no coins on this floor */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	TSubclassOf<ACoin> CoinClass;

	/** Number of coins to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Coin", meta = (ClampMin = "1"))
	int32 CoinCount = 3;

	/** Fallback pattern when spline is empty */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	ECoinSpawnPattern SpawnPattern = ECoinSpawnPattern::StraightLine;

	/** Spacing between coins in world units (line patterns only) */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	float CoinSpacing = 80.0f;

	/** Peak height of parabolic arc above midpoint (Parabolic only) */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	float ParabolaHeight = 200.0f;

	/** Local-space start offset relative to floor origin */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector StartOffset = FVector(0.0f, 0.0f, 100.0f);

	/** Local-space direction vector for line patterns */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector LineDirection = FVector(100.0f, 0.0f, 0.0f);

	/** Local-space end offset for parabolic pattern */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector EndOffset = FVector(200.0f, 100.0f, 0.0f);

private:
	/** Computes world-space transforms: spline first, enum fallback */
	TArray<FTransform> CalculateCoinTransforms() const;

	void CalculateStraightLine(TArray<FTransform>& OutTransforms) const;
	void CalculateDiagonalLine(TArray<FTransform>& OutTransforms) const;
	void CalculateParabolic(TArray<FTransform>& OutTransforms) const;

	/** Coins currently managed by this component */
	UPROPERTY()
	TArray<TObjectPtr<ACoin>> SpawnedCoins;
};
