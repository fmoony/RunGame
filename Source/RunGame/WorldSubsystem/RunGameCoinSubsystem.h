// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameCoinSubsystem.generated.h"

class ACoin;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinSubsystemReadySignature);

USTRUCT(BlueprintType)
struct FRunGameCoinBenchmarkStats
{
	GENERATED_BODY()

	/** 运行期间真实 SpawnActor 次数 / Number of real SpawnActor calls during benchmark */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Benchmark")
	int32 SpawnActorCount = 0;

	/** 金币预分配耗时 / Coin pre-allocation time */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Benchmark")
	double PreAllocateMs = 0.0;
};

UCLASS()
class RUNGAME_API URunGameCoinSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	URunGameCoinSubsystem();

	/** Pre-allocates a batch of hidden coins into the pool */
	UFUNCTION(BlueprintCallable, Category = "RunGame|CoinSystem")
	void PreAllocateCoins(TSubclassOf<ACoin> CoinClass, int32 Count);

	/** Pops a coin from pool (LIFO) or spawns a new one */
	UFUNCTION(BlueprintCallable, Category = "RunGame|CoinSystem")
	ACoin* AcquireCoin(TSubclassOf<ACoin> CoinClass);

	/** Returns a coin to the pool (LIFO push); coin is hidden in place */
	UFUNCTION(BlueprintCallable, Category = "RunGame|CoinSystem")
	void ReturnCoin(ACoin* Coin);

	/** Destroys all pooled and active coins */
	UFUNCTION(BlueprintCallable, Category = "RunGame|CoinSystem")
	void ClearAllCoins();

	UFUNCTION(BlueprintPure, Category = "RunGame|CoinSystem")
	int32 GetPooledCoinCount() const;

	UFUNCTION(BlueprintPure, Category = "RunGame|CoinSystem")
	int32 GetActiveCoinCount() const;

	/** 配置性能测试开关，默认不影响游戏路径 Configure benchmark toggles without changing normal gameplay defaults */
	UFUNCTION(BlueprintCallable, Category = "RunGame|CoinSystem|Benchmark")
	void SetBenchmarkDisablePool(bool bInDisablePool);

	/** 重置性能统计 Reset benchmark counters */
	UFUNCTION(BlueprintCallable, Category = "RunGame|CoinSystem|Benchmark")
	void ResetBenchmarkStats();

	/** 获取性能统计 Return benchmark counters */
	UFUNCTION(BlueprintPure, Category = "RunGame|CoinSystem|Benchmark")
	FRunGameCoinBenchmarkStats GetBenchmarkStats() const { return BenchmarkStats; }

	/** Broadcast when PreAllocateCoins finishes */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|CoinSystem")
	FOnCoinSubsystemReadySignature OnCoinSubsystemReady;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	/** Pops last coin from per-class sub-pool; returns nullptr if empty */
	ACoin* AcquireCoinFromPool(TSubclassOf<ACoin> CoinClass);

	/** Spawns a new coin actor (hidden, at origin) */
	ACoin* CreateNewCoin(TSubclassOf<ACoin> CoinClass);

	/** Per-class sub-pools of inactive coins, used as LIFO stacks */
	TMap<TSubclassOf<ACoin>, TArray<ACoin*>> PooledCoinsMap;

	/** All currently active coins in the world */
	UPROPERTY()
	TArray<TObjectPtr<ACoin>> ActiveCoins;

	/** 性能测试专用：禁用对象池以对比直接 Spawn/Destroy / Benchmark-only toggle that disables pooling for Spawn/Destroy comparison */
	bool bBenchmarkDisablePool = false;

	/** 性能测试统计数据 / Benchmark counters */
	FRunGameCoinBenchmarkStats BenchmarkStats;
};
