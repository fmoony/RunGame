// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameCountDown.generated.h"

/**
 * 
 */
UCLASS()
class RUNGAME_API URunGameCountDown : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 1. 接收 GameState 广播的回调函数
	UFUNCTION()
	void OnCountdownReceived(int32 CurrentTime);

public:
	// 当 C++ 调用它时，引擎会自动去执行蓝图中对应的事件节点。
	UFUNCTION(BlueprintImplementableEvent, Category = "RunGame|Animation")
	void PlayNumberAnimation(int32 CurrentTime);

};
