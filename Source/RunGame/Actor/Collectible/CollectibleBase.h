// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectibleBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ARunGameCharacter;
class UParticleSystem;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollectibleCollectedSignature, ACollectibleBase*, Collectible);

UCLASS(Abstract)
class RUNGAME_API ACollectibleBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionComponent;

	/** Degrees per second of self-rotation */
	UPROPERTY(EditDefaultsOnly, Category = "Collectible|Animation")
	float RotationSpeed = 90.0f;

	/** Vertical float oscillation amplitude in world units */
	UPROPERTY(EditDefaultsOnly, Category = "Collectible|Animation")
	float FloatAmplitude = 10.0f;

	/** Oscillation frequency multiplier */
	UPROPERTY(EditDefaultsOnly, Category = "Collectible|Animation")
	float FloatFrequency = 2.0f;

	/** Particle effect spawned at location on collection */
	UPROPERTY(EditDefaultsOnly, Category = "Collectible|Effects")
	TObjectPtr<UParticleSystem> CollectionVFX;

	/** Sound played at location on collection */
	UPROPERTY(EditDefaultsOnly, Category = "Collectible|Effects")
	TObjectPtr<USoundBase> CollectionSFX;

	/** True after the collectible has been picked up — pool-ready */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible|State")
	bool bIsCollected;

	FVector InitialLocation;

public:
	ACollectibleBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Activate this collectible from the pool at the given transform */
	UFUNCTION(BlueprintCallable, Category = "Collectible")
	virtual void ActivateCollectible(const FTransform& SpawnTransform);

	/** Deactivate and mark as collected for pool recycling */
	UFUNCTION(BlueprintCallable, Category = "Collectible")
	virtual void DeactivateCollectible();

	/** Returns whether this collectible has been picked up */
	UFUNCTION(BlueprintPure, Category = "Collectible")
	bool IsCollected() const { return bIsCollected; }

	/** Broadcast when this collectible is picked up by a character */
	UPROPERTY(BlueprintAssignable, Category = "Collectible")
	FOnCollectibleCollectedSignature OnCollectedDelegate;

protected:
	/** Handles trigger overlap — validates character, then calls OnCollected */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when player picks up this collectible. Base: VFX, SFX, hide. Override for game logic */
	UFUNCTION(BlueprintNativeEvent, Category = "Collectible")
	void OnCollected(ARunGameCharacter* CollectingCharacter);
	virtual void OnCollected_Implementation(ARunGameCharacter* CollectingCharacter);
};
