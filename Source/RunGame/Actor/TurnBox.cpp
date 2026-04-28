// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/TurnBox.h"
#include "Components/BoxComponent.h"
#include "RunGameCharacter.h"

// Sets default values
ATurnBox::ATurnBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	check(Box);

	// 【防御性代码】确保不管蓝图怎么改，物理碰撞一定是对的
	Box->SetGenerateOverlapEvents(true); // 开启重叠事件
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 只需要查询，不需要物理阻挡
	Box->SetCollisionResponseToAllChannels(ECR_Ignore); // 默认忽略所有东西
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 唯独对玩家(Pawn)产生重叠

}

// Called when the game starts or when spawned
void ATurnBox::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATurnBox::CharaterOverlapStart);
	Box->OnComponentEndOverlap.AddDynamic(this, &ATurnBox::CharaterOverlapEnd);
}

// Called every frame
void ATurnBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurnBox::CharaterOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunGameCharacter* InCharater = Cast<ARunGameCharacter>(OtherActor))
	{
		InCharater->bTurn = true;
		InCharater->InTurnBox = true;
		UE_LOG(LogTemp, Warning, TEXT("Character entered TurnBox: %s"), *GetNameSafe(this));
	}
}

void ATurnBox::CharaterOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARunGameCharacter* InCharater = Cast<ARunGameCharacter>(OtherActor))
	{
		InCharater->bTurn = false;
		InCharater->InTurnBox = false;
		UE_LOG(LogTemp, Warning, TEXT("Character leaved TurnBox: %s"), *GetNameSafe(this));
	}
}

