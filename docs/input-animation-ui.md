# Input, Animation & UI — 输入、动画与界面

> 从 `CLAUDE.md` 提取，描述 EnhancedInput、输入缓冲、动画实例和 HUD 系统。

## Input

`ARunGamePlayerController` manages `EnhancedInput` mapping contexts. Input mode and view target are self-managed reactively via `OnGameStateChangedCallback` (the big switch on state). On `GameOver`, it sets `InputModeUIOnly` — no separate death delegate binding needed (reactive pattern handles it). `MainMenuCameraTag` (`FName`, editor-configurable) identifies the main-menu camera placed in the level via tag-based actor search.

## Input Buffering

`URunGameInputBufferComponent` (`Character/RunGameInputBufferComponent.h/.cpp`) — ActorComponent on Character.

Character binds EnhancedInput → broadcasts `OnInputCommandRequested`. `URunGameInputBufferComponent` subscribes, decides immediate execution vs buffering from `UPlayerRuntimeState`, and broadcasts ready commands through `ARunGameCharacter::OnInputCommandReady`. `URunGameMovementComponent` subscribes to ready commands and owns Jump / Slide execution. FIFO queue with 0.3s timeout. Same-type deduplication.

| Command | Buffer when | Execute immediately when |
|---------|-----------|---------------------------|
| `Slide` | Airborne, CoyoteTime | Idle |
| `Jump` | Sliding | Idle, Turning, CoyoteTime, Airborne |

Move goes directly to MovementComponent, never buffered. `Character` only emits input / UE lifecycle events; MovementComponent reacts and marks ready command requests as handled after successful execution.

## Animation

`URunGameAnimInstance` (`Character/Animation/RunGameAnimInstance.h/.cpp`) — native C++ AnimInstance, replaces deleted `URunGameAnimationComponent`.

**Data cache** (per-frame `NativeUpdateAnimation`): Pulls `Speed`/`GroundSpeed`/`bIsInAir`/`bIsMoving` from `CharacterMovementComponent`. Pulls `CharacterState`/`bIsCoyoteTime`/`bIsSliding`/`bIsTurning`/`bIsDead` from cached `UPlayerRuntimeState*` (cached in `NativeInitializeAnimation`, avoids per-frame `GetSubsystem`). Computes `MoveDirectionAngle` (velocity vs actor forward).

**Hit reaction trigger**: Binds `PRS->OnHitReaction` → sets `bPlayHitReaction + HitReactionDamageType` — ABP reads these.

**ABP (Blueprint side)**: Reads all UPROPERTY variables to drive state machine transitions. `BlendPosesByEnum(CharacterState)` for locomotion/death/slide states. `bPlayHitReaction` for transient hit animation. Death animation end → `AnimNotify_RunGameEvent` → `PRS->NotifyDeathAnimationFinished` → Character dissolve.

## HUD / UI

`ARunGameHUD` owns `CurrentUIMap` mapping each game state to a widget class. Reactively switches widgets via `OnGameStateChangedCallback → UpdateUIForState → ShowUI`. Widget classes (in `RunGame/HUD/`): `RunGameMainMenu`, `RunGameCountDown`, `RunGameInGame`, `RunGamePause`, `RunGameGameOver`.
