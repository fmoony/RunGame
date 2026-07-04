#include "Test_PoolAsyncPerformance.h"
#include "Actor/Floor/FloorBase.h"
#include "Actor/Trap/Trap.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/PlatformMemory.h"
#include "HAL/PlatformTime.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"
#include "WorldSubsystem/RunGameCoinSubsystem.h"
#include "WorldSubsystem/RunGameFloorSubsystem.h"

ATest_PoolAsyncPerformance::ATest_PoolAsyncPerformance()
{
	TimeLimit = 180.0f;
}

void ATest_PoolAsyncPerformance::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoRunOnBeginPlay && GetWorld() && GetWorld()->IsGameWorld())
	{
		// 下一帧启动，给 GameMode/Subsystem BeginPlay 留出初始化时间 Start next tick so GameMode/subsystems finish BeginPlay first
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ATest_PoolAsyncPerformance::StartTest));
	}
}

void ATest_PoolAsyncPerformance::StartTest()
{
	if (bBenchmarkStarted)
	{
		return;
	}
	bBenchmarkStarted = true;

	ObservedGCCount = 0;
	if (!GarbageCollectCompleteHandle.IsValid())
	{
		GarbageCollectCompleteHandle = FCoreUObjectDelegates::GarbageCollectComplete.AddUObject(
			this,
			&ATest_PoolAsyncPerformance::OnGarbageCollectComplete);
	}

	FloorSubsystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>();
	CoinSubsystem = GetWorld()->GetSubsystem<URunGameCoinSubsystem>();

	if (!ValidateRequiredConfig())
	{
		return;
	}

	// 按固定顺序跑对象池/加载模式对照，便于 CSV 横向比较 / Run pool and loading scenarios in a stable order for CSV comparison
	PendingScenarios.Reset();
	PendingScenarios.Add({ TEXT("Pool_ON_AsyncLoad"), true, true });
	PendingScenarios.Add({ TEXT("Pool_OFF_AsyncLoad"), false, true });
	if (bIncludeSynchronousLoadComparison)
	{
		PendingScenarios.Add({ TEXT("Pool_ON_SyncLoad"), true, false });
	}

	Results.Reset();
	CurrentScenarioIndex = INDEX_NONE;
	StartNextScenario();
}

bool ATest_PoolAsyncPerformance::ValidateRequiredConfig()
{
	if (!FloorSubsystem || !CoinSubsystem)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Required floor or coin subsystem is missing."));
		return false;
	}

	if (!TestFloorConfig)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("TestFloorConfig is required."));
		return false;
	}

	return true;
}

void ATest_PoolAsyncPerformance::StartNextScenario()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ScenarioStepTimerHandle);
	}

	CleanupScenarioActors();

	CurrentScenarioIndex++;
	if (!PendingScenarios.IsValidIndex(CurrentScenarioIndex))
	{
		WriteCsvAndFinish();
		return;
	}

	const FPendingScenario& Scenario = PendingScenarios[CurrentScenarioIndex];
	LogStep(ELogVerbosity::Log, FString::Printf(TEXT("Starting benchmark scenario: %s"), *Scenario.Name));
	ScenarioGCStartCount = ObservedGCCount;

	FloorSubsystem->ClearAllFloors();
	CoinSubsystem->ClearAllCoins();
	FloorSubsystem->ResetBenchmarkStats();
	CoinSubsystem->ResetBenchmarkStats();

	// 预分配使用 FloorConfigData 的真实配置，测试目标数量只用于后续逐步生成 / Use real FloorConfigData pre-allocation; target count only drives stepped spawning
	FloorSubsystem->SetBenchmarkOptions(!Scenario.bPoolEnabled, !Scenario.bAsyncLoading, INDEX_NONE);
	CoinSubsystem->SetBenchmarkDisablePool(!Scenario.bPoolEnabled);

	// 隔离 GameMode 的地板初始化回调，避免测试期间额外生成地板 Isolate GameMode floor-ready callbacks to prevent extra floor spawning during benchmark
	FloorSubsystem->OnFloorSystemReady.Clear();
	FloorSubsystem->OnFloorSystemReady.AddUniqueDynamic(this, &ATest_PoolAsyncPerformance::OnFloorSystemReadyCallback);
	FloorSubsystem->InitializeFloorSystem(TestFloorConfig);
}

