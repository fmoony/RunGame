#include "WorldSubsystem/RunGameFloorSubsystem.h"
#include "Actor/Floor/FloorBase.h"
#include "Actor/Component/CoinSpawnerComponent.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "State/PlayerRuntimeState.h"
#include "HAL/PlatformTime.h"
#include "RunGame.h"


URunGameFloorSubsystem::URunGameFloorSubsystem()
	: bIsLoading(false)
	, bIsInitialized(false)
{
}

void URunGameFloorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Initialized"));
}

void URunGameFloorSubsystem::Deinitialize()
{
	ClearAllFloors();
	Super::Deinitialize();
	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Deinitialized and all floors cleared"));
}

// ===== 初始化（异步加载）=====

void URunGameFloorSubsystem::InitializeFloorSystem(UFloorConfigData* InConfig)
{
	if (!InConfig)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: InitializeFloorSystem called with null config!"));
		return;
	}

	if (bIsInitialized)
	{
		UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Already initialized, skipping."));
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
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: No floor classes to load!"));
		bIsInitialized = true;
		OnFloorSystemReady.Broadcast();
		return;
	}

	bIsLoading = true;
	AsyncLoadStartSeconds = FPlatformTime::Seconds();

	if (bBenchmarkUseSynchronousLoad)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		for (const FSoftObjectPath& Path : PathsToLoad)
		{
			Streamable.LoadSynchronous(Path);
		}
		BuildLoadedFloorEntries();
		OnFloorClassesLoaded();
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(PathsToLoad,
		FStreamableDelegate::CreateWeakLambda(this, [this]()
		{
			BuildLoadedFloorEntries();
			OnFloorClassesLoaded();
		})
	);
}

void URunGameFloorSubsystem::BuildLoadedFloorEntries()
{
	LoadedFloorEntries.Empty();
	for (const FFloorClassEntry& Entry : PendingFloorEntries)
	{
		UClass* LoadedClass = Entry.FloorClass.Get();
		if (!IsValid(LoadedClass))
		{
			UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Skipping unresolved floor class in config."));
			continue;
		}

		if (!LoadedClass->IsChildOf(AFloorBase::StaticClass()))
		{
			UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: Config class %s is not an AFloorBase subclass."),
				*GetNameSafe(LoadedClass));
			continue;
		}

		FLoadedFloorEntry LoadedEntry;
		LoadedEntry.LoadedClass = LoadedClass;
		LoadedEntry.SpawnWeight = Entry.SpawnWeight;
		LoadedEntry.InitialGuaranteedCount = Entry.InitialGuaranteedCount;
		LoadedEntry.bEnableCoinSpawn = Entry.bEnableCoinSpawn;
		LoadedFloorEntries.Add(LoadedEntry);
	}

	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Loaded %d valid floor classes"),
		LoadedFloorEntries.Num());
}

void URunGameFloorSubsystem::OnFloorClassesLoaded()
{
	bIsLoading = false;
	BenchmarkStats.AsyncLoadMs = (FPlatformTime::Seconds() - AsyncLoadStartSeconds) * 1000.0;

	if (LoadedFloorEntries.Num() == 0)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: No valid AFloorBase classes loaded."));
		bIsInitialized = false;
		OnFloorSystemReady.Broadcast();
		return;
	}

	const double PreAllocateStartSeconds = FPlatformTime::Seconds();
	const int32 AllocateCount = BenchmarkPreAllocateOverride >= 0
		? BenchmarkPreAllocateOverride
		: (FloorConfig ? FloorConfig->PreAllocateCount : 1);
	for (int32 i = 0; i < AllocateCount; ++i)
	{
		if (bBenchmarkDisablePool)
		{
			break;
		}

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
	BenchmarkStats.PreAllocateMs = (FPlatformTime::Seconds() - PreAllocateStartSeconds) * 1000.0;

	bIsInitialized = true;

	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Pre-allocated %d floors across %d types"),
		GetPooledFloorCount(), PooledFloorsMap.Num());

	OnFloorSystemReady.Broadcast();
}

// ===== 地板生成 =====

void URunGameFloorSubsystem::SpawnInitialFloors(const FTransform& StartTransform)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: Cannot spawn initial floors, not initialized!"));
		return;
	}

	if (!GetWorld())
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: No world available!"));
		return;
	}

	if (!FloorConfig)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: No FloorConfig available!"));
		return;
	}

	NextSpawnTransform = StartTransform;
	FVector InLocation = NextSpawnTransform.GetLocation();
	InLocation += FloorConfig->SpawnStartOffset;
	NextSpawnTransform.SetLocation(InLocation);

	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Spawning initial floors from location: %s"), *InLocation.ToString());

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

	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Initial floor chain generated: %d guaranteed + %d random = %d floors"),
		TotalGuaranteed, FloorConfig->InitialRandomFloorCount, TotalGuaranteed + FloorConfig->InitialRandomFloorCount);
}

