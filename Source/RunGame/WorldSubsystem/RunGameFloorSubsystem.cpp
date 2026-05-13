#include "WorldSubsystem/RunGameFloorSubsystem.h"
#include "Actor/Floor/FloorBase.h"
#include "Actor/Component/CoinSpawnerComponent.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

URunGameFloorSubsystem::URunGameFloorSubsystem()
	: bIsLoading(false)
	, bIsInitialized(false)
{
}

void URunGameFloorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Initialized"));
}

void URunGameFloorSubsystem::Deinitialize()
{
	ClearAllFloors();
	Super::Deinitialize();
	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Deinitialized and all floors cleared"));
}

// ===== 初始化（异步加载）=====

void URunGameFloorSubsystem::InitializeFloorSystem(UFloorConfigData* InConfig)
{
	if (!InConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: InitializeFloorSystem called with null config!"));
		return;
	}

	if (bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Already initialized, skipping."));
		return;
	}

	FloorConfig = InConfig;
	PendingFloorEntries = InConfig->FloorClasses;

	StartAsyncLoad();
}

void URunGameFloorSubsystem::StartAsyncLoad()
{
	TArray<FSoftObjectPath> PathsToLoad;
	for (const FFloorClassEntry& Entry : PendingFloorEntries)
	{
		if (!Entry.FloorClass.IsNull())
		{
			PathsToLoad.Add(Entry.FloorClass.ToSoftObjectPath());
		}
	}

	if (PathsToLoad.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: No floor classes to load!"));
		bIsInitialized = true;
		OnFloorSystemReady.Broadcast();
		return;
	}

	bIsLoading = true;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(PathsToLoad,
		FStreamableDelegate::CreateWeakLambda(this, [this]()
		{
			LoadedFloorEntries.Empty();
			for (const FFloorClassEntry& Entry : PendingFloorEntries)
			{
				if (TSubclassOf<AActor> Loaded = Entry.FloorClass.Get())
				{
					FLoadedFloorEntry LoadedEntry;
					LoadedEntry.LoadedClass = Loaded;
					LoadedEntry.SpawnWeight = Entry.SpawnWeight;
					LoadedEntry.InitialGuaranteedCount = Entry.InitialGuaranteedCount;
					LoadedEntry.bEnableCoinSpawn = Entry.bEnableCoinSpawn;
					LoadedFloorEntries.Add(LoadedEntry);
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Loaded %d floor classes"),
				LoadedFloorEntries.Num());

			OnFloorClassesLoaded();
		})
	);
}

void URunGameFloorSubsystem::OnFloorClassesLoaded()
{
	bIsLoading = false;

	const int32 AllocateCount = FloorConfig ? FloorConfig->PreAllocateCount : 1;
	for (int32 i = 0; i < AllocateCount; ++i)
	{
		TSubclassOf<AActor> RandomClass = WeightedRandomSelectFloorClass();
		if (RandomClass)
		{
			if (AFloorBase* NewFloor = CreateNewFloorActor(RandomClass))
			{
				NewFloor->SetActorHiddenInGame(true);
				NewFloor->SetActorEnableCollision(false);
				PooledFloorsMap.FindOrAdd(RandomClass).Add(NewFloor);
			}
		}
	}

	bIsInitialized = true;

	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Pre-allocated %d floors across %d types"),
		GetPooledFloorCount(), PooledFloorsMap.Num());

	OnFloorSystemReady.Broadcast();
}

// ===== 地板生成 =====

void URunGameFloorSubsystem::SpawnInitialFloors(const FTransform& StartTransform)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: Cannot spawn initial floors, not initialized!"));
		return;
	}

	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: No world available!"));
		return;
	}

	if (!FloorConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: No FloorConfig available!"));
		return;
	}

	NextSpawnTransform = StartTransform;
	FVector InLocation = NextSpawnTransform.GetLocation();
	InLocation += FloorConfig->SpawnStartOffset;
	NextSpawnTransform.SetLocation(InLocation);

	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Spawning initial floors from location: %s"), *InLocation.ToString());

	// Spawn guaranteed-count floors first (in entry order)
	for (const FLoadedFloorEntry& Entry : LoadedFloorEntries)
	{
		for (int32 i = 0; i < Entry.InitialGuaranteedCount; ++i)
		{
			RequestFloorAt(Entry.LoadedClass, NextSpawnTransform.GetLocation(), NextSpawnTransform.Rotator());
		}
	}

	// Spawn random floors
	for (int32 i = 0; i < FloorConfig->InitialRandomFloorCount; ++i)
	{
		RequestNextFloor();
	}

	const int32 TotalGuaranteed = [this]()
	{
		int32 Sum = 0;
		for (const FLoadedFloorEntry& E : LoadedFloorEntries) { Sum += E.InitialGuaranteedCount; }
		return Sum;
	}();

	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Initial floor chain generated: %d guaranteed + %d random = %d floors"),
		TotalGuaranteed, FloorConfig->InitialRandomFloorCount, TotalGuaranteed + FloorConfig->InitialRandomFloorCount);
}

