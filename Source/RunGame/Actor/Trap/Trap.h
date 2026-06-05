#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interfaces/ImpactReceiver.h"
#include "Trap.generated.h"

class UStaticMeshComponent;
class UHealthComponent;
class UDamageDealerComponent;
class UNiagaraSystem;
class USoundBase;

/**
 * 可破坏陷阱基类 — 实现 IImpactReceiver，被角色碰撞技能击中后损毁
 * Destructible trap base — implements IImpactReceiver, destroyed by character collision ability
 *
 * 蓝图派生后替换 Mesh 即可创建不同外观。作为 Floor 子 Actor 放置，随 Floor 池子回收/重置。
 * Derive in Blueprint and swap Mesh to create different looks.
 * Place as a child actor of Floor — recycles/resets with the Floor pool.
 */
UCLASS(Abstract, Blueprintable)
class RUNGAME_API ATrap : public AActor, public IImpactReceiver
{
	GENERATED_BODY()

public:
	ATrap();

	// ---- 池子生命周期 Pool lifecycle ----

	/** 激活陷阱 — 复活 HP + 恢复可见 + 恢复碰撞 Activate trap — revive HP + restore visibility + restore collision */
	UFUNCTION(BlueprintCallable, Category = "Trap")
	void ActivateTrap();

	/** 停用陷阱 — 隐藏 + 禁用碰撞（供 Floor 回收时调用）Deactivate trap — hide + disable collision (called by Floor on recycle) */
	UFUNCTION(BlueprintCallable, Category = "Trap")
	void DeactivateTrap();

	// ---- IImpactReceiver 接口 IImpactReceiver interface ----

	virtual void ReceiveImpact_Implementation(FGameplayTag SkillTag, const FImpactInfo& Impact) override;

protected:
	virtual void BeginPlay() override;

	// ---- 组件 Components ----

	/** 陷阱网格体 — 视觉 + ImpactReceiver 碰撞触发器 + 伤害碰撞体 Trap mesh — visual + ImpactReceiver trigger + damage collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** 生命值组件 — 管理 HP、受伤、死亡委托 Health component — manages HP, damage, death delegate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	/** 伤害组件 — 绑定 Mesh Overlap，角色触碰时造成伤害 Damage dealer — binds to Mesh overlap, damages characters on contact */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Components")
	TObjectPtr<UDamageDealerComponent> DamageDealer;

	// ---- 可配置属性 Configurable properties ----

	/** 所需技能标签查询 — 为空则任意技能均可撞坏；不为空则只有匹配的技能才能摧毁
	 *  Required skill tag query — empty = any skill can destroy; set = only matching skills */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Collision")
	FGameplayTagQuery RequiredSkillTagQuery;

	/** 损毁时生成的 Niagara 特效 Niagara VFX spawned on destruction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Effects")
	TObjectPtr<UNiagaraSystem> DestroyEffect;

	/** 损毁时播放的音效 Sound played on destruction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Effects")
	TObjectPtr<USoundBase> DestroySound;

	// ---- 蓝图事件 Blueprint events ----

	/** 陷阱被摧毁时调用 — BP 侧添加额外逻辑（掉落物、通知等）Called when trap HP reaches zero */
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap")
	void OnTrapDestroyed();

private:
	/** HealthComponent::OnDeath 回调 — 转发到视觉损毁流程 Callback for HealthComponent::OnDeath — forwards to visual destruction */
	UFUNCTION()
	void OnTrapDeath(FGameplayTag DamageType, AActor* DeathCauser);

	/** 执行视觉损毁 — 隐藏网格 + 禁用碰撞 + 播放特效 Perform visual destruction — hide mesh + disable collision + play FX */
	void BreakTrap(const FVector& ImpactPoint);
};
