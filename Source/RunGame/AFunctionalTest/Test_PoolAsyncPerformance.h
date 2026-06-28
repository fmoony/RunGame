#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "DataAssets/FloorConfigData.h"
#include "Test_PoolAsyncPerformance.generated.h"

class ATrap;
class URunGameCoinSubsystem;
class URunGameFloorSubsystem;

USTRUCT()
struct FPoolAsyncBenchmarkRow
{
	GENERATED_BODY()

	FString ScenarioName;
	bool bPoolEnabled = true;
	bool bAsyncLoading = true;
	double LoadMs = 0.0;
	double PreAllocateMs = 0.0;
	double GameThreadMs = 0.0;
	double RecycleMs = 0.0;
	int32 SpawnActorCount = 0;
	int32 GCCount = 0;
	int32 ActiveActorCount = 0;
	int32 ActiveFloorCount = 0;
	int32 ActiveCoinCount = 0;
	int32 ActiveTrapCount = 0;
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

	UPROPERTY(EditAnywhere, Category = "Benchmark|Run")
	bool bAutoRunOnBeginPlay = false;

	UPROPERTY(EditAnywhere, Category = "Benchmark|Config")
	TObjectPtr<UFloorConfigData> TestFloorConfig;

	UPROPERTY(EditAnywhere, Category = "Benchmark|Load")
	bool bIncludeSynchronousLoadComparison = true;

	UPROPERTY(EditAnywhere, Category = "Benchmark|Counts", meta = (ClampMin = "1"))
	int32 FloorSegmentCount = 200;

	UPROPERTY(EditAnywhere, Category = "Benchmark|Recycle")
	float RecycleDistance = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Benchmark|Output")
	FString OutputFileName = TEXT("PoolAsyncBenchmark.csv");

protected:
	virtual void BeginPlay() override;
	virtual void StartTest() override;
	virtual void CleanUp() override;

private:
	struct FPendingScenario
	{
		FString Name;
		bool bPoolEnabled = true;
		bool bAsyncLoading = true;
	};

	void StartNextScenario();

	UFUNCTION()
	void OnFloorSystemReadyCallback();

	void ExecuteCurrentScenario();
	void CleanupScenarioActors();
	void RecordMemorySample(FPoolAsyncBenchmarkRow& Row) const;
	int32 CountActiveActors() const;
	int32 CountActiveTraps() const;
	bool ValidateRequiredConfig();
	void WriteCsvAndFinish();
	FString BuildCsv() const;

	UPROPERTY()
	TObjectPtr<URunGameFloorSubsystem> FloorSubsystem;

	UPROPERTY()
	TObjectPtr<URunGameCoinSubsystem> CoinSubsystem;

	TArray<FPendingScenario> PendingScenarios;
	TArray<FPoolAsyncBenchmarkRow> Results;
	int32 CurrentScenarioIndex = INDEX_NONE;
	bool bBenchmarkStarted = false;
};
