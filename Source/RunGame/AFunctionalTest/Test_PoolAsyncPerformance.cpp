#include "Test_PoolAsyncPerformance.h"
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
	TimeLimit = 120.0f;
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

	FloorSubsystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>();
	CoinSubsystem = GetWorld()->GetSubsystem<URunGameCoinSubsystem>();

	if (!ValidateRequiredConfig())
	{
		return;
	}

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
	CleanupScenarioActors();

	CurrentScenarioIndex++;
	if (!PendingScenarios.IsValidIndex(CurrentScenarioIndex))
	{
		WriteCsvAndFinish();
		return;
	}

	const FPendingScenario& Scenario = PendingScenarios[CurrentScenarioIndex];
	LogStep(ELogVerbosity::Log, FString::Printf(TEXT("Starting benchmark scenario: %s"), *Scenario.Name));

	FloorSubsystem->ClearAllFloors();
	CoinSubsystem->ClearAllCoins();
	FloorSubsystem->ResetBenchmarkStats();
	CoinSubsystem->ResetBenchmarkStats();
	FloorSubsystem->SetBenchmarkOptions(!Scenario.bPoolEnabled, !Scenario.bAsyncLoading, Scenario.bPoolEnabled ? FloorSegmentCount : 0);
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

void ATest_PoolAsyncPerformance::ExecuteCurrentScenario()
{
	if (!PendingScenarios.IsValidIndex(CurrentScenarioIndex))
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Benchmark scenario index became invalid."));
		return;
	}

	const FPendingScenario& Scenario = PendingScenarios[CurrentScenarioIndex];
	FPoolAsyncBenchmarkRow Row;
	Row.ScenarioName = Scenario.Name;
	Row.bPoolEnabled = Scenario.bPoolEnabled;
	Row.bAsyncLoading = Scenario.bAsyncLoading;

	RecordMemorySample(Row);

	RecordMemorySample(Row);

	const FRunGameFloorBenchmarkStats FloorStatsBeforeWorkload = FloorSubsystem->GetBenchmarkStats();
	const FRunGameCoinBenchmarkStats CoinStatsBeforeWorkload = CoinSubsystem->GetBenchmarkStats();
	const int32 PreWorkloadSpawnActorCount = FloorStatsBeforeWorkload.SpawnActorCount + CoinStatsBeforeWorkload.SpawnActorCount;

	const double WorkloadStartSeconds = FPlatformTime::Seconds();
	FloorSubsystem->BenchmarkSpawnFloorChain(FTransform::Identity, FloorSegmentCount);

	Row.GameThreadMs = (FPlatformTime::Seconds() - WorkloadStartSeconds) * 1000.0;
	RecordMemorySample(Row);

	Row.ActiveFloorCount = FloorSubsystem->GetActiveFloorCount();
	Row.ActiveCoinCount = CoinSubsystem->GetActiveCoinCount();
	Row.ActiveTrapCount = CountActiveTraps();
	Row.ActiveActorCount = CountActiveActors();

	const double RecycleStartSeconds = FPlatformTime::Seconds();
	FloorSubsystem->RecycleDistantFloors(FVector(1000000.0f), RecycleDistance);
	Row.RecycleMs = (FPlatformTime::Seconds() - RecycleStartSeconds) * 1000.0;
	RecordMemorySample(Row);

	const FRunGameFloorBenchmarkStats FloorStats = FloorSubsystem->GetBenchmarkStats();
	const FRunGameCoinBenchmarkStats CoinStats = CoinSubsystem->GetBenchmarkStats();
	Row.LoadMs = FloorStats.AsyncLoadMs;
	Row.PreAllocateMs = FloorStats.PreAllocateMs + CoinStats.PreAllocateMs;
	Row.SpawnActorCount = FloorStats.SpawnActorCount + CoinStats.SpawnActorCount - PreWorkloadSpawnActorCount;

	CleanupScenarioActors();
	CollectGarbage(RF_NoFlags);
	Row.GCCount++;
	RecordMemorySample(Row);

	Results.Add(Row);
	LogStep(ELogVerbosity::Log, FString::Printf(
		TEXT("%s: GameThread=%.3fms, SpawnActor=%d, Recycle=%.3fms, ActiveActors=%d, PeakMB=%llu"),
		*Row.ScenarioName,
		Row.GameThreadMs,
		Row.SpawnActorCount,
		Row.RecycleMs,
		Row.ActiveActorCount,
		Row.PeakUsedPhysicalMB));

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
	FString Csv = TEXT("Scenario,PoolEnabled,AsyncLoading,LoadMs,PreAllocateMs,GameThreadMs,SpawnActorCount,GCCount,ActiveActorCount,ActiveFloorCount,ActiveCoinCount,ActiveTrapCount,PeakUsedPhysicalMB,RecycleMs\n");
	for (const FPoolAsyncBenchmarkRow& Row : Results)
	{
		Csv += FString::Printf(
			TEXT("%s,%s,%s,%.3f,%.3f,%.3f,%d,%d,%d,%d,%d,%d,%llu,%.3f\n"),
			*Row.ScenarioName,
			Row.bPoolEnabled ? TEXT("true") : TEXT("false"),
			Row.bAsyncLoading ? TEXT("true") : TEXT("false"),
			Row.LoadMs,
			Row.PreAllocateMs,
			Row.GameThreadMs,
			Row.SpawnActorCount,
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
