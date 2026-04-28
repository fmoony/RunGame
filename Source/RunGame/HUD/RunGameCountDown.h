// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameCountDown.generated.h"

/**
 * 
 */
UCLASS()
class RUNGAME_API URunGameCountDown : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** Callback when countdown time updates, received from GameState broadcast */
	UFUNCTION()
	void OnCountdownReceived(int32 CurrentTime);

public:
	/** Triggers the number animation in blueprint for the current countdown value */
	UFUNCTION(BlueprintImplementableEvent, Category = "RunGame|Animation")
	void PlayNumberAnimation(int32 CurrentTime);

};
