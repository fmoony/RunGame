#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunGameInputContextComponent.generated.h"

/**
 * 预留的输入预处理组件，本阶段不保存输入或请求。
 * Reserved input preprocessing component; it owns no input data in this phase.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameInputContextComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameInputContextComponent();
};
