#include "Actor/Trap/Trap.h"
#include "Components/StaticMeshComponent.h"
#include "Actor/Component/HealthComponent.h"
#include "Actor/Component/DamageDealerComponent.h"
#include "Actor/Floor/FloorBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = false;

	// 网格体即根组件 — 视觉 + IImpactReceiver 碰撞触发器 Mesh as root — visuals + IImpactReceiver collision trigger
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// 仅查询模式 — 只与 Pawn 重叠，供 CollisionAbilityComponent 检测 Only query — overlap Pawn for CollisionAbilityComponent detection
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetGenerateOverlapEvents(true);

	// 生命值组件 — 统一管理 HP / 受伤 / 死亡委托 Health component — unified HP / damage / death delegation
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	// 伤害组件 — 角色触碰时对角色造成伤害 Damage dealer — damages characters on contact
	DamageDealer = CreateDefaultSubobject<UDamageDealerComponent>(TEXT("DamageDealer"));
	DamageDealer->SetupAttachment(RootComponent);
}

void ATrap::BeginPlay()
{
	Super::BeginPlay();

	// 死亡委托 — HP 归零时触发视觉损毁 Death delegate — trigger visual destruction when HP reaches zero
	HealthComponent->OnDeath.AddDynamic(this, &ATrap::OnTrapDeath);

	// 初始激活 Activate on first spawn
	ActivateTrap();
}

// ===== 池子生命周期 Pool lifecycle =====

void ATrap::ActivateTrap()
{
	// 复活 — 恢复 HP 到完整值 Revive — restore HP to full
	HealthComponent->Revive(HealthComponent->MaxHP);

	// 恢复可见 + 碰撞 Restore visibility + collision
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 恢复伤害组件碰撞 — 角色触碰时造成伤害 Restore damage dealer collision — damage characters on contact
	DamageDealer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SetActorHiddenInGame(false);
}

void ATrap::DeactivateTrap()
{
	// 隐藏 + 禁用所有碰撞 Hide + disable all collision
	// 不修改 HP — 由 ActivateTrap 在下次激活时 Revive 重置
	// Don't touch HP — ActivateTrap will Revive on next activation
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageDealer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
}

// ===== 冲击接收 → 转发到 HealthComponent Impact receiver → forward to HealthComponent =====

void ATrap::ReceiveImpact_Implementation(FGameplayTag SkillTag, const FImpactInfo& Impact)
{
	// 已死亡则忽略 — HealthComponent 会拒绝 0 HP 以下的伤害
	// Ignore if already dead — HealthComponent rejects damage below 0 HP
	if (HealthComponent->IsDead())
	{
		return;
	}

	// 转发到 HealthComponent — 统一 HP 管理
	// Forward to HealthComponent — unified HP management
	HealthComponent->ApplyDamage(Impact.Force, SkillTag, Impact.Instigator.Get());
}

// ===== 死亡回调 Death callback =====

void ATrap::OnTrapDeath(FGameplayTag DamageType, AActor* DeathCauser)
{
	// 使用 DeathCauser 位置作为 VFX 生成点 Use death causer location as VFX spawn point
	const FVector SpawnPoint = DeathCauser ? DeathCauser->GetActorLocation() : GetActorLocation();
	BreakTrap(SpawnPoint);
}

// ===== 视觉损毁 Visual destruction =====

void ATrap::BreakTrap(const FVector& ImpactPoint)
{
	// 禁用所有碰撞 — 防止短时间内重复触发 Disable all collision — prevent rapid re-triggering
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageDealer->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 隐藏网格体 Hide mesh
	Mesh->SetVisibility(false);

	// 在碰撞点生成摧毁特效 Spawn destruction VFX at impact point
	if (DestroyEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DestroyEffect,
			ImpactPoint
		);
	}

	// 播放摧毁音效 Play destruction sound
	if (DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySound, ImpactPoint);
	}

	// 通知蓝图子类（掉落物、计分等）Notify Blueprint subclass (drops, scoring, etc.)
	OnTrapDestroyed();
}
