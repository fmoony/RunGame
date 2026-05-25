// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Collectible/Coin.h"
#include "Character/RunGameCharacter.h"
#include "Player/RunGamePlayerState.h"

void ACoin::OnCollected_Implementation(ARunGameCharacter* CollectingCharacter)
{
	Super::OnCollected_Implementation(CollectingCharacter);

	if (!CollectingCharacter)
	{
		return;
	}

	ARunGamePlayerState* PlayerState = CollectingCharacter->GetPlayerState<ARunGamePlayerState>();
	if (PlayerState)
	{
		PlayerState->AddScore(CoinValue);
	}
}