AFloorBase* URunGameFloorSubsystem::RequestNextFloor()
{
	if (!bIsInitialized || !GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: Cannot request next floor, not initialized or no world!"));
		return nullptr;
	}

	TSubclassOf<AActor> SelectedClass = WeightedRandomSelectFloorClass();
	if (!SelectedClass)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: No floor classes available!"));
		return nullptr;
	}

	return RequestFloorAt(SelectedClass, NextSpawnTransform.GetLocation(), NextSpawnTransform.Rotator());
}

AFloorBase* URunGameFloorSubsystem::RequestFloorAt(
		TSubclassOf<AActor> InClass,
		const FVector& Location,
		const FRotator& Rotation)
{
	if (!InClass || !GetWorld())
	{
		return nullptr;
	}

	AFloorBase* Floor = AcquireFloorFromPool(InClass);
	if (!Floor)
	{
		Floor = CreateNewFloorActor(InClass);
		if (!Floor)
		{
			return nullptr;
		}
	}

	Floor->SetActorLocation(Location);
	Floor->SetActorRotation(Rotation);
	Floor->SetActorScale3D(FloorConfig ? FloorConfig->FloorScale : FVector(10.0f));
	Floor->SetActorHiddenInGame(false);
	Floor->SetActorEnableCollision(true);
	Floor->UpdateComponentTransforms();

	BindFloorDelegates(Floor);

	NextSpawnTransform = Floor->GetAttachToTransform(Location);

	ActiveFloors.Add(Floor);

	if (UCoinSpawnerComponent* Spawner = Floor->CoinSpawnerComponent)
	{
		const FLoadedFloorEntry* Entry = FindLoadedEntry(InClass);
		if (Entry && Entry->bEnableCoinSpawn && FloorConfig)
		{
			Spawner->ApplyConfig(FloorConfig->GlobalCoinConfig);
			Spawner->SpawnCoins();
		}
	}

	return Floor;
}

// ===== 地板回收 =====

void URunGameFloorSubsystem::ReturnFloor(AFloorBase* Floor)
{
	if (!Floor)
	{
		return;
	}

	if (UCoinSpawnerComponent* Spawner = Floor->CoinSpawnerComponent)
	{
		Spawner->DespawnCoins();
	}

	ActiveFloors.RemoveSwap(Floor);
	UnbindFloorDelegates(Floor);

	Floor->SetActorHiddenInGame(true);
	Floor->SetActorEnableCollision(false);

	PooledFloorsMap.FindOrAdd(Floor->GetClass()).Add(Floor);
}

void URunGameFloorSubsystem::HideAllActiveFloors()
{
	TArray<TObjectPtr<AFloorBase>> FloorsToHide = ActiveFloors;
	for (AFloorBase* Floor : FloorsToHide)
	{
		ReturnFloor(Floor);
	}
	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: All %d active floors hidden and returned to pool"),
		FloorsToHide.Num());
}

void URunGameFloorSubsystem::RecycleDistantFloors(const FVector& PlayerLocation, float MaxDistance)
{
	if (MaxDistance <= 0.0f)
	{
		MaxDistance = FloorConfig ? FloorConfig->RecycleMaxDistance : 3000.0f;
	}

	TArray<AFloorBase*> ToRecycle;
	const float MaxDistSq = MaxDistance * MaxDistance;
	for (AFloorBase* Floor : ActiveFloors)
	{
		if (!Floor)
		{
			continue;
		}

		if (FVector::DistSquared(Floor->GetActorLocation(), PlayerLocation) > MaxDistSq)
		{
			ToRecycle.Add(Floor);
		}
	}

	for (AFloorBase* Floor : ToRecycle)
	{
		ReturnFloor(Floor);
	}

	if (ToRecycle.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Recycled %d distant floors"), ToRecycle.Num());
	}
}

