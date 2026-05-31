#include "Character/RunGameEffectComponent.h"
#include "Actor/Component/HealthComponent.h"
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

	AActor* Owner = GetOwner();
	if (!Owner) return;

	// 绑定无敌状态 Bind to invincibility
	HealthComp = Owner->FindComponentByClass<UHealthComponent>();
	if (HealthComp)
	{
		HealthComp->OnInvincibilityChanged.AddDynamic(this, &URunGameEffectComponent::OnInvincibilityChanged);
	}

	// 绑定死亡动画完成 → 溶解 Bind to death animation finished → dissolve
	if (UPlayerRuntimeState* PRS = GetWorld()->GetSubsystem<UPlayerRuntimeState>())
	{
		PRS->OnDeathAnimationFinished.AddDynamic(this, &URunGameEffectComponent::OnDissolveTriggered);
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

	if (HealthComp)
	{
		HealthComp->OnInvincibilityChanged.RemoveDynamic(this, &URunGameEffectComponent::OnInvincibilityChanged);
		HealthComp = nullptr;
	}

	if (UWorld* World = GetWorld())
	{
		if (UPlayerRuntimeState* PRS = World->GetSubsystem<UPlayerRuntimeState>())
		{
			PRS->OnDeathAnimationFinished.RemoveDynamic(this, &URunGameEffectComponent::OnDissolveTriggered);
		}
	}

	Super::EndPlay(EndPlayReason);
}

// ---- Invincibility ----

void URunGameEffectComponent::OnInvincibilityChanged(bool bNewInvincible)
{
	if (bNewInvincible)
	{
		SpawnEffect(InvincibilityNiagara, InvincibilityFX);
	}
	else
	{
		DestroyEffect(InvincibilityFX);
	}
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
