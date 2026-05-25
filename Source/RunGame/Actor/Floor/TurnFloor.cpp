// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Floor/TurnFloor.h"
#include "RunGameType.h"
#include "Components/BoxComponent.h"
#include "Character/RunGameCharacter.h"

ATurnFloor::ATurnFloor()
{
	FloorType = FFloorType::TurnFloor;

	TurnBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnBox"));
	TurnBoxComponent->SetupAttachment(ScenceComponent);
	TurnBoxComponent->SetGenerateOverlapEvents(true);
	TurnBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TurnBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TurnBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATurnFloor::BeginPlay()
{
	Super::BeginPlay();

	TurnBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATurnFloor::OnTurnBoxBeginOverlap);
	TurnBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATurnFloor::OnTurnBoxEndOverlap);
}

void ATurnFloor::OnTurnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunGameCharacter* InCharacter = Cast<ARunGameCharacter>(OtherActor))
	{
		InCharacter->SetCharacterState(ERunGameCharacterState::Turning);
	}
}

void ATurnFloor::OnTurnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARunGameCharacter* InCharacter = Cast<ARunGameCharacter>(OtherActor))
	{
		// 只有在仍处于 Turning 状态时才切回 Idle
		// 如果角色已死亡等，则不覆盖
		if (InCharacter->GetCharacterState() == ERunGameCharacterState::Turning)
		{
			InCharacter->SetCharacterState(ERunGameCharacterState::Idle);
		}
	}
}
