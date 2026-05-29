#include "Character/Animation/AnimNotify_RunGameEvent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Engine/World.h"

void UAnimNotify_RunGameEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	if (UWorld* World = MeshComp->GetWorld())
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->NotifyDeathAnimationFinished();
		}
	}
}