void ATest_PoolAsyncPerformance::OnFloorSystemReadyCallback()
{
	if (FloorSubsystem)
	{
		FloorSubsystem->OnFloorSystemReady.RemoveDynamic(this, &ATest_PoolAsyncPerformance::OnFloorSystemReadyCallback);
	}

	ExecuteCurrentScenario();
}

void ATest_PoolAsyncPerformance::OnGarbageCollectComplete()
{
	ObservedGCCount++;
}

void ATest_PoolAsyncPerformance::ExecuteCurrentScenario()
{
	if (!PendingScenarios.IsValidIndex(CurrentScenarioIndex))
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Benchmark scenario index became invalid."));
		return;
	}

	const FPendingScenario& Scenario = PendingScenarios[CurrentScenarioIndex];
	CurrentRow = FPoolAsyncBenchmarkRow();
	CurrentRow.ScenarioName = Scenario.Name;
	CurrentRow.bPoolEnabled = Scenario.bPoolEnabled;
	CurrentRow.bAsyncLoading = Scenario.bAsyncLoading;

	// 先采样基线内存，后续阶段只记录峰值 / Take baseline memory sample; later samples only raise the peak
	RecordMemorySample(CurrentRow);

	// 预分配阶段可能产生 SpawnActor，负载计数只统计逐步生成阶段增量 / Pre-allocation may spawn actors; workload count records only stepped-spawn delta
	const FRunGameFloorBenchmarkStats FloorStatsBeforeWorkload = FloorSubsystem->GetBenchmarkStats();
	const FRunGameCoinBenchmarkStats CoinStatsBeforeWorkload = CoinSubsystem->GetBenchmarkStats();
	CurrentPreWorkloadSpawnActorCount = FloorStatsBeforeWorkload.SpawnActorCount + CoinStatsBeforeWorkload.SpawnActorCount;

	// 地板驱动逐步生成：金币和陷阱跟随真实地板配置，不由测试直接生成 / Floor-driven stepped workload; coins and traps follow real floor config
	CurrentGeneratedFloorCount = 0;
	CurrentGameThreadSeconds = 0.0;
	CurrentRecycleSeconds = 0.0;
	CurrentScenarioStartSeconds = FPlatformTime::Seconds();
	FloorSubsystem->BeginBenchmarkFloorChain(FTransform::Identity);

	const int32 SafeFloorCount = FMath::Max(1, FloorSegmentCount);
	const float StepInterval = FMath::Max(0.001f, ScenarioDurationSeconds / SafeFloorCount);
	LogStep(ELogVerbosity::Log, FString::Printf(
		TEXT("%s: stepped workload started, TargetFloors=%d, Duration=%.3fs, StepInterval=%.3fs"),
		*Scenario.Name,
		FloorSegmentCount,
		ScenarioDurationSeconds,
		StepInterval));

	if (!GetWorld())
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Benchmark world is missing."));
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		ScenarioStepTimerHandle,
		this,
		&ATest_PoolAsyncPerformance::RunScenarioStep,
		StepInterval,
		true,
		0.0f);

}

void ATest_PoolAsyncPerformance::RunScenarioStep()
{
	if (!FloorSubsystem)
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(ScenarioStepTimerHandle);
		}
		FinishTest(EFunctionalTestResult::Failed, TEXT("Floor subsystem became invalid during benchmark."));
		return;
	}

	if (CurrentGeneratedFloorCount >= FloorSegmentCount)
	{
		CompleteCurrentScenario();
		return;
	}

	// 每一步模拟玩家进入下一块地板后触发新地板生成 / Each step simulates the player entering the next floor and requesting a new segment
	const double StepStartSeconds = FPlatformTime::Seconds();
	AFloorBase* SpawnedFloor = FloorSubsystem->RequestNextFloor();
	CurrentGameThreadSeconds += FPlatformTime::Seconds() - StepStartSeconds;

	if (!SpawnedFloor)
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(ScenarioStepTimerHandle);
		}
		FinishTest(EFunctionalTestResult::Failed, TEXT("Failed to spawn benchmark floor step."));
		return;
	}

	CurrentGeneratedFloorCount++;
	CurrentRow.GeneratedFloorCount = CurrentGeneratedFloorCount;
	RecordMemorySample(CurrentRow);

	// 使用生成地板的位置作为压缩后的玩家位置，按真实回收路径淘汰远处地板 / Use the spawned floor as compressed player position and recycle via the real path
	const double RecycleStartSeconds = FPlatformTime::Seconds();
	FloorSubsystem->RecycleDistantFloors(SpawnedFloor->GetActorLocation(), RecycleDistance);
	CurrentRecycleSeconds += FPlatformTime::Seconds() - RecycleStartSeconds;
	RecordMemorySample(CurrentRow);

	if (CurrentGeneratedFloorCount >= FloorSegmentCount)
	{
		CompleteCurrentScenario();
	}
}

