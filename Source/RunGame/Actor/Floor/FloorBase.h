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

	UPROPERTY(EditDefaultsOnly, Category = "FloorType")
	FFloorType FloorType;

	UPROPERTY(EditDefaultsOnly, Category = "FloorRecycleTime")
	float RecycleDelayTime;

public:
	AFloorBase();

	virtual FTransform GetAttachToTransform(const FVector& MyLocation);

protected:
	virtual void BeginPlay() override;

	FTimerHandle MyTimeHandle;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void ReturnToPool();
};
