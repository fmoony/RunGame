#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RunGameDebugSubsystem.generated.h"

class URunGameDebugPanel;

/**
 * 调试子系统 —— 在 World 启动时自动创建并显示调试面板。
 * 所有调试 UI 逻辑集中在此文件，方便后续直接删除。
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
	UPROPERTY()
	TObjectPtr<URunGameDebugPanel> DebugPanel;
};
