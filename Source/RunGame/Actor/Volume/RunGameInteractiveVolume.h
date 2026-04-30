// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RunGameInteractiveVolume.generated.h"

// 前向声明
class ARunGameCharacter;
class UBoxComponent;

// 动态多播委托（方便蓝图绑定）
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionBegin, ARunGameCharacter*, PlayerCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionEnd, ARunGameCharacter*, PlayerCharacter);

/**
 * 极简通用交互体积基类 - 专为跑酷游戏设计
 */
UCLASS(Abstract, Blueprintable)
class RUNGAME_API ARunGameInteractiveVolume : public AVolume
{
	GENERATED_BODY()

public:
	/** Constructs the interactive volume with a default collision box */
	ARunGameInteractiveVolume();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* CollisionComp;

	/** Detects actor entry into the volume and fires the player-enter event */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** Detects actor exit from the volume and fires the player-leave event */
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	/** Default C++ implementation of OnPlayerEnter -- fires the delegate */
	/** BlueprintNativeEvent: called when a RunGame character enters this volume */
	UFUNCTION(BlueprintNativeEvent, Category = "RunGame|Interaction")
	void OnPlayerEnter(ARunGameCharacter* PlayerCharacter);

	/** Default C++ implementation of OnPlayerEnter -- fires the delegate */
	virtual void OnPlayerEnter_Implementation(ARunGameCharacter* PlayerCharacter);

	/** BlueprintNativeEvent: called when a RunGame character leaves this volume */
	UFUNCTION(BlueprintNativeEvent, Category = "RunGame|Interaction")
	void OnPlayerLeave(ARunGameCharacter* PlayerCharacter);
	
	/** Default C++ implementation of OnPlayerLeave -- fires the delegate */
	virtual void OnPlayerLeave_Implementation(ARunGameCharacter* PlayerCharacter);

public:
	/** Delegate fired when a player interacts with this volume */
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Interaction")
	FOnInteractionBegin OnPlayerEnterDelegate;

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Interaction")
	FOnInteractionEnd OnPlayerLeaveDelegate;
};