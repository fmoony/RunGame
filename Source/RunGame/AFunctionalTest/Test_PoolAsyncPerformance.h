#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "TimerManager.h"
#include "DataAssets/FloorConfigData.h"
#include "Test_PoolAsyncPerformance.generated.h"

class ATrap;
class URunGameCoinSubsystem;
class URunGameFloorSubsystem;

USTRUCT()
struct FPoolAsyncBenchmarkRow
{
	GENERATED_BODY()

	/** 场景名称，用于区分对象池/加载方式组合 / Scenario label for pool and loading mode combination */
	FString ScenarioName;

	/** 是否启用对象池 / Whether object pooling is enabled */
	bool bPoolEnabled = true;

	/** 是否使用异步加载 / Whether async loading is enabled */
	bool bAsyncLoading = true;

	/** 地板类加载耗时，来自 FloorSubsystem benchmark 统计 / Floor class load time from FloorSubsystem benchmark stats */
	double LoadMs = 0.0;

	/** 地板和金币预分配总耗时 / Combined floor and coin pre-allocation time */
	double PreAllocateMs = 0.0;

	/** 生成测试负载的 GameThread 耗时 / GameThread time spent generating the benchmark workload */
	double GameThreadMs = 0.0;

	/** 场景逐步运行的实际墙钟耗时 / Wall-clock time spent running the stepped scenario */
	double ScenarioWallMs = 0.0;

	/** 地板回收测试耗时 / Time spent recycling benchmark floors */
	double RecycleMs = 0.0;

	/** 测试负载阶段新增 SpawnActor 次数 / SpawnActor count added during the workload stage */
	int32 SpawnActorCount = 0;

	/** 场景期间实际完成的逐步地板生成数量 / Completed stepped floor spawns during the scenario */
	int32 GeneratedFloorCount = 0;

	/** 场景期间完成的 GC 次数，包含自动 GC 和测试显式 GC / Completed GC count during the scenario, including automatic and explicit GC */
	int32 GCCount = 0;

	/** 当前世界可见 Actor 数量 / Visible actor count in the current world */
	int32 ActiveActorCount = 0;

	/** FloorSubsystem 中仍处于 Active 的地板数量 / Active floor count tracked by FloorSubsystem */
	int32 ActiveFloorCount = 0;

	/** CoinSubsystem 中仍处于 Active 的金币数量 / Active coin count tracked by CoinSubsystem */
	int32 ActiveCoinCount = 0;

	/** 世界中可见 Trap 数量，陷阱由地板配置驱动 / Visible trap count, driven by floor configuration */
	int32 ActiveTrapCount = 0;

	/** 测试期间采样到的物理内存峰值 / Peak sampled used physical memory during the test */
	uint64 PeakUsedPhysicalMB = 0;
};

/**
 * 对象池/异步加载性能测试，输出 Pool ON/OFF 对比 CSV
 * Object-pool and async-load benchmark that writes Pool ON/OFF comparison CSV
 */
UCLASS(Blueprintable)
class RUNGAME_API ATest_PoolAsyncPerformance : public AFunctionalTest
{
	GENERATED_BODY()

public:
	/** 构造性能测试默认参数 Construct benchmark defaults */
	ATest_PoolAsyncPerformance();

	/** 是否在 BeginPlay 后自动开始，默认关闭以避免误跑 / Auto-start after BeginPlay; disabled by default to avoid accidental runs */
	UPROPERTY(EditAnywhere, Category = "Benchmark|Run")
	bool bAutoRunOnBeginPlay = false;

	/** 地板配置数据源，金币和陷阱均跟随地板配置 / Floor config source; coins and traps are driven by floor setup */
	UPROPERTY(EditAnywhere, Category = "Benchmark|Config")
	TObjectPtr<UFloorConfigData> TestFloorConfig;

	/** 是否额外跑同步加载对照组 / Whether to include an additional synchronous-load comparison */
	UPROPERTY(EditAnywhere, Category = "Benchmark|Load")
	bool bIncludeSynchronousLoadComparison = true;

	/** 逐步生成的目标地板段数量，默认用于 200 段压力测试 / Target floor segment count for stepped workload */
	UPROPERTY(EditAnywhere, Category = "Benchmark|Counts", meta = (ClampMin = "1"))
	int32 FloorSegmentCount = 200;

	/** 单个场景的压缩运行时长，用于模拟玩家逐块经过地板 / Compressed run duration per scenario for stepped player traversal */
	UPROPERTY(EditAnywhere, Category = "Benchmark|Run", meta = (ClampMin = "0.1"))
	float ScenarioDurationSeconds = 5.0f;

	/** 模拟玩家移动时使用的回收距离，0 表示使用地板配置 / Recycle distance for simulated player movement; 0 uses floor config */
	UPROPERTY(EditAnywhere, Category = "Benchmark|Recycle")
	float RecycleDistance = 0.0f;

