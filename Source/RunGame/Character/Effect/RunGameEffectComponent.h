#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameType.h"
#include "RunGameEffectComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UMaterialInstanceDynamic;
class UPlayerRuntimeState;

/**
 * 角色特效组件 —— 自治单元，统管所有视觉特效
 *
 * 无敌：监听 PlayerRuntimeState::OnEffectTagsChanged，匹配标签后附着/销毁 Niagara 粒子。
 * 溶解：监听 PlayerRuntimeState::OnDeathAnimationFinished，材质溶解动画 + 可选 Niagara 粒子 → 到期销毁角色。
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGAME_API URunGameEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URunGameEffectComponent();

	// ---- 无敌 Invincibility ----

	UPROPERTY(EditAnywhere, Category = "Effect|Invincibility")
	TObjectPtr<UNiagaraSystem> InvincibilityNiagara;

	/** 匹配后触发无敌视觉的标签查询 Tags matching this query trigger invincibility visual */
	UPROPERTY(EditAnywhere, Category = "Effect|Invincibility")
	FGameplayTagQuery InvincibilityTagQuery;

	// ---- 溶解 Dissolve ----

	UPROPERTY(EditAnywhere, Category = "Effect|Dissolve")
	TObjectPtr<UNiagaraSystem> DissolveNiagara;

	/** 溶解总时长（材质动画 + 到期后 Destroy 角色） */
	UPROPERTY(EditAnywhere, Category = "Effect|Dissolve")
	float DissolveDuration = 2.0f;

	/** 材质上驱动的溶解参数名 Material parameter name to drive during dissolve */
	UPROPERTY(EditAnywhere, Category = "Effect|Dissolve")
	FName DissolveParameterName = "DissolveAmount";

	// ---- 通用 Common ----

	UPROPERTY(EditAnywhere, Category = "Effect")
	FName AttachSocketName = NAME_None;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// -- 无敌 Invincibility --

	UFUNCTION()
	void OnEffectTagChanged(FGameplayTag Tag, bool bAdded);

	bool IsTagVisualRelevant(FGameplayTag Tag) const;

	// -- 溶解 Dissolve --

	UFUNCTION()
	void OnDissolveTriggered();

	UFUNCTION()
	void OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState);

	void StartDissolveMaterial();
	void TickDissolveMaterial();
	void OnDissolveComplete();
	void ResetDissolveMaterials();

	// -- 内部 Internal --

	void SpawnEffect(UNiagaraSystem* System, TObjectPtr<UNiagaraComponent>& OutComponent);
	void DestroyEffect(TObjectPtr<UNiagaraComponent>& Component);

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> CachedPRS;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> InvincibilityFX;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> DissolveFX;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DissolveMaterials;

	/** 缓存溶解前的原始材质 — 重生时复原 Cached original materials before dissolve — restored on respawn */
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OriginalMaterials;

	FTimerHandle DissolveTickTimer;
	FTimerHandle DissolveDestroyTimer;
	float DissolveElapsed = 0.0f;
};
