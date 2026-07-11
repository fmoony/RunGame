# RunGame 角色控制管线分阶段重构

## Summary

在不新增多余 ActorComponent、不改变现有玩法效果的前提下，将当前“InputContext 读取即消费 + Input 直接唤醒 Pipeline + Locomotion 直接写状态”的链路迁移为：

```text
InputSnapshot -> ControlIntent / InputRequest -> ControlPlan -> CMC Execution -> ControlSignal -> Commit
```

只新增必要的数据类型和一个共享控制类型头文件，继续使用现有 Input、Pipeline、Locomotion、Movement、RuntimeState、Skill、Camera。每迁移一个功能就删除该功能的旧路径，确保阶段性可编译、可运行、无双重执行。

## Current Interfaces

| 当前接口 | 处理方式 | 目标 |
|---|---|---|
| `Character::SetupPlayerInputComponent()` | 保留 | 继续只转发给 `InputComponent::BindInput()` |
| `Character::CanJumpInternal_Implementation()` | 保留 | 继续桥接 Locomotion 跳跃规则 |
| `Character::OnJumped_Implementation()`、`Landed()` | 迁移 | 只生成 Locomotion Signal，不修改领域数据或语义状态 |
| `Character::SetCharacterState()` | 分阶段废弃 | TurnFloor 等调用迁走后停止作为通用写状态入口；Death 改走权威中断接口 |
| `InputComponent::BindInput()` | 保留 | 输入资产仍由 InputComponent 管理 |
| `BufferInput()`、`ClearBuffer()` | 废弃 | 替换为请求入队、超时、InFlight 和 Resolve API |
| Input 对 Pipeline/Skill/InputContext 的直接调用 | 废弃 | Input 不再判断、路由或执行 Gameplay |
| `InputContext::ConsumeFrame()` 等可变接口 | 废弃但暂不删除组件 | InputContext 本轮退出数据链，保留为空的未来预处理位置 |
| `Pipeline::ProcessInputFrame()`、状态变化后 NextTick 重试 | 废弃 | 替换为有 Tick 前置约束的固定控制帧 |
| Locomotion 的 `HandleInputContextCommand()`、`TryConsumeInputContextBuffer()` | 废弃 | 替换为快照、评估、Plan、Signal、Commit 回调 |
| Locomotion 的 `RequestCharacterState()` | 废弃 | Locomotion 不再直接写 RuntimeState |
| Movement 的速度、方向、MovementMode 观察 | 保留 | 增加批准后命令执行和物理证据上报 |
| Skill 的 `TryActivateSkill()` | 保留 | Skill 继续拥有 Tag、能量、冷却、实例和执行 |
| `TryActivateRequestedSkill(FGameplayTag)` | 替换 | 改为按 SkillSlot/InputTag 评估，返回 Applied/Deferred/Rejected |
| Camera 的状态表现、FOV、SpringArm、死亡镜头 | 保留 | `HandleLookInput()` 迁至控制侧 Movement 入口 |
| 旧 `RunGameInputBufferComponent` | 无需迁移 | 类已不存在，其遗留语义位于 InputContext 和 Locomotion |

## Core Types

扩展 `RunGameInputTypes.h`，另建一个 `RunGameControlTypes.h`，不增加新的运行时组件类。

### FRunGameInputRequest

- `uint64 RequestId`，0 为无效值。
- `uint64 SequenceId`，InputComponent 内严格递增。
- `double CreatedAt`，使用 World Game Time，暂停期间不流逝。
- `ERunGameInputRequestType`：Jump、Slide、Skill。
- `ERunGameInputRequestState`：Pending、InFlight、Applied、Rejected、Expired、Cancelled。
- 可选 `int32 SkillSlot` 和 `FGameplayTag InputTag`；Input 不保存最终 SkillTag。
- InFlight 时保存关联 `PlanId`。

### FRunGameInputSnapshot

