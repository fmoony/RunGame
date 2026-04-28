# 分数和计时系统使用指南

## 概述
本系统为RunGame项目添加了实时分数和计时显示功能，采用事件驱动架构，避免UI中不必要的Tick更新。

## 架构设计

### 1. 数据层 (RunGameTimerSubsystem)
- **职责**: 管理游戏分数和计时数据
- **特性**: 事件驱动，蓝图友好，自动Tick更新
- **核心功能**:
  - 分数管理: `AddScore()`, `SetScore()`, `GetScore()`
  - 计时管理: `StartTimer()`, `StopTimer()`, `UpdateTimer()`
  - 委托广播: `OnScoreChanged`, `OnTimeChanged`

### 2. UI层 (RunGameInGame)
- **职责**: 显示实时分数和计时信息
- **特性**: 委托监听，自动更新，无需Tick
- **核心组件**:
  - `ScoreText`: 显示当前分数
  - `TimerText`: 显示剩余时间 (MM:SS.ms格式)

## 使用方法

### 1. 蓝图集成

#### 在GameMode中初始化
```cpp
// 在SpawnPlayer函数中调用
void ARunGameGameMode::SpawnPlayer()
{
    // ... 玩家生成逻辑 ...
    
    // 初始化游戏计时器
    InitializeGameTimer();
}
```

#### 奖励分数
```cpp
// 玩家收集物品时调用
void AMyCharacter::CollectItem()
{
    if (ARunGameGameMode* GameMode = GetWorld()->GetAuthGameMode<ARunGameGameMode>())
    {
        GameMode->AwardScore(100); // 奖励100分
    }
}
```

#### 重置游戏
```cpp
// 游戏结束时调用
void ARunGameGameMode::ResetGame()
{
    // 重置分数和计时器
    if (URunGameTimerSubsystem* TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>())
    {
        TimerSubsystem->SetScore(0);
        TimerSubsystem->StopTimer();
    }
}
```

### 2. 蓝图设置

#### UI Widget配置
1. 创建基于`RunGameInGame`的蓝图Widget
2. 添加两个`Text Block`组件:
   - 命名为`ScoreText` (用于显示分数)
   - 命名为`TimerText` (用于显示时间)
3. 在HUD的UI映射中添加`InGame`状态对应的Widget类

#### GameMode配置
1. 选择GameMode蓝图
2. 在Details面板找到`RunGame|Game`分类
3. 设置`Game Total Time`属性 (默认60秒)
4. 设置`Game Character Class`属性

### 3. 事件监听示例

#### 在其他Actor中监听分数变化
```cpp
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    
    if (URunGameTimerSubsystem* TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>())
    {
        TimerSubsystem->OnScoreChanged.AddDynamic(this, &AMyActor::OnScoreChanged);
    }
}

void AMyActor::OnScoreChanged(int32 NewScore)
{
    UE_LOG(LogTemp, Warning, TEXT("Score changed to: %d"), NewScore);
}

void AMyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (URunGameTimerSubsystem* TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>())
    {
        TimerSubsystem->OnScoreChanged.RemoveDynamic(this, &AMyActor::OnScoreChanged);
    }
    
    Super::EndPlay(EndPlayReason);
}
```

## 核心API参考

### RunGameTimerSubsystem

#### 分数相关
- `void AddScore(int32 Value)` - 增加分数并广播事件
- `void SetScore(int32 NewScore)` - 设置分数并广播事件
- `int32 GetScore() const` - 获取当前分数

#### 计时相关
- `void StartTimer(float TotalTime)` - 启动计时器
- `void StopTimer()` - 停止计时器
- `float GetTimeRemaining() const` - 获取剩余时间
- `void UpdateTimer(float DeltaTime)` - 更新计时器 (自动调用)

#### 倒计时相关 (原有功能)
- `void StartCountdown(int32 InitialSeconds)` - 启动游戏开始倒计时
- `void StopCountdown()` - 停止倒计时
- `int32 GetCurrentCountdownSeconds() const` - 获取倒计时秒数

#### 委托
- `FOnScoreChangedDelegate OnScoreChanged` - 分数变更事件
- `FOnTimeChangedDelegate OnTimeChanged` - 时间变更事件
- `FOnCountdownCompleteDelegate OnCountdownComplete` - 倒计时完成事件

### RunGameGameMode

#### 游戏控制
- `void InitializeGameTimer()` - 初始化游戏计时器
- `void AwardScore(int32 Amount)` - 奖励分数
- `void ResetGame()` - 重置游戏

### RunGameInGame (UI)

#### 内部函数
- `void OnScoreUpdated(int32 NewScore)` - 分数更新回调
- `void OnTimerUpdated(float NewTime)` - 时间更新回调
- `FString FormatTimeText(float TimeSeconds)` - 格式化时间文本

## 性能优化

### 1. 事件驱动架构
- 避免UI中不必要的Tick更新
- 仅在数据变化时更新UI
- 使用委托机制实现松耦合

### 2. 内存管理
- 使用TObjectPtr管理引用
- 在NativeDestruct中正确解绑委托
- 避免内存泄漏

### 3. 蓝图性能
- 所有数据暴露给蓝图，减少C++调用
- 使用BindWidget元数据，避免手动查找组件

## 扩展建议

### 1. 添加连击系统
```cpp
// 在TimerSubsystem中添加
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RunGame|Score")
int32 ComboCount;

void AddScore(int32 Value, bool bIsCombo = false)
{
    if (bIsCombo)
    {
        ComboCount++;
        Value *= ComboCount; // 连击加成
    }
    else
    {
        ComboCount = 0;
    }
    Score += Value;
    OnScoreChanged.Broadcast(Score);
}
```

### 2. 添加时间奖励
```cpp
// 收集物品时增加时间
void AwardTime(float BonusTime)
{
    if (URunGameTimerSubsystem* TimerSubsystem = GetWorld()->GetSubsystem<URunGameTimerSubsystem>())
    {
        float CurrentTime = TimerSubsystem->GetTimeRemaining();
        TimerSubsystem->StartTimer(CurrentTime + BonusTime);
    }
}
```

### 3. 添加分数等级系统
```cpp
// 根据分数显示不同等级
FString GetScoreLevel(int32 Score)
{
    if (Score >= 10000) return TEXT("S");
    if (Score >= 5000) return TEXT("A");
    if (Score >= 3000) return TEXT("B");
    if (Score >= 1000) return TEXT("C");
    return TEXT("D");
}
```

## 故障排除

### 1. UI不更新
- 检查Text Block组件是否正确命名为`ScoreText`和`TimerText`
- 确认Widget已添加到HUD的UI映射中
- 查看日志中的错误信息

### 2. 委托不触发
- 确认TimerSubsystem已正确初始化
- 检查委托是否正确绑定和解绑
- 验证广播函数是否被调用

### 3. 计时不准确
- 确认UpdateTimer在Tick中正确调用
- 检查DeltaTime参数是否正确传递
- 验证bIsTimerRunning标志是否正确设置

## 总结

本系统提供了完整的分数和计时解决方案，具有以下优势:
- **高性能**: 事件驱动，避免不必要的更新
- **易扩展**: 清晰的架构，便于添加新功能
- **蓝图友好**: 所有功能都可从蓝图调用
- **类型安全**: 使用强类型委托，减少运行时错误
- **内存安全**: 正确的引用管理和委托清理

通过合理使用此系统，可以轻松实现各种游戏机制，如计分、计时、连击、等级等功能。