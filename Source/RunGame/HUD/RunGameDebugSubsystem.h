#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameType.h"
#include "RunGameDebugSubsystem.generated.h"

class URunGameDebugPanel;

/**
 * 调试子系统 —— 在游戏开始时自动创建并显示调试面板。
 * 面板延迟到 GameState 进入 CountDown 后创建，确保 PlayerController 已就绪。
 * 所有调试 UI 逻辑集中在此文件，方便后续直接删除。
 * Debug subsystem — creates debug panel after game starts, ensuring PlayerController is ready.
 */
UCLASS()
class RUNGAME_API URunGameDebugSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	UFUNCTION()
	void OnGameStateChanged(ERunGameGameState OldState, ERunGameGameState NewState);

	void CreateDebugPanel();

	UPROPERTY()
	TObjectPtr<URunGameDebugPanel> DebugPanel;

	bool bPanelCreated = false;
};