- MoveAxis、LookAxis、JumpHeld。
- JumpReleaseGeneration 等连续输入边沿序号，避免读取时清标记。
- Request 队列的只读值副本。
- `GetInputSnapshot()` 为纯读取，不超时、不确认、不重排请求。

### FRunGameControlIntent

- Move、Look、JumpHeld 和尚未处理的 JumpReleased 边沿。
- 不需要 RequestId、PlanId 或执行确认。

### FRunGameControlPlan

- `PlanId`、可选 `SourceRequestId`、动作类型、预期物理结果。
- `CreatedControlEpoch`、`CreatedRevision`、创建时间、确认截止时间。
- Turn、SlideEnd 等非输入计划使用无效 RequestId。
- Plan 只描述批准的命令和预期证据，不修改任何状态。

### FRunGameControlSignal

- `SignalSequence`、`PhysicsFrameId`、时间戳、Signal 类型和可选 `RelatedPlanId`。
- 承载 JumpLaunched、MovementModeChanged、Landed、SlideActivated、SlideDeactivated、UnCrouchBlocked、CoyoteExpired、TurnZone、MontageBlendOut 等事实。

### FRunGameCharacterStateSnapshot

- 当前语义状态、Revision、ControlEpoch。

### FRunGameLocomotionSnapshot

- MovementMode、落地、速度、蹲伏和 Slide 物理状态。
- 二段跳资格、Coyote 状态、当前 ActivePlan。
- 按值生成，供一个控制阶段稳定读取。

## Input Lifecycle

`RunGameInputComponent` 成为输入数据唯一来源：

- 默认队列容量 `16`，Pending 超时 `0.3s`，InFlight 物理确认超时 `0.5s`。
- 队列满时拒绝最新请求，不驱逐任何已有请求；日志包含请求类型、SequenceId、队列长度和最老请求状态。
- `ExpirePendingRequests(CurrentTime)` 只处理 Pending；InFlight 不使用输入缓冲超时。
- `TryMarkRequestInFlight(RequestId, PlanId)` 原子校验 Pending、RequestId 和 PlanId 后更新。
- `ResolveRequest(RequestId, TerminalState, Reason)` 只接受 Applied、Rejected、Expired、Cancelled。
- Deferred 不调用任何生命周期接口，仍为 Pending。
- `CancelAllRequests(Reason)` 在权威中断时取消 Pending 和 InFlight。
- 即时输入事件改为只读观测事件，发送原始输入样本或快照，不暴露 Resolve/执行入口。
- Skill 绑定阶段使用配置数组索引作为 SkillSlot；Input 不验证或捕获最终 SkillTag，SkillComponent 自己解析 Slot。
- Move/Look 的 Completed/Canceled 回调写回零值，避免快照重复应用旧轴值。
- Jump Started 设置 JumpHeld 并入队 Jump Request；Jump Completed 清除 JumpHeld 并递增 JumpReleaseGeneration，不进入 FIFO。

`InputContextComponent` 保留在 Character 上，但移除数据所有权和 Pipeline 依赖。旧方法在迁移期间标记废弃，最终不再被生产代码调用。

## Fixed Control Frame

`RunGameControlPipelineComponent` 启用 `TG_PrePhysics` Tick，并在 BeginPlay 中执行：

```cpp
MovementComponent->AddTickPrerequisiteComponent(ControlPipelineComponent);
```

运行时用 `ensure` 验证 Pipeline Tick 早于 CharacterMovement Tick，且不存在循环前置依赖。

每个控制帧严格执行：

