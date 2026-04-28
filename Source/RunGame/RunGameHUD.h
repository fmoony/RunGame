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

	UFUNCTION(BlueprintCallable,Category = "UI")
	void ShowUI(TSubclassOf<UUserWidget> UIClass);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideUI(TSubclassOf<UUserWidget> UIClass);

	UFUNCTION(BlueprintPure, Category = "UI")
	UUserWidget* GetCurrentUI() const { return CurrentActiveWidget; };

	void UpdateUIForState(ERunGameGameState NewState);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Config")
	TMap<ERunGameGameState, TSubclassOf<UUserWidget>> CurrentUIMap;
	
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);

private:
	UPROPERTY()
	UUserWidget* CurrentActiveWidget;
};