void URunGameFloorSubsystem::BeginBenchmarkFloorChain(const FTransform& StartTransform)
{
	if (!bIsInitialized || !FloorConfig)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: BeginBenchmarkFloorChain called before initialization."));
		return;
	}

	NextSpawnTransform = StartTransform;
	FVector InLocation = NextSpawnTransform.GetLocation();
	InLocation += FloorConfig->SpawnStartOffset;
	NextSpawnTransform.SetLocation(InLocation);
}

void URunGameFloorSubsystem::BenchmarkSpawnFloorChain(const FTransform& StartTransform, int32 FloorCount)
{
	if (!bIsInitialized || !FloorConfig)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: BenchmarkSpawnFloorChain called before initialization."));
		return;
	}

	BeginBenchmarkFloorChain(StartTransform);

	for (int32 i = 0; i < FloorCount; ++i)
	{
		RequestNextFloor();
	}
}

AFloorBase* URunGameFloorSubsystem::RequestNextFloor()
{
	if (!bIsInitialized || !GetWorld())
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: Cannot request next floor, not initialized or no world!"));
		return nullptr;
	}

	TSubclassOf<AActor> SelectedClass = WeightedRandomSelectFloorClass();
	if (!SelectedClass)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: No floor classes available!"));
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

	// 通知子 Actor（Trap 等）Floor 已从池子激活 Notify child actors (Trap etc.) that Floor was activated from pool
	Floor->OnFloorActivated.Broadcast(Floor);

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

	// 通知子 Actor（Trap 等）Floor 即将归还池子 Notify child actors (Trap etc.) that Floor is returning to pool
	Floor->OnFloorDeactivated.Broadcast(Floor);

	ActiveFloors.RemoveSwap(Floor);
	UnbindFloorDelegates(Floor);

	Floor->SetActorHiddenInGame(true);
	Floor->SetActorEnableCollision(false);

	if (bBenchmarkDisablePool)
	{
		Floor->Destroy();
		return;
	}

	PooledFloorsMap.FindOrAdd(Floor->GetClass()).Add(Floor);
}

void URunGameFloorSubsystem::HideAllActiveFloors()
{
	TArray<TObjectPtr<AFloorBase>> FloorsToHide = ActiveFloors;
	for (AFloorBase* Floor : FloorsToHide)
	{
		ReturnFloor(Floor);
	}
	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: All %d active floors hidden and returned to pool"),
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
		UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: Recycled %d distant floors"), ToRecycle.Num());
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

	FloorConfig = nullptr;
	PendingFloorEntries.Empty();
	LoadedFloorEntries.Empty();
	bIsInitialized = false;
	bIsLoading = false;
	AsyncLoadStartSeconds = 0.0;

	UE_LOG(LogRunGame, Warning, TEXT("RunGameFloorSubsystem: All floors cleared"));
}

void URunGameFloorSubsystem::SetBenchmarkOptions(
	bool bInDisablePool,
	bool bInUseSynchronousLoad,
	int32 InPreAllocateOverride)
{
	bBenchmarkDisablePool = bInDisablePool;
	bBenchmarkUseSynchronousLoad = bInUseSynchronousLoad;
	BenchmarkPreAllocateOverride = InPreAllocateOverride;
}

void URunGameFloorSubsystem::ResetBenchmarkStats()
{
	BenchmarkStats = FRunGameFloorBenchmarkStats();
	AsyncLoadStartSeconds = 0.0;
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

	if (bBenchmarkDisablePool)
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

	UClass* FloorClass = InClass.Get();
	if (!IsValid(FloorClass) || !FloorClass->IsChildOf(AFloorBase::StaticClass()))
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: Refusing to spawn invalid floor class %s."),
			*GetNameSafe(FloorClass));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector OffScreenLocation = FloorConfig ? FloorConfig->PoolHideLocation : FVector(0.0f, 0.0f, -100000.0f);
	AFloorBase* NewFloor = GetWorld()->SpawnActor<AFloorBase>(InClass, OffScreenLocation, FRotator::ZeroRotator, SpawnParams);
	BenchmarkStats.SpawnActorCount++;

	if (!NewFloor)
	{
		UE_LOG(LogRunGame, Error, TEXT("RunGameFloorSubsystem: Failed to spawn floor actor of class %s"), *InClass->GetName());
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
	// 死亡/无碰撞状态不触发新地板 Dead/CoyoteTime don't trigger new floors
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		const ERunGameCharacterState State = PRS->GetCharacterState();
		if (State == ERunGameCharacterState::Dead || State == ERunGameCharacterState::CoyoteTime)
		{
			return;
		}
	}

	RequestNextFloor();
}

void URunGameFloorSubsystem::OnFloorRecycleRequested(AFloorBase* Floor)
{
	ReturnFloor(Floor);
}
