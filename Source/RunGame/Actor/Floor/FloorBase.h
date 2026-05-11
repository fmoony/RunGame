// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunGameType.h"
#include "FloorBase.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;
class UCoinSpawnerComponent;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloorPlayerEnteredSignature, AFloorBase*, Floor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloorRecycleRequestedSignature, AFloorBase*, Floor);

UCLASS()
class RUNGAME_API AFloorBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_J", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ScenceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_J", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> FloorRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_J", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_J", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BoxMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_J", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointMiddle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_J", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_J", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointLeft;

	/** Spline path for coin placement, editable in editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coins")
	TObjectPtr<USplineComponent> CoinPathSpline;

	UPROPERTY(EditDefaultsOnly, Category = "FloorType")
	FFloorType FloorType;

	UPROPERTY(EditDefaultsOnly, Category = "FloorRecycleTime")
	float RecycleDelayTime = 2.0f;

public:
	/** Constructs the floor base actor with default components and collision */
	AFloorBase();

	/** Returns the transform for attaching the next floor piece at the given spawn point */
	virtual FTransform GetAttachToTransform(const FVector& MyLocation);

protected:
	virtual void BeginPlay() override;

	FTimerHandle MyTimeHandle;

public:
	virtual void Tick(float DeltaTime) override;

	/** Broadcast when the player enters this floor's box trigger */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Floor")
	FOnFloorPlayerEnteredSignature OnPlayerEntered;

	/** Broadcast when the recycle delay timer expires and the floor is ready to return to pool */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Floor")
	FOnFloorRecycleRequestedSignature OnRecycleRequested;

	/** Coin spawner component. CoinClass == nullptr means no coins on this floor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coins")
	TObjectPtr<UCoinSpawnerComponent> CoinSpawnerComponent;

	/** Handles box overlap events, broadcasting OnPlayerEntered and starting the recycle timer */
	UFUNCTION()
	virtual void BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Broadcasts OnRecycleRequested to notify listeners this floor is ready for recycling */
	virtual void ReturnToPool();
};
