#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunGameType.h"
#include "GameplayTagContainer.h"
#include "RunGameCameraComponent.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URunGameTimerSubsystem;
class URunGameMovementComponent;

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

	/** 应用玩家视角输入 / Apply player look input */
	void ApplyLookInput(const FVector2D& LookAxis);

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

	/** 镜头相对跑酷方向允许的最大水平偏角 / Maximum yaw offset from the run direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Control", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float MaxYawOffset = 90.0f;

	/** TurnBox 改变方向后镜头参考方向的匀速旋转速度 / Constant camera reference turn speed after a TurnBox */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Control", meta = (ClampMin = "0.0"))
	float TurnFollowYawSpeed = 180.0f;

	/** 没有视角输入时是否缓慢回正 / Recenter yaw when no look input is active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Control")
	bool bEnableYawRecentering = false;

	/** 普通镜头回正插值速度 / Interpolation speed for normal yaw recentering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Control", meta = (EditCondition = "bEnableYawRecentering", ClampMin = "0.0"))
	float YawRecenteringSpeed = 1.5f;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void CacheOwnerComponents();
	class APlayerController* GetPC() const;
	void UpdateControlRotation(float DeltaTime, bool bReceivedLookInput);
	void SyncRunDirection(bool bResetViewYaw);
	void HandleRunDirectionChanged(float OldYaw, float NewYaw);

	UFUNCTION()
	void OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UFUNCTION()
	void OnCharacterDied(FGameplayTag DamageType, class ARunGameCharacter* DeadCharacter);

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
	TObjectPtr<URunGameMovementComponent> MovementComponent;

	ERunGameCharacterState CurrentCharState = ERunGameCharacterState::Idle;
	bool bCameraDetached = false;
	bool bReceivedLookInputSinceLastTick = false;
	bool bRunDirectionInitialized = false;
	float CurrentReferenceYaw = 0.0f;
	float TargetReferenceYaw = 0.0f;
};