	/** 输出 CSV 文件名，目录固定在 Saved/Profiling/RunGamePoolBenchmark / CSV output file name under Saved/Profiling/RunGamePoolBenchmark */
	UPROPERTY(EditAnywhere, Category = "Benchmark|Output")
	FString OutputFileName = TEXT("PoolAsyncBenchmark.csv");

protected:
	virtual void BeginPlay() override;
	virtual void StartTest() override;
	virtual void CleanUp() override;

private:
	/** 单个待测场景配置 / Single benchmark scenario definition */
	struct FPendingScenario
	{
		/** 场景名称 / Scenario name */
		FString Name;

		/** 是否启用对象池 / Whether pooling is enabled */
		bool bPoolEnabled = true;

		/** 是否使用异步加载 / Whether async loading is enabled */
		bool bAsyncLoading = true;
	};

	/** 启动下一个场景，所有场景完成后写出 CSV / Start the next scenario, writing CSV after all scenarios complete */
	void StartNextScenario();

	/** 响应地板系统初始化完成并进入场景执行 / Handle floor system readiness and run the scenario */
	UFUNCTION()
	void OnFloorSystemReadyCallback();

	/** 记录引擎 GC 完成事件 / Record completed engine GC events */
	void OnGarbageCollectComplete();

	/** 执行当前场景的生成、统计、回收和 GC 流程 / Run workload, metrics, recycling, and GC for the current scenario */
	void ExecuteCurrentScenario();

	/** 执行一次模拟玩家经过后的地板生成 / Run one simulated player traversal floor step */
	void RunScenarioStep();

	/** 完成当前逐步场景并记录统计 / Finish the current stepped scenario and record metrics */
	void CompleteCurrentScenario();

	/** 下一帧清理场景并启动后续测试 / Clean up the scenario next tick and continue testing */
	void FinalizeCurrentScenario();

	/** 清理当前场景产生的地板和金币 / Clear floors and coins created by the current scenario */
	void CleanupScenarioActors();

	/** 记录内存峰值采样 / Record peak memory sample */
	void RecordMemorySample(FPoolAsyncBenchmarkRow& Row) const;

	/** 统计当前世界可见 Actor 数 / Count visible actors in the current world */
	int32 CountActiveActors() const;

	/** 统计当前世界可见 Trap 数 / Count visible traps in the current world */
	int32 CountActiveTraps() const;

	/** 校验测试运行所需配置和子系统 / Validate required config and subsystems */
	bool ValidateRequiredConfig();

	/** 写出 CSV 并结束 FunctionalTest / Write CSV and finish the functional test */
	void WriteCsvAndFinish();

	/** 构建 CSV 文本内容 / Build CSV output text */
	FString BuildCsv() const;

	UPROPERTY()
	TObjectPtr<URunGameFloorSubsystem> FloorSubsystem;

	UPROPERTY()
	TObjectPtr<URunGameCoinSubsystem> CoinSubsystem;

	/** 等待执行的场景队列 / Pending benchmark scenario queue */
	TArray<FPendingScenario> PendingScenarios;

	/** 已完成场景结果 / Completed scenario results */
	TArray<FPoolAsyncBenchmarkRow> Results;

	/** 当前正在采样的场景结果 / Result row currently being sampled */
	FPoolAsyncBenchmarkRow CurrentRow;

	/** 当前场景索引 / Current scenario index */
	int32 CurrentScenarioIndex = INDEX_NONE;

	/** 当前场景负载前的 SpawnActor 基线 / SpawnActor baseline before the current scenario workload */
	int32 CurrentPreWorkloadSpawnActorCount = 0;

	/** 当前场景已经逐步生成的地板数量 / Number of stepped floor spawns already completed in the current scenario */
	int32 CurrentGeneratedFloorCount = 0;

	/** 已观察到的 GC 完成事件总数 / Total observed completed GC events */
	int32 ObservedGCCount = 0;

	/** 当前场景开始时的 GC 计数基线 / GC count baseline at current scenario start */
	int32 ScenarioGCStartCount = 0;

	/** 当前场景开始的墙钟时间 / Wall-clock start time for the current scenario */
	double CurrentScenarioStartSeconds = 0.0;

	/** 当前场景累计地板生成 GameThread 时间 / Accumulated floor generation GameThread time for the current scenario */
	double CurrentGameThreadSeconds = 0.0;

	/** 当前场景累计回收耗时 / Accumulated recycle time for the current scenario */
	double CurrentRecycleSeconds = 0.0;

	/** 逐步场景定时器句柄 / Timer handle for stepped scenario execution */
	FTimerHandle ScenarioStepTimerHandle;

	/** GC 完成委托句柄，用于测试结束时解绑 / Delegate handle for unbinding GC completion callback */
	FDelegateHandle GarbageCollectCompleteHandle;

	/** 防止 FunctionalTest 和 BeginPlay 重复启动 / Prevent duplicate start from FunctionalTest and BeginPlay */
	bool bBenchmarkStarted = false;
};
