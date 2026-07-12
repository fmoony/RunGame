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
- **单一数据源** — `ARunGameGameState` 持有游戏状态，`UPlayerRuntimeState` 持有角色语义状态
- **输入生命周期自治** — `URunGameInputComponent` 保存连续快照与 Jump/Slide/Skill 请求，统一负责超时和移除
- **固定控制阶段** — `URunGameControlPipelineComponent` 在 PrePhysics 中先处理物理信号，再路由连续输入和领域请求
- **跑酷与镜头解耦** — `DesireRotation` 表示赛道前进方向，`ControllerRotation` 只表示玩家观察方向
- **状态请求有返回语义** — `TrySetCharacterState()` 明确返回状态切换是否被状态机接受
- **事件驱动响应** — RuntimeState 状态变更后广播，各运动/动画/镜头/特效组件独立响应
- **GameplayTag 通信** — 技能效果通过 Tag 发布，各子系统独立响应

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

### 角色输入与状态链路

角色层只保留 Unreal 必须的输入与生命周期回调。输入、规则、物理执行和语义状态分别归属不同组件：

```
EnhancedInput
    ↓
URunGameInputComponent
    ├─ Move / Look / JumpHeld 快照
    └─ Jump / Slide / Skill 请求队列（容量 16，超时 0.3s）
    ↓
URunGameControlPipelineComponent（PrePhysics）
    ├─ 先处理 Landed / Coyote / SlideEnded Signal
    ├─ Move / JumpRelease → MovementComponent
    ├─ Look → CameraComponent
    └─ 分别扫描 Locomotion / Skill 请求域
    ↓
URunGameLocomotionComponent
    ├─ Jump / Slide 允许性判断
    ├─ 土狼时间 / 二段跳资源
    └─ 简单移动 Signal
    ↓
URunGameMovementComponent
    ├─ 基于 DesireRotation 自动前进和横向移动
    └─ Jump / StopJumping / Crouch 等 CMC 命令
URunGameCameraComponent
    └─ Look / 视角限制 / TurnBox 镜头跟随
    ↓
UPlayerRuntimeState::TrySetCharacterState()
    ↓
OnCharacterStateChanged → Anim / Camera / Effect 自行响应
```

`InputComponent` 管输入生命周期，`Pipeline` 管固定阶段，`LocomotionComponent` 管规则和移动领域数据，`MovementComponent` 管跑酷方向与 CMC 命令，`CameraComponent` 管观察方向和镜头表现，`RuntimeState` 管角色语义状态真相源。

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
- **自由观察**：Look 只改变 `ControllerRotation`，不会改变 `DesireRotation` 跑酷方向
- **视角限制**：水平观察限制为相对当前赛道方向左右各 90°，避免持续回看已回收地板
- **TurnBox 跟随**：角色和速度立即转向，镜头参考方向以 180°/s 匀速旋转，并保留玩家当前观察偏角
- **可选回正**：默认不自动回正；开启后仅在无 Look 输入且 TurnBox 跟随结束时缓慢回正

镜头控制参数位于 CameraComponent 的 `Camera|Control` 分类：`MaxYawOffset`、`TurnFollowYawSpeed`、`bEnableYawRecentering` 和 `YawRecenteringSpeed`。

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

详见 `docs/_coding-style.md`。

## 构建 & 运行

1. 右键 `RunGame.uproject` → Generate Visual Studio project files
2. 打开 `RunGame.sln`，编译 `RunGameEditor` 目标
3. 或直接在 Unreal Editor 中打开 `.uproject`

## License

本项目代码采用 MIT 许可证。

本项目基于 [Unreal® Engine](https://www.unrealengine.com/) 开发，须遵守 [Unreal Engine EULA](https://www.unrealengine.com/eula) 条款。部分游戏资产（角色、动画、材质）基于 Unreal Engine 官方示例资产修改，其版权归 Epic Games, Inc. 所有。

`Source/RunGame/Variant_*/` 及 `Content/Variant_*/` 目录下的变体代码与资产来源于 Unreal Engine 官方模板，尚未完全重构，版权归 Epic Games, Inc. 所有。