1. 检测 Dead、Respawn、LostControl 等权威中断边沿。
2. 中断时由 RuntimeState 提升 ControlEpoch，取消 ActivePlan 和全部 Request，清空 Signal 双缓冲，本帧停止普通提交。
3. 调用 Input 清理 Pending 超时，再获取不可变 InputSnapshot。
4. 从 Locomotion 双缓冲 Drain 当前 Signal 批次；处理过程中产生的新 Signal 进入下一缓冲。
5. 按 PhysicsFrameId 归并 MovementModeChanged、Landed、OnJumped 等同一物理更新事实。
6. 用 Signal 解析当前 InFlight Plan，包括正常确认、失败和超时 Reconcile。
7. 每次成功提交后重新获取 CharacterStateSnapshot 和 LocomotionSnapshot。
8. Signal 阶段结束后再次获取最终快照，再生成 Move/Look/JumpRelease Intent。
9. 按冲突域扫描离散 Request。
10. 启动 Plan 或立即领域事务后结束对应域扫描。
11. 本帧结束时只广播完整的 PostCommit/领域已提交事件。

Pipeline 不实现 Jump、Slide 或 Skill 合法性，只调用领域评估入口。

## Conflict Domains

保持一个 Input FIFO，但按 Locomotion 与 Skill 两个冲突域扫描：

- 每域最多扫描 `16` 个队列元素。
- 每域只考虑本域最早的非终态请求，保持域内 FIFO。
- 一个域的 Deferred 不阻塞另一个域。
- 可以连续清理本域永久 Rejected 或 Expired 的队首。
- 遇到 Deferred、成功启动 InFlight Plan、启动立即动作后，停止当前域扫描。
- 每个域最多一个 ActivePlan；Locomotion ActivePlan 由 LocomotionComponent 拥有，Pipeline 只查询和协调。
- Skill 当前是领域内立即事务，不由 Pipeline 持有技能执行生命周期。

## Locomotion And CMC

`RunGameLocomotionComponent` 负责：

- 生成稳定 LocomotionSnapshot。
- 双缓冲收集 Signal。
- 维护 PlanId、ActivePlan、二段跳、Coyote、Slide 等领域运行时数据。
- 根据最新状态快照评估 Jump、Slide、Turn，生成 ControlPlan 或 Deferred/Rejected。
- 生成提交所需的 Locomotion 领域 Delta。
- 不读取或修改 Input Request，不直接写 PlayerRuntimeState。

`RunGameMovementComponent` 负责执行批准后的命令：

- Move：执行 `AddMovementInput()`，无 Plan、无确认。
- Look：更新 Controller Rotation；CameraComponent 不再处理 Gameplay Look/Aim。
- JumpRelease：立即调用 `StopJumping()`，使用 generation 保证每次释放只执行一次，不受 Jump ActivePlan 阻塞。
- Jump：在 ActivePlan 已建立后调用 Character Jump 路径。
- Slide：执行 Crouch、胶囊/摩擦和 Slide 物理运行时设置。
- Turn：执行现有 DesireRotation 和 Velocity 旋转逻辑。
- 不写语义状态。

Jump 成功主要由 `OnJumped`/DoJump 成功产生的 `JumpLaunched Signal` 确认，并携带当前 RelatedPlanId。MovementModeChanged 只证明当前 Airborne，不单独证明是该 Jump Plan 导致，从边缘坠落不得确认 Jump。

Slide 成功必须同时具备：

- ActivePlan 匹配。
- Crouch 或胶囊缩放执行成功。
- Slide 物理标记和摩擦参数已经生效。
- 当前 MovementMode 允许滑铲。

SlideEnd 先执行 UnCrouch；空间阻挡时产生 `UnCrouchBlocked`，保持 Sliding 并继续 Reconcile，不提前提交 Idle。成功恢复胶囊、摩擦和 Slide 标记后才提交 Idle。

## Signal And Reconcile

Locomotion 持有两个 Signal 数组：

- 回调始终写入 `IncomingSignals`。
- Pipeline Drain 时交换 `IncomingSignals` 与 `ProcessingSignals`。
- 处理过程中产生的 Signal 留给下一控制帧，禁止递归处理。

MovementComponent 每次物理 Tick 递增自己的 PhysicsFrameId；动画和 Timer Signal 使用最近 PhysicsFrameId 加独立 SignalSequence。

InFlight 超时进入 Reconcile，不立即 Rejected：