void URunGameFloorSubsystem::ClearAllFloors()
{
	for (auto& Pair : PooledFloorsMap)
	{
		for (AFloorBase* Floor : Pair.Value)
		{
			if (IsValid(Floor))
			{
				Floor->Destroy();
			}
		}
	}
	PooledFloorsMap.Empty();

	for (AFloorBase* Floor : ActiveFloors)
	{
		if (Floor && Floor->IsValidLowLevel())
		{
			Floor->Destroy();
		}
	}
	ActiveFloors.Empty();

	bIsInitialized = false;
	bIsLoading = false;

	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: All floors cleared"));
}

// ===== 查询 =====

int32 URunGameFloorSubsystem::GetPooledFloorCount() const
{
	int32 Total = 0;
	for (const auto& Pair : PooledFloorsMap)
	{
		Total += Pair.Value.Num();
	}
	return Total;
}

// ===== 内部方法 =====

const FLoadedFloorEntry* URunGameFloorSubsystem::FindLoadedEntry(TSubclassOf<AActor> InClass) const
{
	for (const FLoadedFloorEntry& Entry : LoadedFloorEntries)
	{
		if (Entry.LoadedClass == InClass)
		{
			return &Entry;
		}
	}
	return nullptr;
}

TSubclassOf<AActor> URunGameFloorSubsystem::WeightedRandomSelectFloorClass()
{
	if (LoadedFloorEntries.Num() == 0)
	{
		return nullptr;
	}

	float TotalWeight = 0.0f;
	for (const FLoadedFloorEntry& Entry : LoadedFloorEntries)
	{
		TotalWeight += FMath::Max(0.0f, Entry.SpawnWeight);
	}

	if (TotalWeight <= 0.0f)
	{
		// All weights are zero: fall back to uniform random
		const int32 Index = FMath::RandRange(0, LoadedFloorEntries.Num() - 1);
		return LoadedFloorEntries[Index].LoadedClass;
	}

	float Random = FMath::FRandRange(0.0f, TotalWeight);
	float Accumulated = 0.0f;
	for (const FLoadedFloorEntry& Entry : LoadedFloorEntries)
	{
		Accumulated += FMath::Max(0.0f, Entry.SpawnWeight);
		if (Random <= Accumulated)
		{
			return Entry.LoadedClass;
		}
	}

	return LoadedFloorEntries.Last().LoadedClass;
}

AFloorBase* URunGameFloorSubsystem::AcquireFloorFromPool(TSubclassOf<AActor> InClass)
{
	if (!InClass)
	{
		return nullptr;
	}

	TArray<AFloorBase*>* SubPool = PooledFloorsMap.Find(InClass);
	if (SubPool && SubPool->Num() > 0)
	{
		return SubPool->Pop();
	}
	return nullptr;
}

AFloorBase* URunGameFloorSubsystem::CreateNewFloorActor(TSubclassOf<AActor> InClass)
{
	if (!InClass || !GetWorld())
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector OffScreenLocation = FloorConfig ? FloorConfig->PoolHideLocation : FVector(0.0f, 0.0f, -100000.0f);
	AFloorBase* NewFloor = GetWorld()->SpawnActor<AFloorBase>(InClass, OffScreenLocation, FRotator::ZeroRotator, SpawnParams);

	if (!NewFloor)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: Failed to spawn floor actor of class %s"), *InClass->GetName());
	}

	return NewFloor;
}

void URunGameFloorSubsystem::BindFloorDelegates(AFloorBase* Floor)
{
	if (!Floor)
	{
		return;
	}

	Floor->OnPlayerEntered.AddDynamic(this, &URunGameFloorSubsystem::OnFloorPlayerEntered);
	Floor->OnRecycleRequested.AddDynamic(this, &URunGameFloorSubsystem::OnFloorRecycleRequested);
}

void URunGameFloorSubsystem::UnbindFloorDelegates(AFloorBase* Floor)
{
	if (!Floor)
	{
		return;
	}

	Floor->OnPlayerEntered.RemoveDynamic(this, &URunGameFloorSubsystem::OnFloorPlayerEntered);
	Floor->OnRecycleRequested.RemoveDynamic(this, &URunGameFloorSubsystem::OnFloorRecycleRequested);
}

void URunGameFloorSubsystem::OnFloorPlayerEntered(AFloorBase* Floor)
{
	RequestNextFloor();
}

void URunGameFloorSubsystem::OnFloorRecycleRequested(AFloorBase* Floor)
{
	ReturnFloor(Floor);
}
