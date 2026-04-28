// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RunGameCountDown.h"
#include "RunGameGameState.h"

void URunGameCountDown::NativeConstruct()
{
	Super::NativeConstruct();

	// 获取 GameState 并绑定我们刚刚写的广播代理 (OnCountdownUpdated)
	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnCountdownUpdated.AddDynamic(this, &URunGameCountDown::OnCountdownReceived);
		OnCountdownReceived(GS->GetCountdownSeconds()); // 同步当前倒计时状态，防止UI滞后
	}
}

void URunGameCountDown::NativeDestruct()
{
	// UI 被销毁时，解除绑定，防止内存泄漏或野指针
	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnCountdownUpdated.RemoveDynamic(this, &URunGameCountDown::OnCountdownReceived);
	}

	Super::NativeDestruct();
}


void URunGameCountDown::OnCountdownReceived(int32 CurrentTime)
{
	// 3. 当收到 GameState 发来的数字时，立刻呼叫蓝图去播放动画！
	PlayNumberAnimation(CurrentTime);
}