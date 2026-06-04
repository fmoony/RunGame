#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RunGameCollisionAbilityComponent.generated.h"

class UNiagaraSystem;
class UPlayerRuntimeState;
class UCapsuleComponent;

/**
 * 碰撞管理组件 —— 自治单元，利用角色自身胶囊体做碰撞检测。
 *
 * 监听 PlayerRuntimeState::OnEffectTagChanged。
 * Tag 激活 → 胶囊体开启碰撞 Overlap。
 * Tag 全移除 → 恢复默认碰撞设置。
 *
 * Collision ability component — self-contained, uses character capsule for collision.
 * Listens to PlayerRuntimeState::OnEffectTagChanged.
 * Tag active → enables capsule overlap.
 * All tags removed → restores default collision settings.
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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnEffectTagChanged(FGameplayTag Tag, bool bAdded);

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	TObjectPtr<UPlayerRuntimeState> CachedPRS;

	UPROPERTY()
	TObjectPtr<UCapsuleComponent> Capsule;

	int32 ActiveTagCount = 0;
	FGameplayTag ActiveSkillTag;
	TArray<TWeakObjectPtr<AActor>> DestroyedActors;
};
