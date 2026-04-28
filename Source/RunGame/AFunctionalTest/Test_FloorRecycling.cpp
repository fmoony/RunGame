#include "Test_FloorRecycling.h"
#include "WorldSubsystem/RunGameFloorSubsystem.h"
#include "Actor/Floor/FloorBase.h"
#include "Engine/World.h"

ATest_FloorRecycling::ATest_FloorRecycling()
{
	TimeLimit = 30.0f;
}

void ATest_FloorRecycling::StartTest()
{
	FloorSubsystem = GetWorld()->GetSubsystem<URunGameFloorSubsystem>();

	if (!FloorSubsystem)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("URunGameFloorSubsystem not found in world."));
		return;
	}

	if (StraightFloorClasses.Num() == 0 && TurnFloorClasses.Num() == 0)
	{
		FinishTest(EFunctionalTestResult::Failed,
			TEXT("No floor classes configured. Set StraightFloorClasses or TurnFloorClasses on the test actor."));
		return;
	}

	LogStep(ELogVerbosity::Log,FString::Printf(TEXT("Initializing floor system with %d straight + %d turn classes, pre-allocate: %d"),
		StraightFloorClasses.Num(), TurnFloorClasses.Num(), PreAllocateCount));

	FloorSubsystem->OnFloorSystemReady.AddDynamic(this, &ATest_FloorRecycling::OnFloorSystemReadyCallback);
	FloorSubsystem->InitializeFloorSystem(StraightFloorClasses, TurnFloorClasses, PreAllocateCount);
}

void ATest_FloorRecycling::OnFloorSystemReadyCallback()
{
	if (FloorSubsystem)
	{
		FloorSubsystem->OnFloorSystemReady.RemoveDynamic(this, &ATest_FloorRecycling::OnFloorSystemReadyCallback);
	}

	ExecuteFloorRecyclingTest();
}

void ATest_FloorRecycling::ExecuteFloorRecyclingTest()
{
	if (!FloorSubsystem)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("FloorSubsystem became null."));
		return;
	}

	// Step 1: Verify pre-allocation
	const int32 PooledBefore = FloorSubsystem->GetPooledFloorCount();
	LogStep(ELogVerbosity::Log,FString::Printf(TEXT("Pre-allocation complete: %d floors in pool"), PooledBefore));
	AssertTrue(PooledBefore > 0, TEXT("Pool should contain pre-allocated floors"));

	// Step 2: Spawn initial floor chain
	LogStep(ELogVerbosity::Log,FString::Printf(TEXT("Spawning initial floors: %d straight + %d random"),
		InitialStraightCount, InitialRandomCount));

	FloorSubsystem->SpawnInitialFloors(FTransform::Identity, InitialStraightCount, InitialRandomCount);

	const int32 ExpectedActive = InitialStraightCount + InitialRandomCount;
	const int32 ActualActive = FloorSubsystem->GetActiveFloorCount();

	LogStep(ELogVerbosity::Log,FString::Printf(TEXT("Active floors: %d (expected %d)"), ActualActive, ExpectedActive));
	AssertEqual_Int(ActualActive, ExpectedActive,
		TEXT("Active floor count should match initial spawn request"));

	// Step 3: Verify next spawn transform was advanced
	const FTransform NextTransform = FloorSubsystem->GetNextSpawnTransform();
	AssertTrue(!NextTransform.GetLocation().IsNearlyZero(),
		TEXT("NextSpawnTransform should be updated after spawning"));

	// Step 4: Test floor recycling — recycle a single floor
	if (ActualActive > 0)
	{
		LogStep(ELogVerbosity::Log, TEXT("Testing single-floor recycling"));
		FloorSubsystem->RecycleDistantFloors(FVector::ZeroVector, 1.0f);

		const int32 AfterRecycle = FloorSubsystem->GetActiveFloorCount();
		const int32 PoolAfterRecycle = FloorSubsystem->GetPooledFloorCount();
		LogStep(ELogVerbosity::Log,FString::Printf(TEXT("After recycle: %d active, %d pooled"), AfterRecycle, PoolAfterRecycle));

		AssertTrue(AfterRecycle < ActualActive,
			TEXT("Active floors should decrease after recycling distant floors"));
		AssertTrue(PoolAfterRecycle > PooledBefore,
			TEXT("Pool should grow after recycling floors back"));
	}

	// Step 5: Request a new floor from recycled pool
	if (FloorSubsystem->GetPooledFloorCount() > 0)
	{
		LogStep(ELogVerbosity::Log, TEXT("Requesting floor from recycled pool"));
		AFloorBase* NewFloor = FloorSubsystem->RequestNextFloor();
		AssertTrue(NewFloor != nullptr,
			TEXT("Should successfully request a floor when pool has entries"));
		AssertTrue(!NewFloor->IsHidden(),
			TEXT("Acquired floor should be visible"));
	}

	// Step 6: Test HideAllActiveFloors
	LogStep(ELogVerbosity::Log, TEXT("Hiding all active floors"));
	FloorSubsystem->HideAllActiveFloors();
	AssertEqual_Int(FloorSubsystem->GetActiveFloorCount(), 0,
		TEXT("Active floor count should be 0 after hiding all"));

	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Floor recycling test passed."));
}

void ATest_FloorRecycling::CleanUp()
{
	if (FloorSubsystem)
	{
		FloorSubsystem->OnFloorSystemReady.RemoveDynamic(this, &ATest_FloorRecycling::OnFloorSystemReadyCallback);
		FloorSubsystem->ClearAllFloors();
	}

	Super::CleanUp();
}
