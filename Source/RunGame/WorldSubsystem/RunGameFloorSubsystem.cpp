#include "WorldSubsystem/RunGameFloorSubsystem.h"
#include "Actor/Floor/FloorBase.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

URunGameFloorSubsystem::URunGameFloorSubsystem()
	: PreAllocateCount(10)
	, bIsLoading(false)
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

// ===== 初始化（异步加载） =====

void URunGameFloorSubsystem::InitializeFloorSystem(
	const TArray<TSoftClassPtr<AActor>>& InStraightClasses,
	const TArray<TSoftClassPtr<AActor>>& InTurnClasses,
	int32 InPreAllocateCount)
{
	if (bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Already initialized, skipping."));
		return;
	}

	StraightClassPtrs = InStraightClasses;
	TurnClassPtrs = InTurnClasses;
	PreAllocateCount = FMath::Max(1, InPreAllocateCount);

	StartAsyncLoad();
}

void URunGameFloorSubsystem::StartAsyncLoad()
{
	TArray<FSoftObjectPath> PathsToLoad;
	for (const auto& SoftClass : StraightClassPtrs)
	{
		if (!SoftClass.IsNull())
		{
			PathsToLoad.Add(SoftClass.ToSoftObjectPath());
		}
	}
	for (const auto& SoftClass : TurnClassPtrs)
	{
		if (!SoftClass.IsNull())
		{
			PathsToLoad.Add(SoftClass.ToSoftObjectPath());
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
			LoadedStraightClasses.Empty();
			for (const auto& SoftClass : StraightClassPtrs)
			{
				if (TSubclassOf<AActor> Loaded = SoftClass.Get())
				{
					LoadedStraightClasses.Add(Loaded);
				}
			}

			LoadedTurnClasses.Empty();
			for (const auto& SoftClass : TurnClassPtrs)
			{
				if (TSubclassOf<AActor> Loaded = SoftClass.Get())
				{
					LoadedTurnClasses.Add(Loaded);
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Loaded %d straight + %d turn floor classes"),
				LoadedStraightClasses.Num(), LoadedTurnClasses.Num());

			OnFloorClassesLoaded();
		})
	);
}

void URunGameFloorSubsystem::OnFloorClassesLoaded()
{
	bIsLoading = false;

	for (int32 i = 0; i < PreAllocateCount; ++i)
	{
		TSubclassOf<AActor> RandomClass = RandomSelectFloorClass();
		if (RandomClass)
		{
			if (AFloorBase* NewFloor = CreateNewFloorActor(RandomClass))
			{
				NewFloor->SetActorLocation(FVector(0.0f, 0.0f, -100000.0f));
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

void URunGameFloorSubsystem::SpawnInitialFloors(const FTransform& StartTransform, int32 StraightCount, int32 RandomCount)
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

	NextSpawnTransform = StartTransform;
	FVector InLocation = NextSpawnTransform.GetLocation();
	InLocation.Y += 200.0f;
	InLocation.Z -= 200.0f;
	InLocation.X -= 200.0f;
	NextSpawnTransform.SetLocation(InLocation);

	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Spawning initial floors from location: %s"), *InLocation.ToString());

	for (int32 i = 0; i < StraightCount; ++i)
	{
		if (LoadedStraightClasses.Num() > 0)
		{
			RequestFloorAt(LoadedStraightClasses[0], NextSpawnTransform.GetLocation(), NextSpawnTransform.Rotator());
		}
	}

	for (int32 i = 0; i < RandomCount; ++i)
	{
		RequestNextFloor();
	}

	UE_LOG(LogTemp, Warning, TEXT("RunGameFloorSubsystem: Initial floor chain generated: %d floors"), StraightCount + RandomCount);
}

AFloorBase* URunGameFloorSubsystem::RequestNextFloor()
{
	if (!bIsInitialized || !GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: Cannot request next floor, not initialized or no world!"));
		return nullptr;
	}

	TSubclassOf<AActor> SelectedClass = RandomSelectFloorClass();
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
	Floor->SetActorScale3D(FVector(10.0f));
	Floor->SetActorHiddenInGame(false);
	Floor->SetActorEnableCollision(true);
	Floor->UpdateComponentTransforms();

	NextSpawnTransform = Floor->GetAttachToTransform(Location);

	ActiveFloors.Add(Floor);

	return Floor;
}

// ===== 地板回收 =====

void URunGameFloorSubsystem::ReturnFloor(AFloorBase* Floor)
{
	if (!Floor)
	{
		return;
	}

	ActiveFloors.Remove(Floor);

	Floor->SetActorLocation(FVector(0.0f, 0.0f, -100000.0f));
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
	TArray<AFloorBase*> ToRecycle;
	for (AFloorBase* Floor : ActiveFloors)
	{
		if (Floor && FVector::Dist(Floor->GetActorLocation(), PlayerLocation) > MaxDistance)
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

TSubclassOf<AActor> URunGameFloorSubsystem::RandomSelectFloorClass()
{
	if (FMath::RandRange(1, 100) <= 80)
	{
		if (LoadedStraightClasses.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, LoadedStraightClasses.Num() - 1);
			return LoadedStraightClasses[Index];
		}
	}
	else
	{
		if (LoadedTurnClasses.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, LoadedTurnClasses.Num() - 1);
			return LoadedTurnClasses[Index];
		}
	}

	if (LoadedStraightClasses.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, LoadedStraightClasses.Num() - 1);
		return LoadedStraightClasses[Index];
	}

	return nullptr;
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

	FVector OffScreenLocation = FVector(0.0f, 0.0f, -100000.0f);
	AFloorBase* NewFloor = GetWorld()->SpawnActor<AFloorBase>(InClass, OffScreenLocation, FRotator::ZeroRotator, SpawnParams);

	if (!NewFloor)
	{
		UE_LOG(LogTemp, Error, TEXT("RunGameFloorSubsystem: Failed to spawn floor actor of class %s"), *InClass->GetName());
	}

	return NewFloor;
}
