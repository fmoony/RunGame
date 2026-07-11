# Input, Animation & UI — 输入、动画与界面

> 描述当前 Enhanced Input 快照、请求队列、控制管线、动画实例和 HUD 系统。

## Input

`ARunGamePlayerController` 管理 Enhanced Input Mapping Context 和 UI/Game 输入模式。`ARunGameCharacter::SetupPlayerInputComponent()` 是 Unreal 必需入口，只调用 `URunGameInputComponent::BindInput()`。

`URunGameInputComponent` 拥有输入资产绑定和输入生命周期：

- Move、Look、JumpHeld 作为连续值保存在组件中。
- JumpRelease 使用递增 generation 表达释放边沿，不进入请求队列。
- Jump、Slide、Skill 作为 `FRunGameInputRequest` 进入 FIFO。
- 队列容量默认 16，满时拒绝最新请求。
- 请求默认在 0.3 秒后超时。
- `GetInputSnapshot()` 返回不可变副本，读取不会消费请求。

`URunGameInputContextComponent` 当前不持有输入数据，仅作为未来归一化和设备映射层的占位。

## Control Pipeline

`URunGameControlPipelineComponent` 使用 `TG_PrePhysics` Tick，并作为 `URunGameMovementComponent` 的 Tick prerequisite。每个控制帧顺序固定：

1. Dead 时清空输入和 Locomotion Signal。
2. 处理上一物理帧产生的 Landed、StartedFalling、CoyoteExpired、SlideEnded Signal。
3. 清理超时请求并读取 `FRunGameInputSnapshot`。
4. 将 Move、Look、JumpRelease 路由给 MovementComponent。
5. 分别扫描 Locomotion 和 Skill 请求域。

Locomotion 与 Skill 各自维持域内 FIFO；某一域 Deferred 不阻塞另一域。

| Input | Path | Removal |
|---|---|---|
| Move / Look | Snapshot → Pipeline → Movement | 连续值，无请求 |
| JumpRelease | generation → Pipeline → StopJumping | 无请求 |
| Jump / Slide | Request → Locomotion Evaluate → Movement Execute → RuntimeState | Applied/Rejected/Expired 时移除 |
| Skill | Request → SkillComponent | Applied/Rejected/Expired 时移除，Deferred 保留 |

## Animation

`URunGameAnimInstance` 是原生 C++ AnimInstance。每帧从 CharacterMovementComponent 读取速度和空中状态，从 `UPlayerRuntimeState` 读取最终语义状态。

- Sliding 最终提交后播放 Slide Montage。
- Montage BlendOut 只向 Locomotion 生成 `SlideEnded` Signal。
- Pipeline 下一控制帧恢复 Crouch/摩擦并提交 Idle。
- HitReaction 和 Death 继续通过 RuntimeState 事件驱动。

动画层只读取已经提交的状态，不根据按键推测动作成功。

## Camera

`URunGameCameraComponent` 只负责 FOV、SpringArm、主菜单/跟随视点、死亡镜头和插值。Gameplay Look 已迁到 `URunGameMovementComponent::ExecuteLookInput()`，由控制管线路由。

## HUD / UI

`ARunGameHUD` 通过 `CurrentUIMap` 响应 GameState 并切换对应 Widget。HUD 和技能槽继续订阅 RuntimeState、SkillComponent 等领域最终事件，不消费输入请求。