void ATest_PoolAsyncPerformance::CompleteCurrentScenario()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ScenarioStepTimerHandle);
	}

	CurrentRow.ScenarioWallMs = (FPlatformTime::Seconds() - CurrentScenarioStartSeconds) * 1000.0;
	CurrentRow.GameThreadMs = CurrentGameThreadSeconds * 1000.0;
	CurrentRow.RecycleMs = CurrentRecycleSeconds * 1000.0;
	RecordMemorySample(CurrentRow);

	CurrentRow.ActiveFloorCount = FloorSubsystem ? FloorSubsystem->GetActiveFloorCount() : 0;
	CurrentRow.ActiveCoinCount = CoinSubsystem ? CoinSubsystem->GetActiveCoinCount() : 0;
	CurrentRow.ActiveTrapCount = CountActiveTraps();
	CurrentRow.ActiveActorCount = CountActiveActors();

	const FRunGameFloorBenchmarkStats FloorStats = FloorSubsystem->GetBenchmarkStats();
	const FRunGameCoinBenchmarkStats CoinStats = CoinSubsystem->GetBenchmarkStats();
	CurrentRow.LoadMs = FloorStats.AsyncLoadMs;
	CurrentRow.PreAllocateMs = FloorStats.PreAllocateMs + CoinStats.PreAllocateMs;
	CurrentRow.SpawnActorCount = FloorStats.SpawnActorCount + CoinStats.SpawnActorCount - CurrentPreWorkloadSpawnActorCount;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ATest_PoolAsyncPerformance::FinalizeCurrentScenario));
		return;
	}

	FinalizeCurrentScenario();
}

void ATest_PoolAsyncPerformance::FinalizeCurrentScenario()
{
	CleanupScenarioActors();
	CollectGarbage(RF_NoFlags);
	CurrentRow.GCCount = FMath::Max(0, ObservedGCCount - ScenarioGCStartCount);
	RecordMemorySample(CurrentRow);

	// 当前场景完成后立即进入下一场景，避免跨场景状态残留 / Start the next scenario immediately after cleanup to avoid cross-scenario state leakage
	Results.Add(CurrentRow);
	LogStep(ELogVerbosity::Log, FString::Printf(
		TEXT("%s: GeneratedFloors=%d, Wall=%.3fms, GameThread=%.3fms, SpawnActor=%d, GC=%d, Recycle=%.3fms, ActiveActors=%d, PeakMB=%llu"),
		*CurrentRow.ScenarioName,
		CurrentRow.GeneratedFloorCount,
		CurrentRow.ScenarioWallMs,
		CurrentRow.GameThreadMs,
		CurrentRow.SpawnActorCount,
		CurrentRow.GCCount,
		CurrentRow.RecycleMs,
		CurrentRow.ActiveActorCount,
		CurrentRow.PeakUsedPhysicalMB));

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ATest_PoolAsyncPerformance::StartNextScenario));
		return;
	}

	StartNextScenario();
}

void ATest_PoolAsyncPerformance::CleanupScenarioActors()
{
	if (FloorSubsystem)
	{
		FloorSubsystem->ClearAllFloors();
	}

	if (CoinSubsystem)
	{
		CoinSubsystem->ClearAllCoins();
	}
}

void ATest_PoolAsyncPerformance::RecordMemorySample(FPoolAsyncBenchmarkRow& Row) const
{
	const FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
	const uint64 UsedPhysicalMB = Stats.UsedPhysical / (1024ull * 1024ull);
	Row.PeakUsedPhysicalMB = FMath::Max(Row.PeakUsedPhysicalMB, UsedPhysicalMB);
}

