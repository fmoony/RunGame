# Character State Machine — 角色状态机

> 从 `CLAUDE.md` 提取，完整描述角色状态机、过渡矩阵、土狼时间、二段跳及组件反应。

Mirrors the `ARunGameGameState` reactive pattern: `Guard → Validate → Save → Modify → Broadcast`.

**`UPlayerRuntimeState`** owns the state via `TrySetCharacterState(NewState)`. The method follows `Guard → Validate → Save → Modify → Broadcast` — it validates the transition, updates `CurrentCharacterState`, broadcasts `OnCharacterStateChanged(Old, New)`, and returns whether the target state is active. **`ARunGameCharacter`** is a UE callback / Blueprint compatibility facade. Input lifetime lives in **`URunGameInputBufferComponent`**; movement rules and physics execution live in **`URunGameMovementComponent`**.

| State | Description | Entry |
|-------|-------------|-------|
| `Idle` | Default grounded: walking, running, standing | Start of game; landed from Airborne/CoyoteTime; slide/turn ended |
| `CoyoteTime` | Grace period after walking off ledge (0.15s) — can still jump | `SetMovementMode` detects `MOVE_Falling` from Idle/Turning |
| `Airborne` | Jumping, falling — supports double jump (`bAirJumpAvailable`) | CoyoteTime expiry; intentional ground jump; coyote-time jump |
| `Sliding` | Crouched, no ground friction, montage playing | Ready Slide command handled by `URunGameMovementComponent` |
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

- `URunGameMovementComponent::SetMovementMode` detects `MOVE_Falling` from Idle/Turning. Intentional jump launches transition directly to `Airborne`; ledge falls enter `CoyoteTime` and arm a 0.15s `CoyoteTimer`. On expiry → auto-transition to `Airborne`.
- `URunGameMovementComponent::bAirJumpAvailable` represents double-jump availability, not CoyoteTime. It is available from grounded start / landing, stays available through coyote jump, and is consumed only by a real Airborne double jump.
- `ARunGameCharacter::CanJumpInternal_Implementation` forwards UE's jump permission callback to `URunGameMovementComponent::CanStartJump`.
- `ARunGameCharacter::Landed()` forwards UE's landed callback to `URunGameMovementComponent::HandleOwnerLanded`, which handles both Airborne and CoyoteTime → Idle transitions.
- `RunGameInputBufferComponent`: Jump during CoyoteTime → immediate execute (not buffered). Slide during CoyoteTime → buffered (like Airborne).

## Movement Reaction

`URunGameMovementComponent`: Self-binds to `OnCharacterStateChanged`, consumes movement-domain input requests from `URunGameInputBufferComponent`, and handles UE jump / landed callbacks forwarded by Character. Sliding → Crouch + `GroundFriction=0`. Turning → set turn flags. Dead → `DisableMovement`. Airborne/CoyoteTime detection via `SetMovementMode` override (not Tick polling). Landing clears `CoyoteTimer`.

## Animation Reaction

`URunGameAnimInstance`: Self-binds to `OnCharacterStateChanged` and `OnCharacterDied`. Plays slide/death montages. Death blend-out → `PRS->NotifyDeathAnimationFinished` → Character dissolve.

## Input Buffering

`URunGameInputBufferComponent`: `PendingInputState` removed. Character forwards EnhancedInput → `BufferInput()`. InputBuffer decides immediate execution vs queueing, asks `URunGameMovementComponent::TryConsumeInputCommand()` to consume movement-domain commands, and removes the signal only when consumption succeeds. State change back to `Idle` auto-attempts the oldest buffered entry. Deduplicates same-type commands. Dead → clears buffer.

## Skill Gating

`ActivateSkillByTag()` reads `UPlayerRuntimeState::GetCharacterState()` and blocks activation while `Dead` or `Sliding`. All other states (`Idle`, `Airborne`, `Turning`) allow skill input through to `SkillComponent::TryActivateSkill`.

## Invincibility

`UHealthComponent::SetInvincible()` follows the same Guard→Modify→Broadcast pattern, broadcasting `OnInvincibilityChanged(bool)` when the value changes. Invincibility is orthogonal — it can coexist with any character state.

## Cleanup

`BeginPlay` binds `OnCharacterStateChanged.AddDynamic`; `EndPlay` calls `RemoveDynamic`.
