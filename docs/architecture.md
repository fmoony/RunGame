# Architecture — 反应式状态机架构

> 从 `CLAUDE.md` 提取，完整描述 RunGame 项目的核心架构：类型系统、事件总线、玩家生命周期。

## Central Types (`RunGameType.h`)

- **`ERunGameGameState`**: `MainMenu, CountDown, InGame, Pause, GameOver, MAX` — drives the entire reactive state machine.
- **`ERunGameCharacterState`**: `Idle, Airborne, Sliding, Turning, Dead, MAX` — character core state machine. Mutually exclusive locomotion/life states. Dead is terminal.
- **`FFloorType`**: `StraightFloor, TurnFloor, UpAndDownFloor, MAX` — floor segment categories.

## Reactive State Machine (central event bus)

Game states flow: `MainMenu → CountDown → InGame → GameOver` (with `Pause` as an interstitial).

**`ARunGameGameState`** is the single source of truth. All configurable defaults (`DefaultCountdownSeconds`, `DefaultGameTotalTime`) and mutable state (`CurrentState`, `CountdownSeconds`) live here. `SetGameState()` broadcasts `OnGameStateChanged(OldState, NewState)`, which is the central event bus every system listens to. `SetCountdownSeconds()` broadcasts `OnCountdownUpdated(CountdownSeconds)` when the value changes.

**No class directly commands another.** Each class binds to `GameState::OnGameStateChanged` in `BeginPlay` and reactively manages only its own domain:

| Class | What it does when state changes |
|---|---|
| `URunGameTimerSubsystem` | `CountDown` → starts countdown; `InGame` → starts forward timer (0→up); `Pause`/`GameOver`/`MainMenu` → stops timer |
| `ARunGamePlayerController` | `MainMenu`/`CountDown` → UI input mode + main menu camera; `InGame` → game-only input; `Pause`/`GameOver` → UI input mode |
| `ARunGamePlayerState` | `CountDown`/`MainMenu` → clears score to 0; `InGame` → unpauses 0.1s score timer; all others → pauses score timer |
| `ARunGameHUD` | Switches widget via `CurrentUIMap` (`TMap<ERunGameGameState, TSubclassOf<UUserWidget>>`) |
| `ARunGameCharacter` | `MainMenu` → self-destroys |
| `ARunGameGameMode` | `InGame` → spawns player via `SpawnPlayer()`; orchestrates floor system init/reset |

## Key Delegates (who declares what)

| Delegate | Owner | Signature |
|---|---|---|
| `OnGameStateChanged` | `ARunGameGameState` | `(ERunGameGameState Old, ERunGameGameState New)` |
| `OnCountdownUpdated` | `ARunGameGameState` | `(int32 CountdownSeconds)` |
| `OnCountdownComplete` | `URunGameTimerSubsystem` | `()` — no params |
| `OnTimeChanged` | `URunGameTimerSubsystem` | `(float NewTime)` |
| `OnScoreChanged` | `ARunGamePlayerState` | `(int64 NewScore)` |
| `OnFloorSystemReady` | `URunGameFloorSubsystem` | `()` — no params |
| `OnInteractionBegin` | `ARunGameInteractiveVolume` | `(ARunGameCharacter* PlayerCharacter)` |
| `OnInteractionEnd` | `ARunGameInteractiveVolume` | `(ARunGameCharacter* PlayerCharacter)` |
| `OnHealthChanged` | `UHealthComponent` | `(float CurrentHP, float MaxHP, float Delta)` — positive=heal, negative=damage |
| `OnDeath` | `UHealthComponent` | `(FGameplayTag DamageType, AActor* DeathCauser)` |
| `OnDamageTaken` | `UHealthComponent` | `(float Damage, FGameplayTag DamageType, AActor* DamageCauser)` |
| `OnCharacterDied` | `ARunGameCharacter` / `UPlayerRuntimeState` | `(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter)` — both declare this delegate |
| `OnCharacterStateChanged` | `UPlayerRuntimeState` | `(ERunGameCharacterState Old, ERunGameCharacterState New)` |
| `OnHitReaction` | `UPlayerRuntimeState` | `(float Damage, FGameplayTag DamageType)` — Character bridges HealthComponent→OnDamageTaken |
| `OnDeathAnimationFinished` | `UPlayerRuntimeState` | `()` — AnimNotify or AnimInstance montage callback triggers dissolve |
| `OnInvincibilityChanged` | `UHealthComponent` | `(bool bNewInvincible)` |
| `OnSkillActivated` | `USkillComponent` | `(FGameplayTag SkillTag, float CooldownDuration)` |
| `OnSkillReady` | `USkillComponent` | `(FGameplayTag SkillTag)` |
| `OnSkillExecuted` | `USkillComponent` | `(FGameplayTag SkillTag)` |
| `OnEnergyChanged` | `USkillComponent` | `(float CurrentEnergy, float MaxEnergy)` |

