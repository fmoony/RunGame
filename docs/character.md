# Character State Machine — 角色状态机

> 描述角色语义状态、轻量控制管线、土狼时间、二段跳和组件反应。

`UPlayerRuntimeState` 是角色语义状态唯一拥有者。`TrySetCharacterState()` 校验转移、更新 `CurrentCharacterState` 并广播 `OnCharacterStateChanged(Old, New)`。

`ARunGameCharacter` 只保留 Unreal 必需桥接：输入绑定入口、`CanJumpInternal_Implementation()`、`OnJumped_Implementation()` 和 `Landed()`。输入数据属于 InputComponent，移动规则属于 Locomotion，物理命令属于 Movement，阶段协调属于 Pipeline。

| State | Description | Entry |
|---|---|---|
| `Idle` | 默认地面状态 | 游戏开始、落地、滑铲结束 |
| `CoyoteTime` | 离开边缘后的 0.15 秒跳跃宽限期 | StartedFalling Signal |
| `Airborne` | 主动跳跃、二段跳或土狼时间结束 | Jump 请求或 CoyoteExpired Signal |
| `Sliding` | 蹲伏且地面摩擦为零 | Slide 请求成功 |
| `Turning` | 位于转向区域 | TurnFloor overlap |
| `Dead` | 输入清空、Movement 禁用 | Character Death |

## Transition Matrix

由 `UPlayerRuntimeState::IsCharacterStateTransitionAllowed()` 强制执行：

| FROM \ TO | Idle | CoyoteTime | Airborne | Sliding | Turning | Dead |
|---|---:|---:|---:|---:|---:|---:|
| Idle | - | YES | YES | YES | YES | YES |
| CoyoteTime | YES | - | YES | NO | NO | YES |
| Airborne | YES | NO | - | NO | YES | YES |
| Sliding | YES | NO | YES | - | YES | YES |
| Turning | YES | YES | YES | NO | - | YES |
| Dead | NO | NO | NO | NO | NO | - |

Slide 还要求 CMC 当前不处于 Falling。

## Control Ownership

```text
RunGameInputComponent
  continuous snapshot + pending requests
        ↓
RunGameControlPipelineComponent
  PrePhysics fixed stages + domain routing
        ↓
RunGameLocomotionComponent
  Jump / Slide rules + coyote / air-jump data
        ↓
RunGameMovementComponent
  Move / Look / Jump / StopJumping / Crouch
        ↓
UPlayerRuntimeState
  semantic state validation + final broadcast
```

Pipeline 是协调入口，不拥有输入队列、移动领域数据或技能状态。

## CoyoteTime And Double Jump

- `RunGameMovementComponent::SetMovementMode()` 将 MovementMode 变化广播给 Locomotion。
- 从 Idle/Turning 进入 Falling 时，Locomotion 生成 StartedFalling Signal 并启动 0.15 秒 CoyoteTimer。
- Pipeline 下一 PrePhysics 帧将状态提交为 CoyoteTime。
- CoyoteTimer 到期只生成 CoyoteExpired Signal；Pipeline 再提交 Airborne。
- `bAirJumpAvailable` 由 Locomotion 拥有。
- 地面跳和土狼跳保留一次空中跳；真实 Airborne 起跳通过 Character `OnJumped` 回调消耗资格。
- Landed 只生成 Signal；Pipeline 下一控制帧先提交 Idle，再评估仍在缓冲期内的 Jump/Slide 请求。

## Jump And Slide

Jump：

```text
Jump Request
→ Locomotion CanExecuteJump
→ Movement ExecuteJump
→ RuntimeState Airborne
→ RemoveRequest
```

Slide：

```text
Slide Request
→ Locomotion CanExecuteSlide
→ Movement Crouch + GroundFriction=0
→ RuntimeState Sliding
→ RemoveRequest
```

Slide Montage BlendOut 生成 SlideEnded Signal；Pipeline 下一帧调用 `ExecuteSlideEnd()` 并提交 Idle。外部状态直接中断 Sliding 时，Movement 的最终状态监听会兜底恢复 UnCrouch 和摩擦。

## Skill Gating

Pipeline 只把 Skill Request 交给 `USkillComponent::TryActivateRequestedSkill()`。SkillComponent 自己检查状态、冷却、能量、配置和执行对象，并返回：

- `Applied`：技能已执行，请求移除。
- `Deferred`：当前暂不可执行，请求保留至超时。
- `Rejected`：请求永久无效，请求移除。

## Presentation

AnimInstance、Camera、HUD 和效果组件只订阅 RuntimeState 或领域最终事件。Camera 不再拥有 Gameplay Look 输入；Look 由 Pipeline 路由到 Movement 执行。
