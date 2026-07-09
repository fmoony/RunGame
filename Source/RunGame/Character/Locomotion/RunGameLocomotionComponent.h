#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/RunGameInputBufferComponent.h"
#include "RunGameType.h"
#include "RunGameLocomotionComponent.generated.h"

class ARunGameCharacter;
class UPlayerRuntimeState;
class URunGameMovementComponent;
struct FHitResult;

/**
 * 跑酷运动规则组件，后续承接跳跃/滑铲/转向等玩法层运动规则
 * Runner locomotion rules component; will own gameplay-level jump/slide/turn rules later.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameLocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameLocomotionComponent();

	/** 获取当前角色语义状态 Get current semantic character state */
	ERunGameCharacterState GetRuntimeCharacterState() const;

	/** 获取缓存的运动组件 Get cached movement component */
	FORCEINLINE URunGameMovementComponent* GetRunGameMovementComponent() const { return MovementComponent; }

	/** 消费输入命令，当前阶段转发到 Movement Consume an input command, currently forwards to Movement */
	bool TryConsumeInputCommand(ERunGameInputCommand Command) const;

	/** 处理跳跃按键释放 Handle jump input release */
	void HandleJumpInputReleased() const;

	/** 判断当前状态是否允许跳跃 Check whether the current state allows jump */
	bool CanStartJump(bool bDefaultCanJump) const;

	/** 响应 Character 起跳回调 React to Character jump callback */
	void HandleOwnerJumped() const;

	/** 响应 Character 落地回调 React to Character landed callback */
	void HandleOwnerLanded(const FHitResult& Hit) const;

	/** 应用转向规则，返回是否阻止横向输入 Apply turn rule and return whether lateral input should be blocked */
	bool ApplyTurnRotation(float Right) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** 缓存本地依赖，暂不接管任何行为 Cache local dependencies without taking over behavior yet */
	void CacheDependencies();

	UPROPERTY()
	TObjectPtr<ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<URunGameMovementComponent> MovementComponent;

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> RuntimeState;
};
