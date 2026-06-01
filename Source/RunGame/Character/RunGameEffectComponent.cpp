#include "Character/RunGameEffectComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

URunGameEffectComponent::URunGameEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URunGameEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// 缓存 PRS + 绑定效果标签变化和死亡溶解 Cache PRS + bind effect tag changes and death dissolve
	CachedPRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>();
	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.AddDynamic(this, &URunGameEffectComponent::OnEffectTagChanged);
		CachedPRS->OnDeathAnimationFinished.AddDynamic(this, &URunGameEffectComponent::OnDissolveTriggered);
	}
}

void URunGameEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyEffect(InvincibilityFX);
	DestroyEffect(DissolveFX);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DissolveTickTimer);
		World->GetTimerManager().ClearTimer(DissolveDestroyTimer);
	}

	DissolveMaterials.Empty();

	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.RemoveDynamic(this, &URunGameEffectComponent::OnEffectTagChanged);
		CachedPRS->OnDeathAnimationFinished.RemoveDynamic(this, &URunGameEffectComponent::OnDissolveTriggered);
		CachedPRS = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// ---- Invincibility ----

void URunGameEffectComponent::OnEffectTagChanged(FGameplayTag Tag, bool bAdded)
{
	if (!IsTagVisualRelevant(Tag)) return;

	if (bAdded)
	{
		SpawnEffect(InvincibilityNiagara, InvincibilityFX);
	}
	else
	{
		// 检查是否还有其他无敌标签在活跃 Check if other invincibility tags are still active
		bool bStillActive = false;
		if (CachedPRS)
		{
			FGameplayTagContainer SingleTagContainer;
			for (const FGameplayTag& ActiveTag : CachedPRS->ActiveEffectTags)
			{
				if (ActiveTag == Tag) continue;  // skip the one being removed
				SingleTagContainer.Reset();
				SingleTagContainer.AddTag(ActiveTag);
				if (SingleTagContainer.MatchesQuery(InvincibilityTagQuery))
				{
					bStillActive = true;
					break;
				}
			}
		}
		if (!bStillActive)
		{
			DestroyEffect(InvincibilityFX);
		}
	}
}

bool URunGameEffectComponent::IsTagVisualRelevant(FGameplayTag Tag) const
{
	if (InvincibilityTagQuery.IsEmpty()) return false;

	FGameplayTagContainer SingleTagContainer;
	SingleTagContainer.AddTag(Tag);
	return SingleTagContainer.MatchesQuery(InvincibilityTagQuery);
}

// ---- Dissolve ----

void URunGameEffectComponent::OnDissolveTriggered()
{
	// Niagara 粒子（可选）
	SpawnEffect(DissolveNiagara, DissolveFX);

	// 材质溶解动画 Material dissolve animation
	StartDissolveMaterial();

	// 到期销毁 Owner Destroy owner after dissolve duration
	GetWorld()->GetTimerManager().SetTimer(
		DissolveDestroyTimer,
		this,
		&URunGameEffectComponent::OnDissolveComplete,
		DissolveDuration,
		false
	);
}

void URunGameEffectComponent::StartDissolveMaterial()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (!Mesh) return;

	const int32 NumMaterials = Mesh->GetNumMaterials();
	for (int32 i = 0; i < NumMaterials; ++i)
	{
		if (UMaterialInterface* Mat = Mesh->GetMaterial(i))
		{
			UMaterialInstanceDynamic* DMI = UMaterialInstanceDynamic::Create(Mat, this);
			DissolveMaterials.Add(DMI);
			Mesh->SetMaterial(i, DMI);
		}
	}

	DissolveElapsed = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(
		DissolveTickTimer,
		this,
		&URunGameEffectComponent::TickDissolveMaterial,
		0.033f,
		true
	);
}

void URunGameEffectComponent::TickDissolveMaterial()
{
	DissolveElapsed += 0.033f;
	const float Alpha = FMath::Clamp(DissolveElapsed / DissolveDuration, 0.0f, 1.0f);

	for (UMaterialInstanceDynamic* DMI : DissolveMaterials)
	{
		if (DMI)
		{
			DMI->SetScalarParameterValue(DissolveParameterName, Alpha);
		}
	}

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DissolveTickTimer);
	}
}

void URunGameEffectComponent::OnDissolveComplete()
{
	if (AActor* Owner = GetOwner())
	{
		Owner->Destroy();
	}
}

// ---- Internal ----

void URunGameEffectComponent::SpawnEffect(UNiagaraSystem* System, TObjectPtr<UNiagaraComponent>& OutComponent)
{
	if (!System) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (!Mesh) return;

	OutComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		System,
		Mesh,
		AttachSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
	);
}

void URunGameEffectComponent::DestroyEffect(TObjectPtr<UNiagaraComponent>& Component)
{
	if (Component)
	{
		Component->DestroyComponent();
		Component = nullptr;
	}
}