int32 ATest_PoolAsyncPerformance::CountActiveActors() const
{
	int32 Count = 0;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (!It->IsActorBeingDestroyed() && !It->IsHidden())
		{
			Count++;
		}
	}
	return Count;
}

int32 ATest_PoolAsyncPerformance::CountActiveTraps() const
{
	int32 Count = 0;
	for (TActorIterator<ATrap> It(GetWorld()); It; ++It)
	{
		if (!It->IsActorBeingDestroyed() && !It->IsHidden())
		{
			Count++;
		}
	}
	return Count;
}

void ATest_PoolAsyncPerformance::WriteCsvAndFinish()
{
	// 输出到 Saved 目录，避免污染 Content 并便于性能结果归档 / Write under Saved to avoid Content churn and keep profiling artifacts together
	const FString OutputDirectory = FPaths::ProjectSavedDir() / TEXT("Profiling") / TEXT("RunGamePoolBenchmark");
	IFileManager::Get().MakeDirectory(*OutputDirectory, true);

	const FString OutputPath = OutputDirectory / OutputFileName;
	if (!FFileHelper::SaveStringToFile(BuildCsv(), *OutputPath))
	{
		FinishTest(EFunctionalTestResult::Failed, FString::Printf(TEXT("Failed to write CSV: %s"), *OutputPath));
		return;
	}

	LogStep(ELogVerbosity::Log, FString::Printf(TEXT("Benchmark CSV written: %s"), *OutputPath));
	FinishTest(EFunctionalTestResult::Succeeded, FString::Printf(TEXT("Pool/async benchmark complete: %s"), *OutputPath));
}

FString ATest_PoolAsyncPerformance::BuildCsv() const
{
	// CSV 表头固定，方便多次测试结果用脚本或表格直接比较 / Keep CSV header stable for script and spreadsheet comparison across runs
	FString Csv = TEXT("Scenario,PoolEnabled,AsyncLoading,LoadMs,PreAllocateMs,GameThreadMs,ScenarioWallMs,SpawnActorCount,GeneratedFloorCount,GCCount,ActiveActorCount,ActiveFloorCount,ActiveCoinCount,ActiveTrapCount,PeakUsedPhysicalMB,RecycleMs\n");
	for (const FPoolAsyncBenchmarkRow& Row : Results)
	{
		Csv += FString::Printf(
			TEXT("%s,%s,%s,%.3f,%.3f,%.3f,%.3f,%d,%d,%d,%d,%d,%d,%d,%llu,%.3f\n"),
			*Row.ScenarioName,
			Row.bPoolEnabled ? TEXT("true") : TEXT("false"),
			Row.bAsyncLoading ? TEXT("true") : TEXT("false"),
			Row.LoadMs,
			Row.PreAllocateMs,
			Row.GameThreadMs,
			Row.ScenarioWallMs,
			Row.SpawnActorCount,
			Row.GeneratedFloorCount,
			Row.GCCount,
			Row.ActiveActorCount,
			Row.ActiveFloorCount,
			Row.ActiveCoinCount,
			Row.ActiveTrapCount,
			Row.PeakUsedPhysicalMB,
			Row.RecycleMs);
	}
	return Csv;
}

void ATest_PoolAsyncPerformance::CleanUp()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ScenarioStepTimerHandle);
	}

	if (GarbageCollectCompleteHandle.IsValid())
	{
		FCoreUObjectDelegates::GarbageCollectComplete.Remove(GarbageCollectCompleteHandle);
		GarbageCollectCompleteHandle.Reset();
	}

	if (FloorSubsystem)
	{
		FloorSubsystem->OnFloorSystemReady.RemoveDynamic(this, &ATest_PoolAsyncPerformance::OnFloorSystemReadyCallback);
	}

	CleanupScenarioActors();

	if (FloorSubsystem)
	{
		FloorSubsystem->SetBenchmarkOptions(false, false, INDEX_NONE);
	}

	if (CoinSubsystem)
	{
		CoinSubsystem->SetBenchmarkDisablePool(false);
	}

	Super::CleanUp();
}
