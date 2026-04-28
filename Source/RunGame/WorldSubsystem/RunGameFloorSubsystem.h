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
	URunGameFloorSubsystem();

	// ===== 初始化 =====

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void InitializeFloorSystem(
		const TArray<TSoftClassPtr<AActor>>& InStraightClasses,
		const TArray<TSoftClassPtr<AActor>>& InTurnClasses,
		int32 InPreAllocateCount = 10
	);

	// ===== 地板生成 =====

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void SpawnInitialFloors(const FTransform& StartTransform, int32 StraightCount = 5, int32 RandomCount = 15);

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	AFloorBase* RequestNextFloor();

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	AFloorBase* RequestFloorAt(TSubclassOf<AActor> InClass, const FVector& Location, const FRotator& Rotation);

	// ===== 地板回收 =====

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void ReturnFloor(AFloorBase* Floor);

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void RecycleDistantFloors(const FVector& PlayerLocation, float MaxDistance = 3000.0f);

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void HideAllActiveFloors();

	// ===== 查询 =====

	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem")
	FTransform GetNextSpawnTransform() const { return NextSpawnTransform; }

	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem")
	int32 GetActiveFloorCount() const { return ActiveFloors.Num(); }

	UFUNCTION(BlueprintPure, Category = "RunGame|FloorSystem")
	int32 GetPooledFloorCount() const;

	UFUNCTION(BlueprintCallable, Category = "RunGame|FloorSystem")
	void ClearAllFloors();

	// ===== 异步加载就绪事件 =====

	UPROPERTY(BlueprintAssignable, Category = "RunGame|FloorSystem")
	FOnFloorSystemReadyDelegate OnFloorSystemReady;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	// ===== 内部方法 =====

	TSubclassOf<AActor> RandomSelectFloorClass();

	AFloorBase* AcquireFloorFromPool(TSubclassOf<AActor> InClass);

	AFloorBase* CreateNewFloorActor(TSubclassOf<AActor> InClass);

	void StartAsyncLoad();

	UFUNCTION()
	void OnFloorClassesLoaded();

	// ===== 配置数据 =====

	TArray<TSoftClassPtr<AActor>> StraightClassPtrs;

	TArray<TSoftClassPtr<AActor>> TurnClassPtrs;

	UPROPERTY()
	TArray<TSubclassOf<AActor>> LoadedStraightClasses;

	UPROPERTY()
	TArray<TSubclassOf<AActor>> LoadedTurnClasses;

	int32 PreAllocateCount;

	bool bIsLoading;

	bool bIsInitialized;

	// ===== 对象池（按类型维护多子池） =====

	TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap;

	UPROPERTY()
	TArray<TObjectPtr<AFloorBase>> ActiveFloors;

	// ===== 生成位置追踪 =====

	FTransform NextSpawnTransform;
};
