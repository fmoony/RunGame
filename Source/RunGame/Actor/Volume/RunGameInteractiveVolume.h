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
	ARunGameInteractiveVolume();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* CollisionComp;

	// 核心：碰撞重叠回调（基类处理通用逻辑）
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	// 核心：虚函数，子类重写（C++ 用）
	// 只保留 Begin，End 可选，跑酷游戏 90% 只用 Begin
	UFUNCTION(BlueprintNativeEvent, Category = "RunGame|Interaction")
	void OnPlayerEnter(ARunGameCharacter* PlayerCharacter);

	virtual void OnPlayerEnter_Implementation(ARunGameCharacter* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, Category = "RunGame|Interaction")
	void OnPlayerLeave(ARunGameCharacter* PlayerCharacter);
	
	virtual void OnPlayerLeave_Implementation(ARunGameCharacter* PlayerCharacter);

public:
	// 委托：蓝图绑定用
	UPROPERTY(BlueprintAssignable, Category = "RunGame|Interaction")
	FOnInteractionBegin OnPlayerEnterDelegate;

	UPROPERTY(BlueprintAssignable, Category = "RunGame|Interaction")
	FOnInteractionEnd OnPlayerLeaveDelegate;
};