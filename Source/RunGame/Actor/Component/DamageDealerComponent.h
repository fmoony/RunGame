// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameplayTagContainer.h"
#include "DamageDealerComponent.generated.h"

UCLASS(ClassGroup = (RunGame), meta = (BlueprintSpawnableComponent))
class RUNGAME_API UDamageDealerComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UDamageDealerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FGameplayTag DamageType;

	UFUNCTION()
	void OnTrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
