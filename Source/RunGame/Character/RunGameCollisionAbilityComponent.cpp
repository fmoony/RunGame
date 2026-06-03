#include "Character/RunGameCollisionAbilityComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/ImpactReceiver.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "RunGame.h"

URunGameCollisionAbilityComponent::URunGameCollisionAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameCollisionAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.AddDynamic(this, &URunGameCollisionAbilityComponent::OnEffectTagChanged);
	}

	// 绑定角色胶囊体 Overlap Bind character capsule overlap
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Capsule = Character->GetCapsuleComponent();
		if (Capsule)
		{
			Capsule->OnComponentBeginOverlap.AddDynamic(this, &URunGameCollisionAbilityComponent::OnCapsuleOverlap);
		}
	}
}

void URunGameCollisionAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Capsule)
	{
		Capsule->OnComponentBeginOverlap.RemoveDynamic(this, &URunGameCollisionAbilityComponent::OnCapsuleOverlap);
	}

	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.RemoveDynamic(this, &URunGameCollisionAbilityComponent::OnEffectTagChanged);
		CachedPRS = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// ---- Tag reaction ----

void URunGameCollisionAbilityComponent::OnEffectTagChanged(FGameplayTag Tag, bool bAdded)
{
	if (ActivationTagQuery.IsEmpty() || !Capsule) return;

	FGameplayTagContainer SingleTag;
	SingleTag.AddTag(Tag);
	if (!SingleTag.MatchesQuery(ActivationTagQuery)) return;

	if (bAdded)
	{
		ActiveSkillTag = Tag;
		ActiveTagCount++;
		UE_LOG(LogRunGame, Warning, TEXT("CollisionAbilityComponent:Tag added Tag:%s, Count:%d"), *Tag.ToString(), ActiveTagCount);
	}
	else
	{
		ActiveTagCount = FMath::Max(ActiveTagCount - 1, 0);
		UE_LOG(LogRunGame, Warning, TEXT("CollisionAbilityComponent:Tag removed Tag:%s, Count:%d"), *Tag.ToString(), ActiveTagCount);
	}
}

// ---- Collision ----

void URunGameCollisionAbilityComponent::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	// 只处理实现了 IImpactReceiver 的 Actor Only process actors that implement IImpactReceiver
	if (!OtherActor->Implements<UImpactReceiver>()) return;

	for (const auto& WeakActor : DestroyedActors)
	{
		if (WeakActor.Get() == OtherActor) return;
	}
	DestroyedActors.Add(OtherActor);

	// 碰撞点生成 Niagara Spawn impact effect
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactEffect,
			SweepResult.ImpactPoint,
			SweepResult.ImpactNormal.Rotation()
		);
	}

	// 构造冲击信息 → 通知目标 Build impact info → notify target
	FImpactInfo Impact;
	Impact.Force        = CollisionDamage;
	Impact.Direction    = GetOwner()->GetVelocity().GetSafeNormal();
	Impact.ImpactPoint  = SweepResult.ImpactPoint;
	Impact.ImpactNormal = SweepResult.ImpactNormal;
	Impact.ImpactType   = ActiveSkillTag;
	Impact.Instigator   = GetOwner();

	IImpactReceiver::Execute_ReceiveImpact(OtherActor, ActiveSkillTag, Impact);

	UE_LOG(LogRunGame, Warning, TEXT("CollisionAbilityComponent:Impact Tag:%s → %s"), *ActiveSkillTag.ToString(), *OtherActor->GetName());
}
