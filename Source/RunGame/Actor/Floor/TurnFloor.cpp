// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Floor/TurnFloor.h"
#include "RunGameType.h"
#include "Components/BoxComponent.h"
#include "RunGameCharacter.h"

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
	if (ARunGameCharacter* InCharater = Cast<ARunGameCharacter>(OtherActor))
	{
		InCharater->bTurn = true;
		InCharater->InTurnBox = true;
	}
}

void ATurnFloor::OnTurnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARunGameCharacter* InCharater = Cast<ARunGameCharacter>(OtherActor))
	{
		InCharater->bTurn = false;
		InCharater->InTurnBox = false;
	}
}
