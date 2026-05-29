#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_RunGameEvent.generated.h"

/**
 * 通用动画通知 — 挂死亡动画末尾调 PRS->NotifyDeathAnimationFinished。
 * Generic animation notify — placed at end of death anim to call PRS->NotifyDeathAnimationFinished.
 */
UCLASS()
class RUNGAME_API UAnimNotify_RunGameEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
