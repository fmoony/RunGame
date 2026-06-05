// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Component/DamageDealerComponent.h"
#include "Components/PrimitiveComponent.h"
#include "RunGame.h"
#include "Interfaces/Damagable.h"

UDamageDealerComponent::UDamageDealerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDamageDealerComponent::BeginPlay()
{
	Super::BeginPlay();

	BindToOwnerMesh();
}

void UDamageDealerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindFromOwnerMesh();

	Super::EndPlay(EndPlayReason);
}

void UDamageDealerComponent::BindToOwnerMesh()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// 拿根组件作为碰撞体——Trap 的 Mesh 就是根，其他 Actor 同理
	// Use root component as collision primitive — Trap's Mesh is root, same for other actors
	UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Owner->GetRootComponent());
	if (!Prim)
	{
		UE_LOG(LogRunGame, Error, TEXT("UDamageDealerComponent: Owner %s has no UPrimitiveComponent as root"), *Owner->GetName());
		return;
	}

	Prim->OnComponentBeginOverlap.AddDynamic(this, &UDamageDealerComponent::OnTrapOverlap);
	BoundPrimitive = Prim;
}

void UDamageDealerComponent::UnbindFromOwnerMesh()
{
	if (BoundPrimitive)
	{
		BoundPrimitive->OnComponentBeginOverlap.RemoveDynamic(this, &UDamageDealerComponent::OnTrapOverlap);
		BoundPrimitive = nullptr;
	}
}

void UDamageDealerComponent::OnTrapOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner() && OtherActor->Implements<UDamagable>())
	{
		IDamagable::Execute_OnTakeDamage(OtherActor, DamageAmount, DamageType, GetOwner());
		UE_LOG(LogRunGame, Warning, TEXT("UDamageDealerComponent: Dealt %f damage to %s with type %s"), DamageAmount, *OtherActor->GetName(), *DamageType.ToString());
	}
}
