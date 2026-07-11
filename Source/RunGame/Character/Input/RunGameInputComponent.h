#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Character/Input/RunGameInputTypes.h"
#include "RunGameInputComponent.generated.h"

class UInputAction;
class UInputComponent;
class USkillComponent;
struct FInputActionValue;

/**
 * 采集 Enhanced Input，保存连续输入与待处理请求。
 * Collects Enhanced Input and owns continuous input values plus pending requests.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameInputComponent();

	/** 绑定角色输入动作 / Bind character input actions */
	void BindInput(UInputComponent* PlayerInputComponent);

	/** 返回输入数据副本，读取不会修改请求队列 / Return a snapshot without mutating the request queue */
	FRunGameInputSnapshot GetInputSnapshot() const;

	/** 移除超过缓冲时间的请求 / Remove requests older than the buffer timeout */
	void ExpireRequests(float CurrentTime);

	/** 按标识移除已经完成或拒绝的请求 / Remove a completed or rejected request by id */
	bool RemoveRequest(uint64 RequestId);

	/** 清空连续输入和所有待处理请求 / Clear continuous input and all pending requests */
	void ClearInputState();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** 跳跃输入动作 / Jump input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/** 移动输入动作 / Move input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** 滑铲输入动作 / Slide input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SlideAction;

	/** 视角输入动作 / Look input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	/** 鼠标视角输入动作 / Mouse look input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MouseLookAction;

	/** 离散请求队列容量 / Discrete request queue capacity */
	UPROPERTY(EditAnywhere, Category = "Input|Buffer", meta=(ClampMin="1"))
	int32 RequestCapacity = 16;

	/** 请求缓冲时间 / Request buffer timeout */
	UPROPERTY(EditAnywhere, Category = "Input|Buffer", meta=(ClampMin="0.0"))
	float RequestTimeout = 0.3f;

private:
	// 缓存绑定技能输入所需的组件 / Cache the skill component used to bind skill input actions
	void CacheOwnerComponents();

	// 将离散输入加入等待队列 / Enqueue a discrete input request
	void EnqueueRequest(ERunGameInputRequestType Type, FGameplayTag SkillTag = FGameplayTag());

	// 更新移动轴 / Update the move axis
	void HandleMove(const FInputActionValue& Value);

	// 移动输入结束时清零 / Clear the move axis when input ends
	void HandleMoveCompleted();

	// 更新视角轴 / Update the look axis
	void HandleLook(const FInputActionValue& Value);

	// 视角输入结束时清零 / Clear the look axis when input ends
	void HandleLookCompleted();

	// 记录跳跃按下并创建请求 / Record jump press and enqueue a request
	void HandleJumpStarted();

	// 记录跳跃释放边沿 / Record the jump release edge
	void HandleJumpCompleted();

	// 创建滑铲请求 / Enqueue a slide request
	void HandleSlideStarted();

	// 创建技能请求 / Enqueue a skill request
	void HandleSkillStarted(FGameplayTag SkillTag);

	UPROPERTY()
	TObjectPtr<USkillComponent> SkillComponent;

	UPROPERTY()
	FVector2D MoveAxis = FVector2D::ZeroVector;

	UPROPERTY()
	FVector2D LookAxis = FVector2D::ZeroVector;

	UPROPERTY()
	TArray<FRunGameInputRequest> PendingRequests;

	bool bJumpHeld = false;
	uint64 JumpReleaseGeneration = 0;
	uint64 NextRequestId = 1;
};
