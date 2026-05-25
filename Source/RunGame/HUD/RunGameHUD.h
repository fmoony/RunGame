// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RunGameType.h"
#include "RunGameHUD.generated.h"

class UUserWidget;

/**
 *
 */
UCLASS()
class RUNGAME_API ARunGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Creates and displays the specified UI widget on the viewport */
	UFUNCTION(BlueprintCallable,Category = "UI")
	void ShowUI(TSubclassOf<UUserWidget> UIClass);

	/** Removes the specified UI widget from the viewport if currently displayed */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideUI(TSubclassOf<UUserWidget> UIClass);

	UFUNCTION(BlueprintPure, Category = "UI")
	UUserWidget* GetCurrentUI() const { return CurrentActiveWidget; };

	/** Switches the displayed UI widget to match the given game state */
	void UpdateUIForState(ERunGameGameState NewState);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Config")
	TMap<ERunGameGameState, TSubclassOf<UUserWidget>> CurrentUIMap;

	/** Reactively updates the displayed UI when the game state changes */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

private:
	UPROPERTY()
	UUserWidget* CurrentActiveWidget;
};