- 校验 ControlEpoch、PlanId、Request InFlight 状态和 Locomotion ActivePlan。
- 重新读取 MovementMode、OnGround、Velocity、bIsCrouched、Slide 物理标记和执行证据。
- 已真实成功但 Signal 丢失时补做 Applied Commit。
- 真实失败时 Resolve 为 Rejected，Reason 使用 ExecutionFailed。
- 状态仍不确定但未超过最终 Reconcile 限制时保持 InFlight。
- Epoch 不匹配或权威中断时 Cancelled。
- Revision 变化只触发重新校验和日志，不单独作废跨物理帧 Plan。

## RuntimeState And Commit

`PlayerRuntimeState` 继续拥有语义状态、合法转移、Revision 和广播：

- 每次语义状态变化递增 Revision。
- Death、Respawn、LostControl 边沿递增 ControlEpoch。
- 增加只读 `GetCharacterStateSnapshot()`。
- 增加带 `ExpectedRevision`、`ExpectedEpoch` 的协调提交接口。
- 增加权威中断接口，原子提升 Epoch 并提交 Dead/Respawn 状态。
- 保留当前状态事件和状态转移规则。

`CommitControlResult()` 顺序固定：

1. 校验当前不是禁止普通提交的权威中断状态。
2. 校验 ExpectedEpoch、PlanId、Request InFlight 和 ActivePlan。
3. 对即时提交校验 ExpectedRevision；跨物理帧结果在最新快照上重新验证可接受性。
4. 让 RuntimeState 完成最后一次无副作用转移校验。
5. 先应用 Locomotion 领域 Delta，使领域数据与即将广播的语义状态一致。
6. 提交 PlayerRuntimeState，递增 Revision 并广播。
7. 将 Request Resolve 为 Applied、Rejected、Expired 或 Cancelled。
8. 清除 ActivePlan。
9. 广播 PostCommit/领域提交完成事件。

Locomotion Delta 应可恢复；如果 RuntimeState 在最终提交时意外失败，立即回滚 Delta、保留或取消 Request，并输出 Error 日志，禁止留下半提交状态。

## Skill And Presentation

`SkillComponent` 新增按 SkillSlot/InputTag 的请求入口，返回 Applied、Deferred 或 Rejected：

- SkillComponent 自己解析最终 SkillTag。
- Invalid Slot、缺失配置和永久非法输入返回 Rejected。
- 状态暂不可用、能量不足或冷却中返回 Deferred。
- 实际扣除能量、启动冷却、创建执行结果并广播成功后才返回 Applied。
- Pipeline 只据返回值更新 Request，不接触 SkillStates、能量或 ExecutionObject。

表现层约束：

- AnimInstance 只响应已提交 PlayerRuntimeState 和 Locomotion 事件。
- CameraComponent 仅保留 FOV、SpringArm、镜头震动、死亡镜头、切换和插值。
- HUD、音效和按键提示不得根据输入观测事件推测动作成功。
- TurnFloor 改为向 Locomotion 上报 TurnZone Signal，不再调用 Character 状态 setter。

## Implementation Phases

### Phase 0: Baseline

- 编译当前 `RunGameEditor`。
- 记录 Move、Look、短按/长按 Jump、二段跳、Coyote、Slide、TurnFloor、Skill 和 Death 的 PIE 行为。
- 确认 `.codegraph` 本地状态不纳入源码提交。

### Phase 1: Types And Input Lifecycle

- 添加 Request、Snapshot、Intent、Plan、Signal、状态快照和结果枚举。
- 在 InputComponent 实现容量、Sequence、超时、InFlight、Resolve、Cancel 和日志。
- 暂不切换现有 Gameplay 回调。
- 添加纯逻辑 Automation Test。
- 编译并运行输入生命周期测试。

### Phase 2: Snapshots, Epoch And Signal Infrastructure

