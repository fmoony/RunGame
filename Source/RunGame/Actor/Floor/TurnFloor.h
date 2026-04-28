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
	ATurnFloor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTurnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTurnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
