#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunGameType.h"
#include "GameplayTagContainer.h"
#include "RunGameCameraComponent.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URunGameTimerSubsystem;
class URunGameInputContextComponent;

/**
 * 镜头逻辑组件 — 挂载于 Character，听命于玩家状态机
 * Camera logic component — attached to Character, driven by player state machine
 *
 * 状态驱动：Idle/跑动(FOV速度曲线) / Sliding(拉近) / Airborne(拉远) / Dead(脱离留在原地)
 * State-driven: Idle/Run(FOV speed curve) / Slide(close) / Airborne(far) / Dead(detach & stay)
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameCameraComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	float BaseFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	float SpeedFOVBoost = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	float FOVInterpSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SpringArm")
	float DefaultArmLength = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SpringArm")
	float AirborneArmLength = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SpringArm")
	float SlideArmLength = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SpringArm")
	float ArmInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|MainMenu")
	FName MainMenuCameraTag = FName("MainMenuCamera");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Blend")
	float RestoreBlendTime = 0.3f;

	/** 处理视角输入并作用到拥有者控制器 Handle look input on the owner controller */
	void HandleLookInput(float Yaw, float Pitch) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void CacheOwnerComponents();
	class APlayerController* GetPC() const;

	UFUNCTION()
	void OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UFUNCTION()
	void OnCharacterDied(FGameplayTag DamageType, class ARunGameCharacter* DeadCharacter);

	void BindInputContext();
	void UnbindInputContext();
	void OnLookInputChanged(const FVector2D& LookAxis);

	void SwitchToMainMenuCamera();
	void SwitchToFollowCamera();
	void DetachCameraToWorld();
	void ReattachCameraToOwner();

	float CurrentTargetFOV = 90.0f;
	float CurrentSmoothedFOV = 90.0f;
	float CurrentTargetArmLength = 400.0f;
	float CurrentSmoothedArmLength = 400.0f;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> CachedCameraBoom;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CachedFollowCamera;

	UPROPERTY()
	TObjectPtr<URunGameTimerSubsystem> TimerSubsystem;

	UPROPERTY()
	TObjectPtr<URunGameInputContextComponent> InputContext;

	FDelegateHandle LookInputChangedHandle;

	ERunGameCharacterState CurrentCharState = ERunGameCharacterState::Idle;
	bool bCameraDetached = false;
};
