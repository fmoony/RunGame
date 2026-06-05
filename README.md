# RunGame

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.6-orange)](https://www.unrealengine.com/)

基于 UE5.6 的 3D 跑酷游戏，C++ 原生开发。采用反应式状态机架构，支持 Combat / Platforming / SideScrolling 三种玩法变体，集成 MCP 自动化桥接与外部 AI 编程助手深度协作。

## 架构概览

```
┌──────────────────────────────────────────────────┐
│                  GameState                        │
│           (Single Source of Truth)                │
│    MainMenu → CountDown → InGame ⇄ Pause → Over  │
└──────────┬───────────────────────────────────────┘
           │ OnGameStateChanged (委托广播)
    ┌──────┼──────┬──────────┬──────────┬──────────┐
    ▼      ▼      ▼          ▼          ▼          ▼
  Timer  HUD    Controller  Player    GameMode
  Subsys                            (Floor/Coin Pool)
           │
    ┌──────┼──────┬──────────┬──────────┐
    ▼      ▼      ▼          ▼          ▼
  Health  Skill  Movement  Collision  Damage
  Comp    Comp   Comp      Ability    Dealer
```

**核心设计原则：**
- **单一数据源** — `ARunGameGameState` 持有全部游戏状态，`UPlayerRuntimeState` 持有角色状态
- **事件驱动** — 所有系统通过委托订阅状态变更，无直接跨类命令调用
- **GameplayTag 通信** — 技能效果通过 Tag 发布，各子系统（无敌/碰撞/加速）独立响应

## 模块结构

```
Source/RunGame/
├── Game/              # ARunGameGameState / ARunGameGameMode
├── Character/         # ARunGameCharacter + 运动/碰撞/动画组件
├── Player/            # PlayerController / PlayerState
├── HUD/               # 纯 C++ UMG 控件的多状态 UI + Debug 面板
├── Skill/             # 技能组件 + 可扩展执行对象 + 数据资产配置
├── Actor/
│   ├── Floor/         # FloorBase — 地板池基类
│   ├── Trap/          # ATrap — 可破坏陷阱（IImpactReceiver）
│   ├── Component/     # Health / DamageDealer / CoinSpawner
│   └── Volume/        # 交互体 + 死亡体
├── Interfaces/        # IDamagable / IImpactReceiver
├── WorldSubsystem/    # Timer / Floor / Coin 子系统
└── Variant_* /        # Combat / Platforming / SideScrolling 变体
```

## 关键系统

### 反应式状态机

| 状态 | 说明 |
|------|------|
| `MainMenu` | 主菜单，销毁旧角色 |
| `CountDown` | 倒计时，相机拉远 |
| `InGame` | 游戏中，生成角色，地板开始滚动 |
| `Pause` | 暂停，冻结所有 Tick + Timer |
| `GameOver` | 结束，停止计时，显示结算 |

### 地板对象池

- 数据资产驱动（`UFloorConfigData`）：直道/弯道/上下坡类型 + 生成权重 + 保底数量
- 异步加载 → 预分配池 → 加权随机出池 → 玩家通过后定时回收
- 回收时广播 `OnFloorDeactivated` 委托，子 Actor（Trap/Coin）自行复位

### 技能系统

- 数据资产配置（`USkillConfigData`）：Tag 标识 / 冷却 / 能量消耗 / 执行类
- 可扩展执行对象（`USkillExecutionBase`）：Unstoppable（加速+无敌）、PlayMontageAndImpulse 等
- 双 Tag 效果发布：Duration 到期撤速度 Tag → 速度插值完成撤无敌 Tag

### 碰撞系统

- `URunGameCollisionAbilityComponent` 监听 GameplayTag 激活胶囊体 Overlap
- IImpactReceiver 接口通知被命中 Actor
- 浮点时间戳冷却制（0.5s 可配），替代 Timer 黑名单
- 死亡状态自动清冷却 + 阻止碰撞

### MCP 自动化桥

通过 `McpAutomationBridge` 插件（22 个工具类，200+ 自动化操作），外部 AI 编程助手可直接操控 Unreal Editor 的蓝图/资产/AI/战斗/特效全流程。

## 编码规范

- **Allman 括号**：花括号独占一行
- **双语言注释**：中文在前，英文在后（`/** 中文 English */`）
- **反射宏紧凑**：`UFUNCTION`/`UPROPERTY` 紧贴声明，中间无空行
- **意图注释**：注释说 Why 不说 What，标准标记词（`// TODO:` / `// FIXME:`）

详见 `CODING_STYLE.md`。

## 构建 & 运行

1. 右键 `RunGame.uproject` → Generate Visual Studio project files
2. 打开 `RunGame.sln`，编译 `RunGameEditor` 目标
3. 或直接在 Unreal Editor 中打开 `.uproject`

## License

MIT