## Player Lifecycle

1. **Countdown**: `GameMode::StartGameCountDown()` sets state to `CountDown`. `TimerSubsystem` reacts, reads `GameState->DefaultCountdownSeconds`, starts a 1Hz `FTimerHandle` that decrements `GameState->CountdownSeconds` (broadcasting `OnCountdownUpdated` each tick).
2. **Spawn**: When countdown reaches 0, `TimerSubsystem` sets state to `InGame`, broadcasts `OnCountdownComplete`. `GameMode::SpawnPlayer()` spawns `GameCharacterClass`, calls `Possess`, blends view back to character, sets `bAutoManageActiveCameraTarget = true`.
3. **Death flow** — fully owned by `ARunGameCharacter::Die(FGameplayTag DamageType, float DestroyDelay = 3.0f)`:
   - **Two entry paths converge** on `Die()`:
     - (A) `ARunGameDeathVolume` clears invincibility, then applies 99999 damage via `IDamagable::Execute_OnTakeDamage` → `HealthComponent::ApplyDamage` → `OnDeath` → `OnHealthDepleted` → `Die()`
     - (B) Other damage sources (`UDamageDealerComponent`) → same `ApplyDamage` → `OnDeath` → `Die()` chain
   - **Die() executes in this order:**
     1. Spawn death `ACameraActor` at follow-camera location, blend view **(before animation to avoid camera jitter)**
     2. `SetCharacterState(Dead)` — state machine guards against double-entry, all components react synchronously
     3. `RS->OnCharacterDied.Broadcast(DamageType, this)` — GameMode subscribes and reactively calls `GS->SetGameState(GameOver)` (IoC: Pawn never touches global state)
     4. `OnCharacterDied.Broadcast(DamageType, this)` — for Blueprint / other listeners
     5. Death montage plays via `URunGameAnimInstance` (which also binds `PRS->OnCharacterDied`); dissolve → `Destroy()` on completion
   - **GameMode is completely decoupled** from death handling — no `HandlePlayerDeath`, no `OnPlayerDeath`
4. **Respawn**: `GameMode::StartNewGame()` → same countdown path → `SpawnPlayer()` re-possesses a new pawn and blends the view back.

## Cleanup Pattern

All classes that bind to `GameState::OnGameStateChanged` unbind in `EndPlay` using `RemoveDynamic`. Subsystems unbind in `Deinitialize`. Timer handles (`FTimerHandle`) are cleared on teardown. Montage blend-out callbacks (`OnMontageBlendingOut`) unbind themselves inside the callback.

## Variant Directories

Three gameplay-template variants under `Source/RunGame/`:

- **`Variant_Combat/`** — Melee combat character (`ACombatCharacter`) with combo attacks, charged attacks, damage/health, ragdoll death, and auto-respawn. Implements `ICombatAttacker` and `ICombatDamageable` interfaces.
- **`Variant_Platforming/`** — Platformer movement model (dash, air-dash).
- **`Variant_SideScrolling/`** — 2.5D side-scroller with custom `SideScrollingCameraManager`.

Each variant provides its own Character, GameMode, and PlayerController subclass.

## Key Dependencies

From `RunGame.Build.cs` — Public: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `AIModule`, `StateTreeModule`, `GameplayStateTreeModule`, `UMG`, `FunctionalTesting`, `GameplayTags`. Private: `Slate`, `SlateCore`.
