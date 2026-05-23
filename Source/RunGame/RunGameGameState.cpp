#include "RunGameGameState.h"
#include "WorldSubsystem/State/GameFlowRuntimeState.h"
#include "RunGame.h"

ARunGameGameState::ARunGameGameState()
{
}

void ARunGameGameState::BeginPlay()
{
	Super::BeginPlay();

	if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		// 同步配置默认值到 RS
		RS->DefaultCountdownSeconds = DefaultCountdownSeconds;
		RS->DefaultGameTotalTime = DefaultGameTotalTime;

		// 绑定转发器：RS 广播 → 重播自己的委托
		RS->OnGameStateChanged.AddDynamic(this, &ARunGameGameState::OnRS_GameStateChanged);
		RS->OnCountdownUpdated.AddDynamic(this, &ARunGameGameState::OnRS_CountdownUpdated);
		RS->OnCountdownComplete.AddDynamic(this, &ARunGameGameState::OnRS_CountdownComplete);
	}
}

void ARunGameGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		RS->OnGameStateChanged.RemoveDynamic(this, &ARunGameGameState::OnRS_GameStateChanged);
		RS->OnCountdownUpdated.RemoveDynamic(this, &ARunGameGameState::OnRS_CountdownUpdated);
		RS->OnCountdownComplete.RemoveDynamic(this, &ARunGameGameState::OnRS_CountdownComplete);
	}

	Super::EndPlay(EndPlayReason);
}

void ARunGameGameState::SetGameState(ERunGameGameState NewState)
{
	if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		RS->SetGameState(NewState);
	}
}

ERunGameGameState ARunGameGameState::GetCurrentState() const
{
	if (const UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		return RS->GetGameState();
	}
	return ERunGameGameState::MainMenu;
}

void ARunGameGameState::SetCountdownSeconds(int32 NewCountdownSeconds)
{
	if (UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		RS->SetCountdownSeconds(NewCountdownSeconds);
	}
}

int32 ARunGameGameState::GetCountdownSeconds() const
{
	if (const UGameFlowRuntimeState* RS = GetWorld()->GetSubsystem<UGameFlowRuntimeState>())
	{
		return RS->GetCountdownSeconds();
	}
	return 0;
}

// ---- RS 转发器 ----

void ARunGameGameState::OnRS_GameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState)
{
	OnGameStateChanged.Broadcast(OldState, NewState);
}

void ARunGameGameState::OnRS_CountdownUpdated(int32 CountdownSeconds)
{
	OnCountdownUpdated.Broadcast(CountdownSeconds);
}

void ARunGameGameState::OnRS_CountdownComplete()
{
	// GameState 本身不持 OnCountdownComplete 委托（由 TimerSubsystem 持有）
	// 如果后续需要，可在此添加转发
}
