# RunGame

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.6-orange)](https://www.unrealengine.com/)

基于 UE5.6 的 3D 跑酷游戏，C++ 原生开发。采用反应式状态机架构，集成 MCP 自动化桥接与外部 AI 编程助手深度协作。

## 架构概览

```
┌─────────────────────────────────────────────────────────┐
│                       GameState                         │
│               (Single Source of Truth)                  │
│     MainMenu → CountDown → InGame ⇄ Pause → GameOver   │
└────────────────────┬────────────────────────────────────┘
                     │ OnGameStateChanged 委托广播
       ┌──────┬──────┼──────┬──────────┬──────────┐
       ▼      ▼      ▼      ▼          ▼          ▼
     Timer  HUD    Player  Player    GameMode   Floor
     Subsys        Ctlr    State               Subsystem
                              │
                       SpawnPlayer()  ← CountDown 时就定位
                              │
               ┌──────────────▼──────────────────────┐
               │         ARunGameCharacter           │
               │  Health / Skill / Move / Camera     │
               │  Collision / Effect / AnimInstance  │
               │  死亡→子组件自愈, 重生→复用不销毁    │
               └─────────────────────────────────────┘
```

**核心设计原则：**
- **单一数据源** — `ARunGameGameState` 持有全部游戏状态，`UPlayerRuntimeState` 持有角色状态
- **事件驱动** — 所有系统通过委托订阅状态变更，无直接跨类命令调用
- **GameplayTag 通信** — 技能效果通过 Tag 发布，各子系统独立响应
- **自主复原** — Dead→Idle 时各组件自愈（HP/能量/移动/碰撞/材质/动画），GameMode 只管定位和显示

## 模块结构

```
Source/RunGame/
├── Game/              # ARunGameGameState / ARunGameGameMode
├── Character/         # ARunGameCharacter + 运动/碰撞/动画/效果/镜头/Camera 组件
├── Player/            # PlayerController / PlayerState
├── HUD/               # 纯 C++ UMG 多状态 UI + Debug 面板
├── Skill/             # 技能组件 + 可扩展执行对象 + 数据资产配置
├── Actor/
│   ├── Floor/         # FloorBase — 地板池基类（直道/弯道/上下坡）
│   ├── Trap/          # ATrap — 可破坏陷阱（IImpactReceiver）
│   ├── Collectible/   # Coin — 可回收拾取物
│   ├── Component/     # Health / DamageDealer / CoinSpawner
│   └── Volume/        # 交互体 + 死亡体
├── Interfaces/        # IDamagable / IImpactReceiver
└── WorldSubsystem/    # Timer / Floor / Coin / PlayerRuntimeState
```

## 关键系统

### 反应式状态机

| 状态 | 说明 |
|------|------|
| `MainMenu` | 主菜单，角色隐藏 |
| `CountDown` | 倒计时，SpawnPlayer 定位角色，禁运动；镜头挂回+主菜单视点 |
| `InGame` | 开跑：角色显示，运动恢复，镜头 Blend 到跟随 |
| `Pause` | 暂停，冻结所有 Tick + Timer |
| `GameOver` | 结束，停止计时，显示结算 |

### 角色生命周期

角色预生成于 `OnFloorSystemReady`（MainMenu 时已存在），死亡后隐藏不销毁。重启时 `SpawnPlayer` 定位+复位，`PRS::ResetForNewGame` 广播 Dead→Idle 触发全系统自愈：

| 组件 | Dead→Idle 响应 |
|------|---------------|
| HealthComponent | `Revive(MaxHP)` |
| SkillComponent | `CurrentEnergy = InitialEnergy` |
| MovementComponent | `MOVE_Walking` + `DesireRotation` 对齐 |
| CollisionAbilityComponent | 胶囊体 `QueryAndPhysics` |
| EffectComponent | 清定时器 + 还原原始材质 |
| AnimInstance | `Montage_Stop` + 解冻骨骼 |

### 镜头系统

`URunGameCameraComponent` 挂载于 Character，全程存活：
- **MainMenu/CountDown**：切换至场景标记的 `ACameraActor`
- **Death**：SpringArm `DetachFromComponent` 留在死亡点
- **Restart**：CountDown 时挂回+切主菜单视点，InGame 丝滑 Blend 无跳变
- **FOV**：速度驱动平滑 FOV；状态驱动 SpringArm 长度

### 技能系统

- 数据资产配置（`USkillConfigData`）：Tag 标识 / 冷却 / 能量消耗 / 执行类 / 效果 Tag
- 可扩展执行对象：`Unstoppable`（加速+无敌）、`Shield`（一次性护盾）、`Dash`（冲刺）
- 双 Tag 效果：Duration 到期撤速度 Tag → 速度插值完成撤无敌 Tag

### 可破坏陷阱

`ATrap` 实现 `IImpactReceiver`，挂载 `UHealthComponent` + `UDamageDealerComponent`：
- 角色碰撞技能激活时被摧毁，Tag Query 过滤可破坏条件
- `DamageDealer` 绑定宿主 Mesh 重叠事件，碰撞形状=视觉形状
- 随 Floor 池子回收/重置

### 碰撞系统

- `URunGameCollisionAbilityComponent` 监听 GameplayTag 激活胶囊体 Overlap
- `IImpactReceiver` 接口通知被命中 Actor
- 浮点时间戳冷却制，死亡状态清冷却+阻止碰撞

### MCP 自动化桥

通过 `McpAutomationBridge` 插件，外部 AI 编程助手可直接操控 Unreal Editor 全流程。

## 编码规范

- **Allman 括号**：花括号独占一行
- **双语言注释**：中文在前，英文在后
- **反射宏紧凑**：`UFUNCTION`/`UPROPERTY` 紧贴声明
- **意图注释**：注释说 Why 不说 What

详见 `CODING_STYLE.md`。

## 构建 & 运行

1. 右键 `RunGame.uproject` → Generate Visual Studio project files
2. 打开 `RunGame.sln`，编译 `RunGameEditor` 目标
3. 或直接在 Unreal Editor 中打开 `.uproject`

## License

本项目代码采用 MIT 许可证。

本项目基于 [Unreal® Engine](https://www.unrealengine.com/) 开发，须遵守 [Unreal Engine EULA](https://www.unrealengine.com/eula) 条款。部分游戏资产（角色、动画、材质）基于 Unreal Engine 官方示例资产修改，其版权归 Epic Games, Inc. 所有。

`Source/RunGame/Variant_*/` 及 `Content/Variant_*/` 目录下的变体代码与资产来源于 Unreal Engine 官方模板，尚未完全重构，版权归 Epic Games, Inc. 所有。
