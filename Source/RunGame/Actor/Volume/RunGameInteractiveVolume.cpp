// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Volume/RunGameInteractiveVolume.h"
#include "Character/RunGameCharacter.h"
#include "Components/BoxComponent.h"

ARunGameInteractiveVolume::ARunGameInteractiveVolume()
{
	// 创建Box碰撞体作为根组件 Create Box collision as root component
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	// 设置碰撞体大小 Set collision size
	CollisionComp->SetBoxExtent(FVector(200.f, 200.f, 1.f));

	// 绑定Overlap事件 Bind overlap events
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComp->SetGenerateOverlapEvents(true);

	PrimaryActorTick.bCanEverTick = false;
}

void ARunGameInteractiveVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ARunGameInteractiveVolume: NotifyActorBeginOverlap called with OtherActor: %s"), *OtherActor->GetName());
	Super::NotifyActorBeginOverlap(OtherActor);

	if (ARunGameCharacter* PlayerCharacter = Cast<ARunGameCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("ARunGameInteractiveVolume: Player entered volume: %s"), *GetName());
		// 调用虚函数让子类重写 Call virtual function for subclasses to override
		OnPlayerEnter(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARunGameInteractiveVolume: Overlapped actor is not ARunGameCharacter: %s"), *OtherActor->GetName());
	}
}

void ARunGameInteractiveVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ARunGameInteractiveVolume: NotifyActorEndOverlap called with OtherActor: %s"), *OtherActor->GetName());
	Super::NotifyActorEndOverlap(OtherActor);

	if (ARunGameCharacter* PlayerCharacter = Cast<ARunGameCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("ARunGameInteractiveVolume: Player left volume: %s"), *GetName());
		OnPlayerLeave(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARunGameInteractiveVolume: EndOverlap actor is not ARunGameCharacter: %s"), *OtherActor->GetName());
	}
}

// 默认实现，空函数 Default implementation, no-op
// 蓝图可以绑委托，C++ 子类可以直接重写 Blueprint delegates, C++ subclass can override
void ARunGameInteractiveVolume::OnPlayerEnter_Implementation(ARunGameCharacter* PlayerCharacter)
{
	OnPlayerEnterDelegate.Broadcast(PlayerCharacter);
}

void ARunGameInteractiveVolume::OnPlayerLeave_Implementation(ARunGameCharacter* PlayerCharacter)
{
	OnPlayerLeaveDelegate.Broadcast(PlayerCharacter);
}
