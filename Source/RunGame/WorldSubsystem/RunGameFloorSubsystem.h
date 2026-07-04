#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DataAssets/FloorConfigData.h"
#include "RunGameFloorSubsystem.generated.h"

class AFloorBase;
struct FFloorClassEntry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorSystemReadyDelegate);

USTRUCT(BlueprintType)
struct FRunGameFloorBenchmarkStats
{
	GENERATED_BODY()

	/** 运行期间真实 SpawnActor 次数 / Number of real SpawnActor calls during benchmark */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Benchmark")
	int32 SpawnActorCount = 0;

	/** 地板类加载耗时，异步和同步测试共用该字段 / Floor class load time, shared by async and sync scenarios */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Benchmark")
	double AsyncLoadMs = 0.0;

	/** 地板预分配耗时 / Floor pre-allocation time */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Benchmark")
	double PreAllocateMs = 0.0;
};

/** Internal entry holding a loaded floor class with its config metadata */
struct FLoadedFloorEntry
{
	TSubclassOf<AActor> LoadedClass;
	float SpawnWeight;
	int32 InitialGuaranteedCount;
	bool bEnableCoinSpawn = true;
};

UCLASS()
class RUNGAME_API URunGameFloorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Constructs the floor subsystem */
	URunGameFloorSubsystem();

	/** Initializes the floor system using a data-driven configuration asset */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void InitializeFloorSystem(UFloorConfigData* InConfig);

	/** Spawns the initial chain of floor segments using config-driven counts */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void SpawnInitialFloors(const FTransform& StartTransform);

	/** 重置性能测试的连续生成起点 / Reset the continuous spawn start for benchmark runs */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem|Benchmark")
	void BeginBenchmarkFloorChain(const FTransform& StartTransform);

	/** 生成指定数量的地板用于性能测试 / Spawn an exact floor count for performance tests */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem|Benchmark")
	void BenchmarkSpawnFloorChain(const FTransform& StartTransform, int32 FloorCount);

	/** Acquires the next random floor from the pool at the tracked spawn position */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	AFloorBase* RequestNextFloor();

	/** Acquires a floor of the specified class from the pool at the given location */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	AFloorBase* RequestFloorAt(TSubclassOf<AActor> InClass, const FVector& Location, const FRotator& Rotation);

	/** Returns a floor actor to the pool, hiding and deactivating it */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void ReturnFloor(AFloorBase* Floor);

	/** Recycles active floors farther than MaxDistance from the player. Pass 0 to use config value */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void RecycleDistantFloors(const FVector& PlayerLocation, float MaxDistance = 0.0f);

	/** Hides all active floors and returns them to the pool */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void HideAllActiveFloors();

	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem")
	FTransform GetNextSpawnTransform() const { return NextSpawnTransform; }

	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem")
	int32 GetActiveFloorCount() const { return ActiveFloors.Num(); }

	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem")
	int32 GetPooledFloorCount() const;

	/** Destroys all pooled and active floors, resetting to uninitialized state */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void ClearAllFloors();

	/** Returns the current floor configuration data asset */
	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem")
	UFloorConfigData* GetFloorConfig() const { return FloorConfig; }

	/** 配置性能测试开关，默认不影响游戏路径 Configure benchmark toggles without changing normal gameplay defaults */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem|Benchmark")
	void SetBenchmarkOptions(bool bInDisablePool, bool bInUseSynchronousLoad, int32 InPreAllocateOverride);

	/** 重置性能统计 Reset benchmark counters */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem|Benchmark")
	void ResetBenchmarkStats();

	/** 获取性能统计 Return benchmark counters */
	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem|Benchmark")
	FRunGameFloorBenchmarkStats GetBenchmarkStats() const { return BenchmarkStats; }

	/** Delegate broadcast when floor subsystem finishes async loading */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|FloorSystem")
	FOnFloorSystemReadyDelegate OnFloorSystemReady;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	/** Finds the loaded entry matching the given class */
	const FLoadedFloorEntry* FindLoadedEntry(TSubclassOf<AActor> InClass) const;

	/** Randomly selects a floor class using weighted probability from config */
	TSubclassOf<AActor> WeightedRandomSelectFloorClass();

	/** Pops a floor of given class from pool; returns nullptr if empty */
	AFloorBase* AcquireFloorFromPool(TSubclassOf<AActor> InClass);

	/** Spawns a new floor actor at the pool hide location */
	AFloorBase* CreateNewFloorActor(TSubclassOf<AActor> InClass);

	/** Binds to a floor's delegates when it becomes active */
	void BindFloorDelegates(AFloorBase* Floor);

	/** Unbinds from a floor's delegates before returning it to the pool */
	void UnbindFloorDelegates(AFloorBase* Floor);

	/** Called when a floor's box trigger is overlapped by the player */
	UFUNCTION()
	void OnFloorPlayerEntered(AFloorBase* Floor);

	/** Called when a floor's recycle timer expires */
	UFUNCTION()
	void OnFloorRecycleRequested(AFloorBase* Floor);

	/** Initiates async loading of floor classes via the asset manager */
	void StartAsyncLoad();

	/** 从已加载的软引用重建地板类列表 / Rebuild loaded floor class entries from resolved soft references */
	void BuildLoadedFloorEntries();

	/** Callback when async load finishes; pre-allocates the pool */
	UFUNCTION()
	void OnFloorClassesLoaded();

	// Config

	UPROPERTY()
	TObjectPtr<UFloorConfigData> FloorConfig;

	// Async-load: pending entries copied from config for load tracking
	TArray<FFloorClassEntry> PendingFloorEntries;

	// Loaded floor classes with spawn metadata
	TArray<FLoadedFloorEntry> LoadedFloorEntries;

	bool bIsLoading;
	bool bIsInitialized;

	// 性能测试开关，默认关闭以保持正式路径不变 Benchmark toggles, disabled by default for normal gameplay
	/** 性能测试专用：禁用地板对象池 / Benchmark-only toggle that disables the floor pool */
	bool bBenchmarkDisablePool = false;

	/** 性能测试专用：使用同步加载作为异步加载对照 / Benchmark-only toggle that uses sync loading as async-load comparison */
	bool bBenchmarkUseSynchronousLoad = false;

	/** 性能测试专用：覆盖预分配数量 / Benchmark-only override for pre-allocation count */
	int32 BenchmarkPreAllocateOverride = INDEX_NONE;

	/** 性能测试统计数据 / Benchmark counters */
	FRunGameFloorBenchmarkStats BenchmarkStats;

	/** 加载开始时间戳，用于计算加载耗时 / Load start timestamp used to calculate load duration */
	double AsyncLoadStartSeconds = 0.0;

	// Object pool: per-type sub-pools
	TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap;

	UPROPERTY()
	TArray<TObjectPtr<AFloorBase>> ActiveFloors;

	// Tracked position for next floor spawn
	FTransform NextSpawnTransform;
};
