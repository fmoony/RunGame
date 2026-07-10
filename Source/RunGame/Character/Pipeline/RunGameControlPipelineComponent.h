#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunGameType.h"
#include "RunGameControlPipelineComponent.generated.h"

class ARunGameCharacter;
class URunGameCameraComponent;
class URunGameInputContextComponent;
class URunGameLocomotionComponent;
class UPlayerRuntimeState;
class USkillComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameControlPipelineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameControlPipelineComponent();

	void ProcessInputFrame();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void CacheOwnerComponents();
	void ProcessBufferedCommand();

	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UPROPERTY()
	TObjectPtr<ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<URunGameInputContextComponent> InputContext;

	UPROPERTY()
	TObjectPtr<URunGameLocomotionComponent> LocomotionComponent;

	UPROPERTY()
	TObjectPtr<URunGameCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<USkillComponent> SkillComponent;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;
};
