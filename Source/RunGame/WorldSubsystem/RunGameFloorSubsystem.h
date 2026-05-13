#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DataAssets/FloorConfigData.h"
#include "RunGameFloorSubsystem.generated.h"

class AFloorBase;
struct FFloorClassEntry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorSystemReadyDelegate);

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

	// Object pool: per-type sub-pools
	TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap;

	UPROPERTY()
	TArray<TObjectPtr<AFloorBase>> ActiveFloors;

	// Tracked position for next floor spawn
	FTransform NextSpawnTransform;
};
