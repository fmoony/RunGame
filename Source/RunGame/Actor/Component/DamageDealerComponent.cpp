// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Component/DamageDealerComponent.h"
#include "Interfaces/Damagable.h"

UDamageDealerComponent::UDamageDealerComponent()
{
	SetCollisionProfileName(TEXT("Trigger"));

	InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	SetGenerateOverlapEvents(true);
}

void UDamageDealerComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UDamageDealerComponent::OnTrapOverlap);
}

void UDamageDealerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnComponentBeginOverlap.RemoveDynamic(this, &UDamageDealerComponent::OnTrapOverlap);

	Super::EndPlay(EndPlayReason);
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
		UE_LOG(LogTemp, Warning, TEXT("UDamageDealerComponent: Dealt %f damage to %s with type %s"), DamageAmount, *OtherActor->GetName(), *DamageType.ToString());
	}
}
