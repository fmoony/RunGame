// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DamageDealerComponent.generated.h"

class UPrimitiveComponent;

/**
 * 伤害组件 — 绑定宿主网格体 Overlap 事件，触碰 IDamagable 时造成伤害
 * Damage dealer — binds to owner mesh overlap, damages IDamagable actors on contact
 *
 * 不自带碰撞体 — 伤害形状 = 宿主 Mesh 碰撞形状，方即方、圆即圆。
 * No built-in collision — damage shape = owner mesh collision, matches any mesh exactly.
 */
UCLASS(ClassGroup = (RunGame), meta = (BlueprintSpawnableComponent))
class RUNGAME_API UDamageDealerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UDamageDealerComponent();

	/** 造成伤害值 Damage amount dealt on contact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 100.0f;

	/** 伤害类型标签 Damage type tag */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FGameplayTag DamageType;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** 绑定到宿主碰撞体（Mesh/Box/Sphere 等任意 PrimitiveComponent）Bind to owner's collision primitive */
	void BindToOwnerMesh();

	/** 解除绑定 Unbind */
	void UnbindFromOwnerMesh();

	UFUNCTION()
	void OnTrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 已绑定的碰撞体 — EndPlay 时解绑 The primitive we bound to — unbind in EndPlay */
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> BoundPrimitive;
};
