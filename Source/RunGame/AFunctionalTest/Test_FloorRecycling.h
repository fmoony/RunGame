#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "Test_FloorRecycling.generated.h"

class URunGameFloorSubsystem;
class AFloorBase;

UCLASS()
class RUNGAME_API ATest_FloorRecycling : public AFunctionalTest
{
	GENERATED_BODY()

public:
	/** Constructs the floor recycling functional test with 30s time limit */
	ATest_FloorRecycling();

	UPROPERTY(EditAnywhere, Category = "Test|FloorConfig")
	TArray<TSoftClassPtr<AActor>> StraightFloorClasses;

	UPROPERTY(EditAnywhere, Category = "Test|FloorConfig")
	TArray<TSoftClassPtr<AActor>> TurnFloorClasses;

	UPROPERTY(EditAnywhere, Category = "Test|FloorConfig")
	int32 PreAllocateCount = 5;

	UPROPERTY(EditAnywhere, Category = "Test|SpawnConfig")
	int32 InitialStraightCount = 3;

	UPROPERTY(EditAnywhere, Category = "Test|SpawnConfig")
	int32 InitialRandomCount = 5;

	UPROPERTY(EditAnywhere, Category = "Test|RecycleConfig")
	float RecycleDistance = 2000.0f;

protected:
	/** Initializes the floor subsystem and binds ready callback */
	virtual void StartTest() override;
	/** Unbinds callbacks and clears all floors on teardown */
	virtual void CleanUp() override;

private:
	/** Called when async loading finishes; triggers test execution */
	void OnFloorSystemReadyCallback();
	/** Runs recycling test: pre-allocation, spawning, recycling, re-acquisition */
	void ExecuteFloorRecyclingTest();

	URunGameFloorSubsystem* FloorSubsystem;
};
