// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "RunGameMainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class RUNGAME_API URunGameMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** Start the game, called by the start button in the main menu */
	UFUNCTION(BlueprintCallable, Category="Main Menu")
	void StartGame();

	/** Quit the game, called by the quit button in the main menu */
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void QuitGame();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

protected:
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();
	
};
