# Character State Machine — 角色状态机

> 从 `CLAUDE.md` 提取，完整描述角色状态机、过渡矩阵、土狼时间、二段跳及组件反应。

Mirrors the `ARunGameGameState` reactive pattern: `Guard → Validate → Save → Modify → Broadcast`.

**`UPlayerRuntimeState`** owns the state via `SetCharacterState(NewState)`. The method follows `Guard → Validate → Save → Modify → Broadcast` — it validates the transition, updates `CurrentCharacterState`, and broadcasts `OnCharacterStateChanged(Old, New)`. **`ARunGameCharacter`** delegates to `RS->SetCharacterState()` and binds `OnCharacterStateChangedCallback` for reactive side effects (slide crouch/friction, turn flags, etc.).

| State | Description | Entry |
|-------|-------------|-------|
| `Idle` | Default grounded: walking, running, standing | Start of game; landed from Airborne/CoyoteTime; slide/turn ended |
| `CoyoteTime` | Grace period after walking off ledge (0.15s) — can still jump | `SetMovementMode` detects `MOVE_Falling` from Idle/Turning |
| `Airborne` | Jumping, falling — supports double jump (`bAirJumpAvailable`) | CoyoteTime expiry; ground jump; coyote-time jump consumed |
| `Sliding` | Crouched, no ground friction, montage playing | `StartSlide()` calls `SetCharacterState(Sliding)` |
| `Turning` | Inside turn zone — lateral movement blocked, rotation locked | `TurnFloor` overlap calls `SetCharacterState(Turning)` |
| `Dead` | All input/movement blocked, dissolving | `Die()` calls `SetCharacterState(Dead)` — terminal |

## Transition Matrix

Enforced by `IsCharacterStateTransitionAllowed`:

| FROM \ TO | Idle | CoyoteTime | Airborne | Sliding | Turning | Dead |
|-----------|------|------------|----------|---------|---------|------|
| **Idle**  | -    | YES        | YES      | YES¹    | YES     | YES  |
| **CoyoteTime** | YES | -     | YES      | NO      | NO      | YES  |
| **Airborne** | YES | NO      | -        | NO²     | YES     | YES  |
| **Sliding** | YES | NO      | YES      | -       | YES     | YES  |
| **Turning** | YES | YES     | YES      | NO      | -       | YES  |
| **Dead**  | NO   | NO      | NO       | NO      | NO      | -    |

¹ Sliding additionally requires `!IsFalling()` at runtime.
² Sliding input while Airborne/CoyoteTime is buffered by `URunGameInputBufferComponent`; auto-consumed when state returns to Idle.

## CoyoteTime + Double Jump

- `URunGameMovementComponent::SetMovementMode` detects `MOVE_Falling` from Idle/Turning → sets `CoyoteTime` (not Airborne) + arms 0.15s `CoyoteTimer`. On expiry → auto-transition to `Airborne`.
- `ARunGameCharacter::bAirJumpAvailable` granted on: entering CoyoteTime, ground jump (Idle→Airborne), landing (→Idle). Consumed on: jump during CoyoteTime or Airborne (either via buffered input or direct `DoJumpStart`).
- `CanJumpInternal_Implementation` overridden: allows jump during CoyoteTime or Airborne with `bAirJumpAvailable`.
- `Landed()` handles both Airborne and CoyoteTime → Idle transitions.
- `RunGameInputBufferComponent`: Jump during CoyoteTime → immediate execute (not buffered). Slide during CoyoteTime → buffered (like Airborne).

## Movement Reaction

`URunGameMovementComponent`: Self-binds to `OnCharacterStateChanged`. Sliding → Crouch + `GroundFriction=0`. Turning → set turn flags. Dead → `DisableMovement`. Airborne/CoyoteTime detection via `SetMovementMode` override (not Tick polling). Landing clears `CoyoteTimer`.

## Animation Reaction

`URunGameAnimInstance`: Self-binds to `OnCharacterStateChanged` and `OnCharacterDied`. Plays slide/death montages. Death blend-out → `PRS->NotifyDeathAnimationFinished` → Character dissolve.

## Input Buffering

`URunGameInputBufferComponent`: `PendingInputState` removed. Character bridges EnhancedInput → `BufferInput()`. Queue stores intent with 0.3s timeout. State change back to `Idle` auto-consumes oldest buffered entry. Deduplicates same-type commands. Dead → clears buffer.

## Skill Gating

`ActivateSkillByTag()` reads `UPlayerRuntimeState::GetCharacterState()` and blocks activation while `Dead` or `Sliding`. All other states (`Idle`, `Airborne`, `Turning`) allow skill input through to `SkillComponent::TryActivateSkill`.

## Invincibility

`UHealthComponent::SetInvincible()` follows the same Guard→Modify→Broadcast pattern, broadcasting `OnInvincibilityChanged(bool)` when the value changes. Invincibility is orthogonal — it can coexist with any character state.

## Cleanup

`BeginPlay` binds `OnCharacterStateChanged.AddDynamic`; `EndPlay` calls `RemoveDynamic`.
