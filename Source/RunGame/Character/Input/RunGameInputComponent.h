#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Character/Input/RunGameInputTypes.h"
#include "RunGameInputComponent.generated.h"

class UInputAction;
class UInputComponent;
class URunGameControlPipelineComponent;
class URunGameInputContextComponent;
class USkillComponent;
struct FInputActionValue;

/**
 * 正式输入组件，拥有输入资产绑定，并把原始输入写入 InputContext
 * Formal input component that owns input action assets and writes raw input into InputContext.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameInputComponent();

	/** 绑定角色输入动作，把 EnhancedInput 入口收束到输入组件 Bind character input actions and keep EnhancedInput entrypoints inside this component */
	void BindInput(UInputComponent* PlayerInputComponent);

	/** 兼容旧入口：写入输入上下文 Compatibility entrypoint: write into input context */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void BufferInput(ERunGameInputCommand Command);

	/** 清空输入上下文 Clear input context */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void ClearBuffer();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** 跳跃输入动作 Jump input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/** 移动输入动作 Move input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** 滑铲输入动作 Slide input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SlideAction;

	/** 视角输入动作 Look input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	/** 鼠标视角输入动作 Mouse look input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MouseLookAction;

private:
	void CacheOwnerComponents();
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJumpStarted();
	void HandleJumpCompleted();
	void HandleSlideStarted();
	void HandleSkillStarted(FGameplayTag SkillTag);

	UPROPERTY()
	TObjectPtr<class ARunGameCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<URunGameInputContextComponent> InputContext;

	UPROPERTY()
	TObjectPtr<URunGameControlPipelineComponent> ControlPipeline;

	UPROPERTY()
	TObjectPtr<USkillComponent> SkillComponent;
};
