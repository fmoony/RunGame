// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Component/CoinSpawnerComponent.h"
#include "Actor/Collectible/Coin.h"
#include "WorldSubsystem/RunGameCoinSubsystem.h"
#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "RunGame.h"

UCoinSpawnerComponent::UCoinSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCoinSpawnerComponent::SpawnCoins()
{
	if (!CoinClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	URunGameCoinSubsystem* CoinSubsystem = World->GetSubsystem<URunGameCoinSubsystem>();
	if (!CoinSubsystem)
	{
		UE_LOG(LogRunGame, Error, TEXT("CoinSpawnerComponent: No URunGameCoinSubsystem available"));
		return;
	}

	DespawnCoins();

	const TArray<FTransform> Transforms = CalculateCoinTransforms();

	for (const FTransform& CoinTransform : Transforms)
	{
		ACoin* Coin = CoinSubsystem->AcquireCoin(CoinClass);
		if (Coin)
		{
			Coin->ActivateCollectible(CoinTransform);
			SpawnedCoins.Add(Coin);
		}
	}
}

void UCoinSpawnerComponent::DespawnCoins()
{
	if (SpawnedCoins.Num() == 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	URunGameCoinSubsystem* CoinSubsystem = World ? World->GetSubsystem<URunGameCoinSubsystem>() : nullptr;

	for (ACoin* Coin : SpawnedCoins)
	{
		if (!Coin)
		{
			continue;
		}

		if (!Coin->IsCollected())
		{
			Coin->DeactivateCollectible();
		}

		if (CoinSubsystem)
		{
			CoinSubsystem->ReturnCoin(Coin);
		}
	}

	SpawnedCoins.Empty();
}

TArray<FTransform> UCoinSpawnerComponent::CalculateCoinTransforms() const
{
	TArray<FTransform> OutTransforms;

	const AActor* Owner = GetOwner();
	const USplineComponent* Spline = Owner ? Owner->GetComponentByClass<USplineComponent>() : nullptr;

	if (Spline && Spline->GetNumberOfSplinePoints() > 0)
	{
		const float TotalLength = Spline->GetSplineLength();
		if (TotalLength <= 0.0f)
		{
			return OutTransforms;
		}

		OutTransforms.Reserve(CoinCount);

		const int32 Steps = FMath::Max(CoinCount - 1, 1);
		for (int32 i = 0; i < CoinCount; ++i)
		{
			const float Distance = TotalLength * static_cast<float>(i) / static_cast<float>(Steps);
			const FVector WorldPos = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			OutTransforms.Add(FTransform(WorldPos));
		}

		return OutTransforms;
	}

	switch (SpawnPattern)
	{
	case ECoinSpawnPattern::StraightLine:
		CalculateStraightLine(OutTransforms);
		break;
	case ECoinSpawnPattern::DiagonalLine:
		CalculateDiagonalLine(OutTransforms);
		break;
	case ECoinSpawnPattern::Parabolic:
		CalculateParabolic(OutTransforms);
		break;
	default:
		break;
	}

	return OutTransforms;
}

void UCoinSpawnerComponent::CalculateStraightLine(TArray<FTransform>& OutTransforms) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FTransform OwnerTransform = Owner->GetActorTransform();
	OutTransforms.Reserve(CoinCount);

	for (int32 i = 0; i < CoinCount; ++i)
	{
		const FVector LocalPos = StartOffset + LineDirection * static_cast<float>(i) * CoinSpacing;
		const FVector WorldPos = OwnerTransform.TransformPosition(LocalPos);
		OutTransforms.Add(FTransform(WorldPos));
	}
}

void UCoinSpawnerComponent::CalculateDiagonalLine(TArray<FTransform>& OutTransforms) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FTransform OwnerTransform = Owner->GetActorTransform();
	OutTransforms.Reserve(CoinCount);

	for (int32 i = 0; i < CoinCount; ++i)
	{
		const FVector LocalPos = StartOffset + LineDirection * static_cast<float>(i) * CoinSpacing;
		const FVector WorldPos = OwnerTransform.TransformPosition(LocalPos);
		OutTransforms.Add(FTransform(WorldPos));
	}
}

void UCoinSpawnerComponent::CalculateParabolic(TArray<FTransform>& OutTransforms) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FTransform OwnerTransform = Owner->GetActorTransform();
	OutTransforms.Reserve(CoinCount);

	const FVector P0 = StartOffset;
	const FVector P2 = EndOffset;
	const FVector P1 = (P0 + P2) * 0.5f + FVector(0.0f, 0.0f, ParabolaHeight);

	const int32 Steps = FMath::Max(CoinCount - 1, 1);
	for (int32 i = 0; i < CoinCount; ++i)
	{
		const float t = static_cast<float>(i) / static_cast<float>(Steps);
		const float OneMinusT = 1.0f - t;
		const FVector LocalPos = OneMinusT * OneMinusT * P0 + 2.0f * OneMinusT * t * P1 + t * t * P2;
		const FVector WorldPos = OwnerTransform.TransformPosition(LocalPos);
		OutTransforms.Add(FTransform(WorldPos));
	}
}
