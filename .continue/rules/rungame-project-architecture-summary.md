---
description: RunGame AIGC 项目的完整架构总结，包含文件结构、关键 API、数据流、对象池机制和初始化流程。当用户询问项目结构、架构或需要了解数据流时使用。
alwaysApply: false
---

## RunGame 项目架构总结

### 架构模式
- **GameMode → 游戏流程控制**（倒计时、玩家生成、计时器、分数、死亡处理）
- **WorldSubsystem → 功能模块解耦**（地板管理、计时器各归各的 Subsystem）
- **对象池 → 复用地板**（替代 Spawn/Destroy 减少 GC 压力）

### 核心文件结构
```
Source/RunGame/
├── RunGameGameMode.h/.cpp          ← 仅负责游戏流程
├── RunGamePlayerState.h/.cpp       ← 分数管理（AddScore/SetRunGameScore/GetRunGameScore）
├── RunGameGameState.h/.cpp         ← 游戏状态、倒计时秒数
├── Actor/
│   ├── Floor.h/.cpp                ← 地板（BoxOverlap → 调用 FloorSubsystem）
│   └── TurnBox.h/.cpp              ← 转向触发盒（设置 Character 的 bTurn 标志）
├── HUD/
│   └── RunGameInGame.h/.cpp        ← UI（绑定 PlayerState.OnScoreChanged 和 TimerSubsystem.OnTimeChanged）
└── WorldSubsystem/
    ├── RunGameTimerSubsystem.h/.cpp ← 计时 + 分数累积（Tick 驱动）
    └── RunGameFloorSubsystem.h/.cpp ← 地板对象池 + 随机生成
```

### 关键 API
| 函数 | 位置 | 用途 |
|------|------|------|
| `AddScore(Value)` | PlayerState | 累加分数并广播 |
| `SetRunGameScore(NewScore)` | PlayerState | 设置分数并广播 |
| `GetRunGameScore()` | PlayerState | 获取当前分数 |
| `InitializeFloorSystem()` | FloorSubsystem | 初始化地板蓝图类和对象池 |
| `SpawnInitialFloors()` | FloorSubsystem | 生成初始地板链 |
| `RequestNextFloor()` | FloorSubsystem | 随机生成下一块地板 |
| `ReturnFloor()` | FloorSubsystem | 归还地板到对象池 |
| `StartTimer()` | TimerSubsystem | 启动计时和分数累积 |
| `GetRunGameScore()` | TimerSubsystem | 从 PlayerState 获取分数 |

### 数据流
```
TimerSubsystem::Tick
  → UpdateTimer(DeltaTime)
    → 每 0.1s: AddScore(分数增量)    ← 分数累加触发点
    → OnTimeChanged.Broadcast()     ← 时间 UI 更新

Floor::BoxOverlap (当玩家踩到地板)
  → FloorSystem->RequestNextFloor()  ← 生成下一块（对象池）
  → 延迟 2.2s: FloorSystem->ReturnFloor(this)  ← 回收当前地板

BP_TurnLeftFloor (蓝图)
  → 注意：旧蓝图中可能有 Destroy Actor 逻辑，与新对象池冲突
  → 需在蓝图中移除 Destroy 逻辑，改为被动等待 ReturnFloor 回收
```

### 对象池机制
- FloorSubsystem 预分配 PreAllocateCount 个地板隐藏在世界外
- `ReturnFloor()`：将地板移到 (-100000,0,0) 并隐藏
- `RequestNextFloor()`：从池取出并放置到 NextSpawnTransform 位置
- 每块地板用 `Floor->GetAttachToTransform()` 计算下一块位置

### 初始化流程
1. GameMode::BeginPlay → FloorSystem->InitializeFloorSystem()
2. FloorSystem->SpawnInitialFloors() 生成 5+15 块地板
3. GameMode::StartGameCountDown → TimerSubsystem->StartCountdown()
4. 倒计时结束 → GameMode::SpawnPlayer() → TimerSubsystem->StartTimer()
5. TimerSubsystem 开始 Tick，每 0.1s 累加分数

### 常见问题排查
- **分数不增加**：检查 TimerSubsystem 是否注册到项目设置、`bIsTimerRunning` 是否为 true、`GetPlayerState()` 是否返回 null
- **BP 报错 "Accessed None"**：旧蓝图仍有 Destroy Actor 逻辑，需移除
- **地板不生成**：检查 GameMode 蓝图中 StraightFloorClasses/TurnFloorClasses 是否已配置
