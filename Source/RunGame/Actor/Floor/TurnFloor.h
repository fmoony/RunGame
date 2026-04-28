// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Floor/FloorBase.h"
#include "TurnFloor.generated.h"

class UBoxComponent;

UCLASS()
class RUNGAME_API ATurnFloor : public AFloorBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> TurnBoxComponent;

public:
	/** Constructs a turn floor segment with a turn detection trigger box */
	ATurnFloor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	/** Called when a character enters the turn trigger box, enabling turn mode */
	void OnTurnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	/** Called when a character leaves the turn trigger box, disabling turn mode */
	void OnTurnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
