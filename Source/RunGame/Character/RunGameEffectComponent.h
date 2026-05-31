#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunGameType.h"
#include "RunGameEffectComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UMaterialInstanceDynamic;
class UHealthComponent;
class UPlayerRuntimeState;

/**
 * 角色特效组件 —— 自治单元，统管所有视觉特效
 *
 * 无敌：监听 HealthComponent::OnInvincibilityChanged，附着/销毁 Niagara 粒子。
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
	void OnInvincibilityChanged(bool bNewInvincible);

	// -- 溶解 Dissolve --

	UFUNCTION()
	void OnDissolveTriggered();

	void StartDissolveMaterial();
	void TickDissolveMaterial();
	void OnDissolveComplete();

	// -- 内部 Internal --

	void SpawnEffect(UNiagaraSystem* System, TObjectPtr<UNiagaraComponent>& OutComponent);
	void DestroyEffect(TObjectPtr<UNiagaraComponent>& Component);

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComp;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> InvincibilityFX;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> DissolveFX;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DissolveMaterials;

	FTimerHandle DissolveTickTimer;
	FTimerHandle DissolveDestroyTimer;
	float DissolveElapsed = 0.0f;
};
