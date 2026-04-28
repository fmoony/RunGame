// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RunGameType.h"
#include "RunGamePlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class ARunGameHUD;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class ARunGamePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	/** Switch to UI input mode: show mouse cursor, disable character control */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Input")
	void SetInputModeToUIOnly();

	/** Tag used to find the MainMenuCamera placed in the level */
	UPROPERTY(EditAnywhere, Category = "RunGame|Camera")
	FName MainMenuCameraTag = FName("MainMenuCamera");

protected:
	/** Finds the main menu camera by tag and blends the view to it */
	void SetViewTargetToMainMenuCamera();

private:
	/** Reactively manages input mode and view target based on game state changes */
	UFUNCTION()
	void OnGameStateChangedCallback(ERunGameGameState OldState, ERunGameGameState NewState);
};

