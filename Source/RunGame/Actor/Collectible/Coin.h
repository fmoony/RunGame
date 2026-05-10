// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Collectible/CollectibleBase.h"
#include "Coin.generated.h"

UCLASS()
class RUNGAME_API ACoin : public ACollectibleBase
{
	GENERATED_BODY()

protected:
	/** Score granted to the collecting player */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	int64 CoinValue = 10;

protected:
	virtual void OnCollected_Implementation(ARunGameCharacter* CollectingCharacter) override;
};
