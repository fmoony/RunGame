#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameCollisionAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionStateChanged);

class UNiagaraSystem;
class UPlayerRuntimeState;
class UCapsuleComponent;

/**
 * 碰撞管理组件 —— 自治单元，利用角色自身胶囊体做碰撞检测。
 *
 * 监听 PlayerRuntimeState::OnEffectTagChanged 激活碰撞。
 * 监听 OnCharacterStateChanged → Dead 时清冷却，死亡状态阻止碰撞。
 *
 * Collision ability component — self-contained, uses character capsule for collision.
 * Listens to PlayerRuntimeState::OnEffectTagChanged for activation.
 * Clears cooldowns on death, blocks collision while dead.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameCollisionAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameCollisionAbilityComponent();

	UPROPERTY(EditAnywhere, Category = "Collision")
	float CollisionDamage = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Collision")
	FGameplayTagQuery ActivationTagQuery;

	/** 命中冷却时间（秒）— 同一 Actor 两次命中之间的最短间隔 Hit cooldown (seconds) — minimum interval between two hits on same actor */
	UPROPERTY(EditAnywhere, Category = "Collision", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float HitCooldownDuration = 0.5f;

	/** 碰撞状态变更时广播 — Tag 增删 / 冷却增删 Broadcast when collision state changes — tag add/remove, cooldown add/expire */
	UPROPERTY(BlueprintAssignable, Category = "Collision")
	FOnCollisionStateChanged OnCollisionStateChanged;

	// 运行时状态 — 调试面板可读 Runtime state — readable by debug panel
	int32 ActiveTagCount = 0;
	FGameplayTag ActiveSkillTag;

	/** 命中冷却记录 — Actor → 冷却到期时间，Transient 不序列化，由 EndPlay/Death 清空
	 *  Hit cooldown map — Actor → expiry time, Transient (no serialization), cleared on EndPlay/Death */
	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, float> HitCooldowns;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnEffectTagChanged(FGameplayTag Tag, bool bAdded);

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 角色状态变更 — Dead 时清空命中冷却 State change — clear cooldowns on death */
	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> CachedPRS;

	UPROPERTY()
	TObjectPtr<UCapsuleComponent> Capsule;
};
