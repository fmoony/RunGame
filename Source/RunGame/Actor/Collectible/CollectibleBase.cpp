// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Collectible/CollectibleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/RunGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

ACollectibleBase::ACollectibleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	bIsCollected = false;
}

void ACollectibleBase::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACollectibleBase::OnOverlapBegin);
}

void ACollectibleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCollected)
	{
		return;
	}

	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

	const float FloatOffset = FloatAmplitude * FMath::Sin(GetWorld()->GetTimeSeconds() * FloatFrequency);
	SetActorLocation(FVector(InitialLocation.X, InitialLocation.Y, InitialLocation.Z + FloatOffset));
}

void ACollectibleBase::ActivateCollectible(const FTransform& SpawnTransform)
{
	bIsCollected = false;
	SetActorTransform(SpawnTransform);
	InitialLocation = SpawnTransform.GetLocation();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ACollectibleBase::DeactivateCollectible()
{
	bIsCollected = true;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ACollectibleBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCollected)
	{
		return;
	}

	ARunGameCharacter* Character = Cast<ARunGameCharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	OnCollected(Character);
}

void ACollectibleBase::OnCollected_Implementation(ARunGameCharacter* CollectingCharacter)
{
	if (bIsCollected)
	{
		return;
	}

	bIsCollected = true;

	if (CollectionVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectionVFX, GetActorLocation(), GetActorRotation());
	}

	if (CollectionSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CollectionSFX, GetActorLocation());
	}

	OnCollectedDelegate.Broadcast(this);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}
