#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Input/RunGameInputTypes.h"
#include "RunGameControlPipelineComponent.generated.h"

class ARunGameCharacter;
class UPlayerRuntimeState;
class URunGameInputComponent;
class URunGameLocomotionComponent;
class URunGameMovementComponent;
class URunGameCameraComponent;
class USkillComponent;

/**
 * 角色控制管线，按固定阶段读取输入、处理移动信号并路由领域请求。
 * Character control pipeline that processes snapshots, locomotion signals, and domain requests in fixed stages.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameControlPipelineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameControlPipelineComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 缓存同一角色上的控制领域组件 / Cache control-domain components on the owning character
	void CacheOwnerComponents();

	// 处理上一物理帧产生的移动信号 / Process locomotion signals produced by the previous physics frame
	void ProcessLocomotionSignals();

	// 应用 Move、Look 和 JumpReleased 连续控制 / Apply Move, Look, and JumpReleased continuous control
	void ProcessContinuousInput(const FRunGameInputSnapshot& Snapshot);

	// 按领域 FIFO 路由离散请求 / Route discrete requests with per-domain FIFO ordering
	void ProcessRequests(const FRunGameInputSnapshot& Snapshot);

	// 尝试执行一个移动领域请求 / Try to execute one locomotion request
	ERunGameInputRequestResult ProcessLocomotionRequest(const FRunGameInputRequest& Request);

	UPROPERTY()
	TObjectPtr<ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<URunGameInputComponent> InputComponent;

	UPROPERTY()
	TObjectPtr<URunGameLocomotionComponent> LocomotionComponent;

	UPROPERTY()
	TObjectPtr<URunGameMovementComponent> MovementComponent;

	UPROPERTY()
	TObjectPtr<URunGameCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<USkillComponent> SkillComponent;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;

	uint64 LastJumpReleaseGeneration = 0;
};
