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
		CachedPRS->OnCharacterStateChanged.AddDynamic(this, &URunGameCollisionAbilityComponent::OnCharacterStateChanged);
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
	HitCooldowns.Empty();

	if (Capsule)
	{
		Capsule->OnComponentBeginOverlap.RemoveDynamic(this, &URunGameCollisionAbilityComponent::OnCapsuleOverlap);
	}

	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.RemoveDynamic(this, &URunGameCollisionAbilityComponent::OnEffectTagChanged);
		CachedPRS->OnCharacterStateChanged.RemoveDynamic(this, &URunGameCollisionAbilityComponent::OnCharacterStateChanged);
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

		// 所有匹配 Tag 已移除——清空 ActiveSkillTag，防止残留值被下次重叠误用
		// All matching tags removed — clear ActiveSkillTag to prevent stale value in next overlap
		if (ActiveTagCount <= 0)
		{
			ActiveSkillTag = FGameplayTag();
		}

		UE_LOG(LogRunGame, Warning, TEXT("CollisionAbilityComponent:Tag removed Tag:%s, Count:%d"), *Tag.ToString(), ActiveTagCount);
	}

	OnCollisionStateChanged.Broadcast();
}

// ---- Collision ----

void URunGameCollisionAbilityComponent::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	// 无活跃碰撞 Tag 则不处理 No active collision tag → nothing to do
	if (ActiveTagCount <= 0) return;

	// 死亡状态不产生碰撞 Dead characters don't deal impact damage
	if (CachedPRS && CachedPRS->GetCharacterState() == ERunGameCharacterState::Dead) return;

	// 只处理实现了 IImpactReceiver 的 Actor Only process actors that implement IImpactReceiver
	if (!OtherActor->Implements<UImpactReceiver>()) return;

	// 冷却检查 — 同一 Actor 在 HitCooldownDuration 内不重复命中
	// Cooldown check — skip if this actor was hit too recently
	const float Now = GetWorld()->GetTimeSeconds();
	if (const float* CooldownEnd = HitCooldowns.Find(OtherActor))
	{
		if (Now < *CooldownEnd)
		{
			return;
		}
	}

	// 记录冷却到期时间 Record cooldown expiry
	HitCooldowns.Add(OtherActor, Now + HitCooldownDuration);
	OnCollisionStateChanged.Broadcast();

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

// ---- State binding ----

void URunGameCollisionAbilityComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	// 角色死亡时清空命中冷却 — 复活后（新 Character）可立即重新命中所有 Trap
	// Clear hit cooldowns on death — new Character after respawn can re-hit all traps immediately
	if (NewState == ERunGameCharacterState::Dead)
	{
		HitCooldowns.Empty();
		OnCollisionStateChanged.Broadcast();
	}
}
