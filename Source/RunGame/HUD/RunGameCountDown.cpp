// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RunGameCountDown.h"
#include "Game/RunGameGameState.h"

void URunGameCountDown::NativeConstruct()
{
	Super::NativeConstruct();

	// 获取 GameState 并绑定 OnCountdownUpdated 委托 Get GameState and bind to OnCountdownUpdated delegate
	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnCountdownUpdated.AddDynamic(this, &URunGameCountDown::OnCountdownReceived);
		// 同步当前倒计时状态，防止 UI 滞后 Sync current countdown to avoid UI lag
		OnCountdownReceived(GS->GetCountdownSeconds());
	}
}

void URunGameCountDown::NativeDestruct()
{
	// 销毁时移除绑定，防止内存泄漏和野指针 Remove bindings on destruction to avoid leaks and dangling pointers
	if (ARunGameGameState* GS = GetWorld()->GetGameState<ARunGameGameState>())
	{
		GS->OnCountdownUpdated.RemoveDynamic(this, &URunGameCountDown::OnCountdownReceived);
	}

	Super::NativeDestruct();
}


void URunGameCountDown::OnCountdownReceived(int32 CurrentTime)
{
	// 收到 GameState 的倒计时更新，让蓝图播动画 Receive countdown update, let Blueprint play animation
	PlayNumberAnimation(CurrentTime);
}
