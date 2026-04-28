// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameFloorSubsystem.generated.h"

class AFloorBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorSystemReadyDelegate);

UCLASS()
class RUNGAME_API URunGameFloorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Constructs the floor subsystem with default pool and spawn settings */
	URunGameFloorSubsystem();

	/** Begins async loading of floor classes and pre-allocates the floor pool */

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void InitializeFloorSystem(
		const TArray<TSoftClassPtr<AActor>>& InStraightClasses,
		const TArray<TSoftClassPtr<AActor>>& InTurnClasses,
		int32 InPreAllocateCount = 10
	);

	/** Spawns the initial chain of straight and random floor segments */

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void SpawnInitialFloors(const FTransform& StartTransform, int32 StraightCount = 5, int32 RandomCount = 15);

	/** Acquires the next random floor from the pool at the tracked spawn position */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	AFloorBase* RequestNextFloor();

	/** Acquires a floor of the specified class from the pool at the given location */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	AFloorBase* RequestFloorAt(TSubclassOf<AActor> InClass, const FVector& Location, const FRotator& Rotation);

	/** Returns a floor actor to the pool, hiding and deactivating it */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void ReturnFloor(AFloorBase* Floor);

	/** Recycles active floors farther than MaxDistance from the player */
	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void RecycleDistantFloors(const FVector& PlayerLocation, float MaxDistance = 3000.0f);

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

	/** Delegate broadcast when floor subsystem finishes async loading */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|FloorSystem")
	FOnFloorSystemReadyDelegate OnFloorSystemReady;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/** Clears all floors and cleans up on subsystem teardown */
	virtual void Deinitialize() override;

private:
	/** Randomly selects a floor class (80% straight / 20% turn) */

	TSubclassOf<AActor> RandomSelectFloorClass();

	/** Pops a floor of given class from pool; returns nullptr if empty */
	AFloorBase* AcquireFloorFromPool(TSubclassOf<AActor> InClass);

	/** Spawns a new floor actor at an off-screen location */
	AFloorBase* CreateNewFloorActor(TSubclassOf<AActor> InClass);

	/** Initiates async loading of floor classes via the asset manager */
	void StartAsyncLoad();

	/** Callback when async load finishes; pre-allocates the pool */
	UFUNCTION()
	void OnFloorClassesLoaded();

	// Async-load configuration data

	TArray<TSoftClassPtr<AActor>> StraightClassPtrs;

	TArray<TSoftClassPtr<AActor>> TurnClassPtrs;

	UPROPERTY()
	TArray<TSubclassOf<AActor>> LoadedStraightClasses;

	UPROPERTY()
	TArray<TSubclassOf<AActor>> LoadedTurnClasses;

	int32 PreAllocateCount;

	bool bIsLoading;

	bool bIsInitialized;

	// Object pool: per-type sub-pools

	TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap;

	UPROPERTY()
	TArray<TObjectPtr<AFloorBase>> ActiveFloors;

	// Tracked position for next floor spawn

	FTransform NextSpawnTransform;
};