- PlayerRuntimeState 增加 Revision、ControlEpoch、快照和版本化提交。
- Locomotion 增加稳定快照、ActivePlan 和 Signal 双缓冲，但旧回调暂维持现有行为。
- Movement 增加 PhysicsFrameId 和执行证据结构。
- 测试 Epoch、Revision、双缓冲和 Observation 归并。
- 编译并完成原玩法冒烟验证。

### Phase 3: Pipeline Tick And Continuous Intent

- 启用 PrePhysics Tick 和 CMC Tick prerequisite。
- 接入外部中断、固定阶段和 PostCommit 日志。
- 迁移 Move、Look、JumpHeld、JumpRelease。
- 对尚未迁移的 Slide/Skill 暂时保留明确命名的 Legacy 路径，禁止新旧路径同时处理同一输入。
- 编译并验证移动、镜头和可变跳高。

### Phase 4: Jump Plan

- Jump Request 接入 Locomotion 域 FIFO。
- 建立 ActivePlan、TryMarkInFlight、CMC Jump、JumpLaunched Signal、MovementMode Observation 和 Reconcile。
- 迁移 OnJumped、Landed、Coyote 和二段跳数据提交。
- 删除 Jump 的旧 InputContext/Locomotion 消费路径。
- 编译并验证普通跳、二段跳、边缘坠落、Coyote、落地后下一控制帧 Jump Buffer、Signal 丢失和死亡中断。

### Phase 5: Slide And Turn Plan

- 迁移 Slide Start、Slide End、Crouch、摩擦、胶囊确认、BlendOut 和 UnCrouchBlocked。
- TurnFloor 改为 Signal，Turn 物理执行改为 Plan。
- 移除 Locomotion 对这些路径的直接 RuntimeState 写入。
- 编译并验证滑铲动画、受阻起身、转向区域和死亡中断。

### Phase 6: Skill Domain

- Skill 输入改为 SkillSlot/InputTag Request。
- SkillComponent 返回三态结果并自行解析 Tag。
- 移除 Input 对最终 SkillTag 和 Skill 执行逻辑的依赖。
- 编译并验证成功、能量不足、冷却、非法 Slot、跨域不阻塞和死亡取消。

### Phase 7: Cleanup

- 删除 `FRunGameBufferedInputCommand`、旧 `FRunGameInputFrame` 和旧消费 API。
- 移除 Input 对 Pipeline、Skill 执行和 InputContext 数据的直接引用。
- 移除 `ProcessInputFrame()`、NextTick 缓冲重试和 Locomotion InputContext API。
- 停止 Camera Gameplay Look 接口和 Character 通用状态转发入口。
- 保留 InputContextComponent 类和 Character 上的组件成员。
- 全量 CodeGraph/源码引用扫描、`git diff --check`、编译和 PIE 回归。

## Tests And Acceptance

自动化测试至少覆盖：

- 快照读取不修改 Request。
- RequestId、SequenceId 单调且唯一。
- 容量 16 时拒绝最新请求。
- Pending 超时和 InFlight 独立超时。
- Deferred 保持 Pending。
- TryMarkInFlight 与 Resolve 的非法状态转换失败。
- Signal 双缓冲不发生重入。
- 同物理帧 Observation 只导致一次 Revision 和广播。
- Locomotion/Skill 域互不阻塞且域内 FIFO。
- Revision 陈旧但 Epoch 有效的 Plan 会重新校验。
- Epoch 变化取消旧 Plan。
- 超时 Reconcile 能补交已成功但丢失 Signal 的动作。

PIE 验收保持现有玩法：

- Move 和 Look 手感不变。
- JumpRelease 及时 StopJumping。
- 普通跳、二段跳、Coyote 和落地缓冲正确。
- 普通坠落不能误确认 Jump Request。
- Slide 进入、动画 BlendOut、受阻起身和恢复摩擦正确。
- TurnFloor 行为不变。
- Skill 能量、冷却、执行和 UI 广播不变。
- Death/Respawn/LostControl 后旧物理结果不能覆盖当前状态。
- 动画、Camera 和 HUD 只反映已提交结果。
