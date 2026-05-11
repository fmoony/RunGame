// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Floor/FloorBase.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Actor/Component/CoinSpawnerComponent.h"
#include "RunGameCharacter.h"

AFloorBase::AFloorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	FloorRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Full"));
	RootComponent = FloorRootComponent;

	ScenceComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Father"));
	ScenceComponent->SetupAttachment(RootComponent);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(ScenceComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(ScenceComponent);

	SpawnPointMiddle = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPointMiddle"));
	SpawnPointRight = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPointRight"));
	SpawnPointLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPointLeft"));
	SpawnPointMiddle->SetupAttachment(ScenceComponent);
	SpawnPointRight->SetupAttachment(ScenceComponent);
	SpawnPointLeft->SetupAttachment(ScenceComponent);

	CoinSpawnerComponent = CreateDefaultSubobject<UCoinSpawnerComponent>(TEXT("CoinSpawner"));

	CoinPathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("CoinPathSpline"));
	CoinPathSpline->SetupAttachment(ScenceComponent);
	CoinPathSpline->SetHiddenInGame(true);

}

FTransform AFloorBase::GetAttachToTransform(const FVector& MyLocation)
{
	SpawnPointMiddle->UpdateComponentToWorld();
	return SpawnPointMiddle->GetComponentTransform();
}

void AFloorBase::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloorBase::BoxOverlap);
}

void AFloorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloorBase::BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("FloorBase: Box overlapped by %s"), *GetNameSafe(OtherActor));

	if (!OtherActor->IsA(ARunGameCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("FloorBase: Overlapping actor is not a RunGameCharacter, ignoring."));
		return;
	}

	OnPlayerEntered.Broadcast(this);

	if (MyTimeHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("FloorBase: Overlap occurred while recycle timer is active, clearing existing timer."));
		GetWorldTimerManager().ClearTimer(MyTimeHandle);
	}

	FTimerDelegate Delegate;
	Delegate.BindLambda([this]()
	{
		if (IsValid(this))
		{
			OnRecycleRequested.Broadcast(this);
			UE_LOG(LogTemp, Warning, TEXT("FloorBase: Recycled after delay"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("FloorBase: Attempted to recycle but floor is no longer valid!"));
		}
	});

	GetWorldTimerManager().SetTimer(MyTimeHandle, Delegate, RecycleDelayTime, false);
}

void AFloorBase::ReturnToPool()
{
	OnRecycleRequested.Broadcast(this);
}
