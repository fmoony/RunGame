// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldSubsystem/RunGameCoinSubsystem.h"
#include "Actor/Collectible/Coin.h"
#include "Engine/World.h"
#include "RunGame.h"

URunGameCoinSubsystem::URunGameCoinSubsystem()
{
}

void URunGameCoinSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogRunGame, Warning, TEXT("RunGameCoinSubsystem: Initialized"));
}

void URunGameCoinSubsystem::Deinitialize()
{
	ClearAllCoins();
	UE_LOG(LogRunGame, Warning, TEXT("RunGameCoinSubsystem: Deinitialized"));
	Super::Deinitialize();
}

void URunGameCoinSubsystem::PreAllocateCoins(TSubclassOf<ACoin> CoinClass, int32 Count)
{
	if (!CoinClass)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameCoinSubsystem: PreAllocateCoins called with null CoinClass"));
		return;
	}

	TArray<ACoin*>& SubPool = PooledCoinsMap.FindOrAdd(CoinClass);

	for (int32 i = 0; i < Count; ++i)
	{
		ACoin* Coin = CreateNewCoin(CoinClass);
		if (Coin)
		{
			Coin->SetActorHiddenInGame(true);
			Coin->SetActorEnableCollision(false);
			Coin->SetActorTickEnabled(false);
			SubPool.Add(Coin);
		}
	}

	UE_LOG(LogRunGame, Warning, TEXT("RunGameCoinSubsystem: Pre-allocated %d coins of class %s"),
		Count, *GetNameSafe(CoinClass));

	OnCoinSubsystemReady.Broadcast();
}

ACoin* URunGameCoinSubsystem::AcquireCoinFromPool(TSubclassOf<ACoin> CoinClass)
{
	TArray<ACoin*>* SubPool = PooledCoinsMap.Find(CoinClass);
	if (SubPool && SubPool->Num() > 0)
	{
		return SubPool->Pop();
	}
	return nullptr;
}

ACoin* URunGameCoinSubsystem::CreateNewCoin(TSubclassOf<ACoin> CoinClass)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACoin* Coin = World->SpawnActor<ACoin>(CoinClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!Coin)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameCoinSubsystem: Failed to spawn coin of class %s"), *GetNameSafe(CoinClass));
	}

	return Coin;
}

ACoin* URunGameCoinSubsystem::AcquireCoin(TSubclassOf<ACoin> CoinClass)
{
	if (!CoinClass)
	{
		return nullptr;
	}

	ACoin* Coin = AcquireCoinFromPool(CoinClass);
	if (!Coin)
	{
		Coin = CreateNewCoin(CoinClass);
	}

	if (Coin)
	{
		ActiveCoins.Add(Coin);
	}

	return Coin;
}

void URunGameCoinSubsystem::ReturnCoin(ACoin* Coin)
{
	if (!Coin)
	{
		return;
	}

	ActiveCoins.RemoveSwap(Coin);

	TArray<ACoin*>& SubPool = PooledCoinsMap.FindOrAdd(Coin->GetClass());
	SubPool.Add(Coin);
}

void URunGameCoinSubsystem::ClearAllCoins()
{
	for (TPair<TSubclassOf<ACoin>, TArray<ACoin*>>& Pair : PooledCoinsMap)
	{
		for (ACoin* Coin : Pair.Value)
		{
			if (IsValid(Coin))
			{
				Coin->Destroy();
			}
		}
	}
	PooledCoinsMap.Empty();

	for (ACoin* Coin : ActiveCoins)
	{
		if (IsValid(Coin))
		{
			Coin->Destroy();
		}
	}
	ActiveCoins.Empty();

	UE_LOG(LogRunGame, Warning, TEXT("RunGameCoinSubsystem: All coins cleared"));
}

int32 URunGameCoinSubsystem::GetPooledCoinCount() const
{
	int32 Total = 0;
	for (const TPair<TSubclassOf<ACoin>, TArray<ACoin*>>& Pair : PooledCoinsMap)
	{
		Total += Pair.Value.Num();
	}
	return Total;
}

int32 URunGameCoinSubsystem::GetActiveCoinCount() const
{
	return ActiveCoins.Num();
}
