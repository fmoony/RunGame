#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ImpactReceiver.generated.h"

/** 冲击信息——描述一次碰撞的完整上下文 Impact info — full context of a collision */
USTRUCT(BlueprintType)
struct FImpactInfo
{
	GENERATED_BODY()

	/** 冲击强度（伤害值/推力大小）Impact strength (damage / push force) */
	UPROPERTY(BlueprintReadWrite)
	float Force = 0.0f;

	/** 冲击方向 Impact direction */
	UPROPERTY(BlueprintReadWrite)
	FVector Direction = FVector::ZeroVector;

	/** 碰撞点 World-space impact point */
	UPROPERTY(BlueprintReadWrite)
	FVector ImpactPoint = FVector::ZeroVector;

	/** 碰撞法线 Impact normal */
	UPROPERTY(BlueprintReadWrite)
	FVector ImpactNormal = FVector::ZeroVector;

	/** 冲击类型 Tag（Impact.Heavy / Impact.Fire / Impact.Electric ...） */
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ImpactType;

	/** 来源 Actor The actor that caused this impact */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr;
};

UINTERFACE(MinimalAPI, Blueprintable)
class UImpactReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 冲击接收接口——碰撞体命中时调用。
 * 实现方自行定义响应：破坏、弹飞、触发机关、施加 Buff 等。
 *
 * Impact receiver interface — called when a collision sphere hits.
 * Implementors define their own response: destroy, bounce, trigger, apply buff, etc.
 */
class RUNGAME_API IImpactReceiver
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Impact")
	void ReceiveImpact(FGameplayTag SkillTag, const FImpactInfo& Impact);
};
