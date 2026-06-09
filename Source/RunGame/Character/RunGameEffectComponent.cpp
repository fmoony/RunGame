#include "Character/RunGameEffectComponent.h"
#include "WorldSubsystem/State/PlayerRuntimeState.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "RunGame.h"

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
		CachedPRS->OnCharacterStateChanged.AddDynamic(this, &URunGameEffectComponent::OnCharacterStateChanged);
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
	OriginalMaterials.Empty();

	if (CachedPRS)
	{
		CachedPRS->OnEffectTagChanged.RemoveDynamic(this, &URunGameEffectComponent::OnEffectTagChanged);
		CachedPRS->OnDeathAnimationFinished.RemoveDynamic(this, &URunGameEffectComponent::OnDissolveTriggered);
		CachedPRS->OnCharacterStateChanged.RemoveDynamic(this, &URunGameEffectComponent::OnCharacterStateChanged);
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

	// 缓存原始材质 — 重生时复原 Cache original materials — restore on respawn
	OriginalMaterials.Empty();
	const int32 NumMaterials = Mesh->GetNumMaterials();
	for (int32 i = 0; i < NumMaterials; ++i)
	{
		OriginalMaterials.Add(Mesh->GetMaterial(i));
	}

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
}

void URunGameEffectComponent::ResetDissolveMaterials()
{
	UE_LOG(LogRunGame, Warning, TEXT("EffectComponent::ResetDissolveMaterials — Count=%d"), DissolveMaterials.Num());

	// 恢复原始材质 — 溶解期间创建的 DMI 可能无法正确复位，直接还原
	// Restore original materials — DMI created during dissolve may not reset correctly
	if (DissolveMaterials.Num() > 0)
	{
		if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
		{
			if (USkeletalMeshComponent* Mesh = Char->GetMesh())
			{
				for (int32 i = 0; i < FMath::Min(DissolveMaterials.Num(), Mesh->GetNumMaterials()); ++i)
				{
					Mesh->SetMaterial(i, OriginalMaterials.IsValidIndex(i) ? OriginalMaterials[i] : nullptr);
				}
			}
		}
	}

	DissolveMaterials.Empty();
}

void URunGameEffectComponent::OnCharacterStateChanged(ERunGameCharacterState OldState, ERunGameCharacterState NewState)
{
	UE_LOG(LogRunGame, Warning, TEXT("EffectComponent::OnCharacterStateChanged: %d → %d"), (int32)OldState, (int32)NewState);
	// 从 Dead 恢复 → 还原溶解材质 Leaving Dead → restore dissolve materials
	if (OldState == ERunGameCharacterState::Dead && NewState != ERunGameCharacterState::Dead)
	{
		UE_LOG(LogRunGame, Warning, TEXT("EffectComponent: ResetDissolveMaterials"));
		ResetDissolveMaterials();
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
