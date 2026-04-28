// Fill out your copyright notice in the Description page of Project Settings.

#include "RunGameInteractiveVolume.h"
#include "RunGameCharacter.h"
#include "Components/BoxComponent.h"

ARunGameInteractiveVolume::ARunGameInteractiveVolume()
{
	// 创建Box组件，并作为根组件
    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;
    
    // 设置体积大小
    CollisionComp->SetBoxExtent(FVector(200.f, 200.f, 1.f));
 
    // 启用Overlap事件
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

	// 基类只做这 3 件事：
	// 1. 只在服务器端执行（服务器权威）
	//if (GetLocalRole() != ROLE_Authority) return;

	// 2. 检查是否是玩家角色
	if (ARunGameCharacter* PlayerCharacter = Cast<ARunGameCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("ARunGameInteractiveVolume: Player entered volume: %s"), *GetName());
		// 3. 调用虚函数，子类重写
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

	//if (GetLocalRole() != ROLE_Authority) return;
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

// 默认实现：空函数
void ARunGameInteractiveVolume::OnPlayerEnter_Implementation(ARunGameCharacter* PlayerCharacter)
{
	// 蓝图可以绑定委托，C++ 子类重写这个函数
	OnPlayerEnterDelegate.Broadcast(PlayerCharacter);
}

void ARunGameInteractiveVolume::OnPlayerLeave_Implementation(ARunGameCharacter* PlayerCharacter)
{
	OnPlayerLeaveDelegate.Broadcast(PlayerCharacter);
}