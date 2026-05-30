# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build / Run

- Open `RunGame.uproject` (UE 5.6) in Rider or VS, or right-click → Generate Visual Studio project files.
- Build target: `RunGameEditor` (Editor) or `RunGame` (Game). The editor target is the typical development entry point.
- There is no CI, linting, or test suite. Build through the Unreal Editor or via Unreal Build Tool.
- `AFunctionalTest` tests (e.g. `Test_FloorRecycling`) can be run via the Editor's **Functional Test** panel or by calling `StartTest()` directly.

## Architecture

### Central Types (`RunGameType.h`)

- **`ERunGameGameState`**: `MainMenu, CountDown, InGame, Pause, GameOver, MAX` — drives the entire reactive state machine.
- **`ERunGameCharacterState`**: `Idle, Airborne, Sliding, Turning, Dead, MAX` — character core state machine. Mutually exclusive locomotion/life states. Dead is terminal.
- **`FFloorType`**: `StraightFloor, TurnFloor, UpAndDownFloor, MAX` — floor segment categories.

### Reactive State Machine (central event bus)

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

### Key Delegates (who declares what)

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

### Player Lifecycle

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

### Character State Machine

Mirrors the `ARunGameGameState` reactive pattern: `Guard → Validate → Save → Modify → Broadcast`.

**`UPlayerRuntimeState`** owns the state via `SetCharacterState(NewState)`. The method follows `Guard → Validate → Save → Modify → Broadcast` — it validates the transition, updates `CurrentCharacterState`, and broadcasts `OnCharacterStateChanged(Old, New)`. **`ARunGameCharacter`** delegates to `RS->SetCharacterState()` and binds `OnCharacterStateChangedCallback` for reactive side effects (slide crouch/friction, turn flags, etc.).

| State | Description | Entry |
|-------|-------------|-------|
| `Idle` | Default grounded: walking, running, standing | Start of game; landed from Airborne; slide/turn ended |
| `Airborne` | Jumping, falling, walking off ledge | `SetMovementMode` override detects `MOVE_Falling` from Idle |
| `Sliding` | Crouched, no ground friction, montage playing | `StartSlide()` calls `SetCharacterState(Sliding)` |
| `Turning` | Inside turn zone — lateral movement blocked, rotation locked | `TurnFloor` overlap calls `SetCharacterState(Turning)` |
| `Dead` | All input/movement blocked, dissolving | `Die()` calls `SetCharacterState(Dead)` — terminal |

**Transition matrix** (enforced by `IsCharacterStateTransitionAllowed`):

| FROM \ TO | Idle | Airborne | Sliding | Turning | Dead |
|-----------|------|----------|---------|---------|------|
| **Idle**  | -    | YES      | YES¹    | YES     | YES  |
| **Airborne** | YES | -      | NO²     | YES     | YES  |
| **Sliding** | YES | YES      | -       | YES     | YES  |
| **Turning** | YES | YES      | NO      | -       | YES  |
| **Dead**  | NO   | NO       | NO      | NO      | -    |

¹ Sliding additionally requires `!IsFalling()` at runtime.
² Sliding input while Airborne is buffered by `URunGameInputBufferComponent`; auto-consumed when state returns to Idle.

**Movement reaction** (`URunGameMovementComponent`): Self-binds to `OnCharacterStateChanged`. Sliding → Crouch + `GroundFriction=0`. Turning → set turn flags. Dead → `DisableMovement`. Airborne detection via `SetMovementMode` override (not Tick polling).

**Animation reaction** (`URunGameAnimInstance`): Self-binds to `OnCharacterStateChanged` and `OnCharacterDied`. Plays slide/death montages. Death blend-out → `PRS->NotifyDeathAnimationFinished` → Character dissolve.

**Input buffering** (`URunGameInputBufferComponent`): `PendingInputState` removed. Character bridges EnhancedInput → `BufferInput()`. Queue stores intent with 0.3s timeout. State change back to `Idle` auto-consumes oldest buffered entry. Deduplicates same-type commands. Dead → clears buffer.

**Skill gating**: `ActivateSkillByTag()` reads `UPlayerRuntimeState::GetCharacterState()` and blocks activation while `Dead` or `Sliding`. All other states (`Idle`, `Airborne`, `Turning`) allow skill input through to `SkillComponent::TryActivateSkill`.

**Invincibility**: `UHealthComponent::SetInvincible()` follows the same Guard→Modify→Broadcast pattern, broadcasting `OnInvincibilityChanged(bool)` when the value changes. Invincibility is orthogonal — it can coexist with any character state.

**Cleanup**: `BeginPlay` binds `OnCharacterStateChanged.AddDynamic`; `EndPlay` calls `RemoveDynamic`.

### World Subsystems

- **`URunGameFloorSubsystem`** — Procedural floor generation via object pooling. `ARunGameGameMode` holds the configuration (`StraightFloorClasses`, `TurnFloorClasses`, `PreAllocateFloorCount`) and calls `InitializeFloorSystem()` at `BeginPlay`. Async-loads floor classes from `TSoftClassPtr` arrays, then `SpawnInitialFloors()` lays out initial straight + random floors tracking `NextSpawnTransform`. `RequestNextFloor()` (80% straight / 20% turn weighted random) and `RequestFloorAt()` serve new floors; `RecycleDistantFloors()` hides distant floors and returns them to the pool. `ReturnFloor()` and `HideAllActiveFloors()` also recycle floors. Pool is per-type: `TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap`. Active floors tracked in `TArray<TObjectPtr<AFloorBase>> ActiveFloors`. `OnFloorSystemReady` delegate signals async load completion; `GameMode` binds to it for initial floor spawn.
- **`URunGameTimerSubsystem`** — Implements `FTickableGameObject`. **Purely a timer — knows nothing about scoring.** Countdown uses `FTimerManager` at 1Hz; the forward game timer increments `TotalTimeSeconds` every `Tick`. All start/stop is driven reactively by `OnGameStateChanged`. Exposes delegates: `OnCountdownComplete`, `OnTimeChanged` (broadcasts every tick while running).
- **`URunGameCoinSubsystem`** — Coin object pooling via LIFO stack (`TArray::Pop`/`Add`). `PreAllocateCoins(CoinClass, Count)` spawns hidden coins into per-class sub-pools (`TMap<TSubclassOf<ACoin>, TArray<ACoin*>>`). `AcquireCoin()` pops from pool or spawns new; `ReturnCoin()` pushes back via `RemoveSwap` from `ActiveCoins` then `Add` to pool. Coins stay hidden in-place (no off-screen relocation). Delegates: `OnCoinSubsystemReady`.
- **`UPlayerRuntimeState`** — Character state machine + animation event bus. Holds `CurrentCharacterState` and enforces the transition matrix via `IsCharacterStateTransitionAllowed`. Delegates: `OnCharacterStateChanged`, `OnCharacterDied`, `OnHitReaction`, `OnDeathAnimationFinished`. `ResetForNewGame()` clears state to Idle.

### Coin System

**`ACollectibleBase`** (`Actor/Collectible/`):
- Abstract base for collectible items. Components: `UStaticMeshComponent` (visual, root) + `USphereComponent` (trigger, `ECC_Pawn` overlap only).
- Tick-driven animation: `AddActorLocalRotation` self-rotation + sinusoidal float oscillation (`FloatAmplitude`, `FloatFrequency`).
- Pool lifecycle: `ActivateCollectible(FTransform)` sets location only (preserves blueprint scale/rotation), unhides, enables collision + tick. `DeactivateCollectible()` hides and flags `bIsCollected = true`.
- `OnCollected(ARunGameCharacter*)` — BlueprintNativeEvent. Base impl: VFX (`SpawnEmitterAtLocation`), SFX (`PlaySoundAtLocation`), broadcast `OnCollectedDelegate`, hide + disable.
- Delegate: `FOnCollectibleCollectedSignature(ACollectibleBase*)`.

**`ACoin`** (`Actor/Collectible/`):
- Inherits `ACollectibleBase`. Overrides `OnCollected_Implementation` → calls `ARunGamePlayerState::AddScore(CoinValue)`. `CoinValue` (int64, EditDefaultsOnly).

**`UCoinSpawnerComponent`** (`Actor/Component/`, attached to every `FloorBase`):
- `CreateDefaultSubobject` in `FloorBase` constructor. `BeginPlay` caches owner's `USplineComponent` → `CachedSpline`.
- `ApplyConfig(FCoinSpawnConfig)` — copies config from data asset. `SpawnCoins()` checks `bEnableCoinSpawn && CoinClass`, acquires coins from `URunGameCoinSubsystem`, places at computed transforms. `DespawnCoins()` deactivates and returns all managed coins.
- **Multi-row**: `RowCount` parallel lanes offset perpendicular to path direction by `(RowIndex - CenterOffset) * RowSpacing`. `RowSpawnChance` (0-1) controls per-row independent probability.
- **Placement**: Spline-first — if owner has `CoinPathSpline` with points, distribute `CoinCount` coins evenly by distance, offset by `GetRightVectorAtDistanceAlongSpline` per row. Enum fallback — `StraightLine`/`DiagonalLine` (linear interpolation along `LineDirection`) or `Parabolic` (quadratic Bezier with `ParabolaHeight` peak).

**`FCoinSpawnConfig`** (in `CoinSpawnerComponent.h`):
- `CoinClass`, `CoinCount`, `RowCount`, `RowSpacing`, `RowSpawnChance`, `SpawnPattern`, `StartOffset`, `LineDirection`, `EndOffset`, `ParabolaHeight`, `CoinSpacing`.

**`UFloorConfigData::GlobalCoinConfig`** — Single shared `FCoinSpawnConfig` applied to all floor classes. If `CoinClass` is null, no coins spawn.

**`FFloorClassEntry::bEnableCoinSpawn`** — Per-floor-class toggle in data asset. Checked in `RequestFloorAt` before calling `SpawnCoins()`.

**Integration flow:**
```
RequestFloorAt → FindLoadedEntry(InClass) → bEnableCoinSpawn?
  → ApplyConfig(FloorConfig->GlobalCoinConfig) → SpawnCoins()
  → CoinSubsystem::AcquireCoin → ActivateCollectible(Transform)
ReturnFloor → DespawnCoins() → DeactivateCollectible → CoinSubsystem::ReturnCoin
```

### Scoring (cubic polynomial, int64-safe)

`ARunGamePlayerState` runs a 0.1s `FTimerHandle` callback (`CalculateScoreProcess`). On each tick it reads elapsed time from `TimerSubsystem` and computes:

```
Multiplier = 1 + minutes² + 0.5·minutes³
ScoreToAdd = floor(10 × Multiplier)
```

All score variables are `int64`. The timer is paused/unpaused reactively by `OnGameStateChanged`. `Tick` is disabled (`bCanEverTick = false`). Public API: `SetScoringActive(bool)`, `AddScore(int64)`, `SetRunGameScore(int64)`, `GetRunGameScore()`. Delegate: `OnScoreChanged(int64 NewScore)`.

### Input

`ARunGamePlayerController` manages `EnhancedInput` mapping contexts. Input mode and view target are self-managed reactively via `OnGameStateChangedCallback` (the big switch on state). On `GameOver`, it sets `InputModeUIOnly` — no separate death delegate binding needed (reactive pattern handles it). `MainMenuCameraTag` (`FName`, editor-configurable) identifies the main-menu camera placed in the level via tag-based actor search.

### Input Buffering (`RunGameInputBufferComponent`)

`URunGameInputBufferComponent` (`Character/RunGameInputBufferComponent.h/.cpp`) — ActorComponent on Character.

Character binds EnhancedInput → `BufferInput(ERunGameInputCommand)`. Component listens to `UPlayerRuntimeState::OnCharacterStateChanged`, auto-consumes buffered commands when state returns to `Idle`. FIFO queue with 0.3s timeout. Same-type deduplication.

| Command | Buffer when | Consume when |
|---------|-----------|--------------|
| `Slide` | Airborne | Landed → state back to Idle |
| `Jump` | Sliding | Slide ends → state back to Idle |

Move goes directly to MovementComponent, never buffered. `OnInputCommandConsumed` delegate fires → Character executes `Jump()` or `SetCharacterState(Sliding)`.

### Animation (`URunGameAnimInstance`)

`URunGameAnimInstance` (`Character/Animation/RunGameAnimInstance.h/.cpp`) — native C++ AnimInstance, replaces deleted `URunGameAnimationComponent`.

**Data cache** (per-frame `NativeUpdateAnimation`): Pulls `Speed`/`GroundSpeed`/`bIsInAir`/`bIsMoving` from `CharacterMovementComponent`. Pulls `CharacterState`/`bIsSliding`/`bIsTurning`/`bIsDead` from cached `UPlayerRuntimeState*` (cached in `NativeInitializeAnimation`, avoids per-frame `GetSubsystem`). Computes `MoveDirectionAngle` (velocity vs actor forward).

**Hit reaction trigger**: Binds `PRS->OnHitReaction` → sets `bPlayHitReaction + HitReactionDamageType` — ABP reads these.

**ABP (Blueprint side)**: Reads all UPROPERTY variables to drive state machine transitions. `BlendPosesByEnum(CharacterState)` for locomotion/death/slide states. `bPlayHitReaction` for transient hit animation. Death animation end → `AnimNotify_RunGameEvent` → `PRS->NotifyDeathAnimationFinished` → Character dissolve.

### HUD / UI

`ARunGameHUD` owns `CurrentUIMap` mapping each game state to a widget class. Reactively switches widgets via `OnGameStateChangedCallback → UpdateUIForState → ShowUI`. Widget classes (in `RunGame/HUD/`): `RunGameMainMenu`, `RunGameCountDown`, `RunGameInGame`, `RunGamePause`, `RunGameGameOver`.

### Cleanup Pattern

All classes that bind to `GameState::OnGameStateChanged` unbind in `EndPlay` using `RemoveDynamic`. Subsystems unbind in `Deinitialize`. Timer handles (`FTimerHandle`) are cleared on teardown. Montage blend-out callbacks (`OnMontageBlendingOut`) unbind themselves inside the callback.

### Variant Directories

Three gameplay-template variants under `Source/RunGame/`:

- **`Variant_Combat/`** — Melee combat character (`ACombatCharacter`) with combo attacks, charged attacks, damage/health, ragdoll death, and auto-respawn. Implements `ICombatAttacker` and `ICombatDamageable` interfaces.
- **`Variant_Platforming/`** — Platformer movement model (dash, air-dash).
- **`Variant_SideScrolling/`** — 2.5D side-scroller with custom `SideScrollingCameraManager`.

Each variant provides its own Character, GameMode, and PlayerController subclass.

### Health / Damage System

Does NOT use UE's built-in `TakeDamage`/`ApplyDamage`/`UDamageType`. Custom system via `IDamagable` interface + `UHealthComponent` + `UDamageDealerComponent`.

**Data flow:**
```
DamageDealerComponent::OnOverlap
  → OtherActor->Implements<UDamagable>()?
    → IDamagable::Execute_OnTakeDamage(Actor, Damage, FGameplayTag DamageType, Causer)
      → Actor::OnTakeDamage_Implementation → HealthComponent->ApplyDamage(...)
        → CurrentHP -= Damage → broadcast OnHealthChanged / OnDeath
```

**`IDamagable`** (Blueprintable interface, `RunGame/Interfaces/`):
- `OnTakeDamage(float, FGameplayTag, AActor*)`, `OnTakeHealing(float, AActor*)`, `OnDeath(AActor*)` — all `BlueprintNativeEvent`
- `GetCurrentHP()`, `GetMaxHP()`, `IsDead()` — query functions, `BlueprintNativeEvent`

**`UHealthComponent`** (ActorComponent, `RunGame/Actor/Component/`):
- Properties: `MaxHP` (EditAnywhere, ClampMin=1), `CurrentHP`, `bIsDead`, `bIsInvincible`
- Public API: `ApplyDamage(Damage, FGameplayTag DamageType, Causer)`, `Heal(Amount, Healer)`, `Revive(RestoreHP)`, `SetInvincible(bool)`, `IsInvincible()`
- Delegates: `FOnHealthChanged(CurrentHP, MaxHP, Delta)`, `FOnDamageTaken(Damage, DamageType, DamageCauser)`, `FOnDeath(FGameplayTag DamageType, AActor* DeathCauser)`, `FOnInvincibilityChanged(bool bNewInvincible)`
- Tick disabled (`bCanEverTick = false`)
- `bIsInvincible` is orthogonal to the character state machine — it can coexist with any `ERunGameCharacterState`. `SetInvincible()` follows Guard→Modify→Broadcast, broadcasting `OnInvincibilityChanged` when the value changes.
- Does NOT inherit from `IDamagable` — the owning Actor implements the interface and forwards to this component

**`UDamageDealerComponent`** (UBoxComponent, `RunGame/Actor/Component/`):
- Trigger overlap → checks `OtherActor->Implements<UDamagable>()` → calls `IDamagable::Execute_OnTakeDamage`
- Properties: `DamageAmount` (float), `DamageType` (FGameplayTag)

**`ARunGameCharacter`** integration:
- Implements `IDamagable`, owns `UHealthComponent*` via `CreateDefaultSubobject`
- Character status tracked by `UPlayerRuntimeState::CurrentCharacterState` — authoritative source (see Character State Machine above)
- `BeginPlay` binds `HealthComponent->OnDeath` → `OnHealthDepleted` → `Die(DamageType)`, and `OnCharacterStateChanged` → `OnCharacterStateChangedCallback` for reactive slide/turn/death behavior
- All `IDamagable` `_Implementation` functions forward to `HealthComponent`; `IsDead_Implementation()` delegates to `HealthComponent::IsDead()` which checks `CurrentHP <= 0.0f` (single source of truth — no separate `bIsDead` flag)
- `Die(FGameplayTag DamageType, float DestroyDelay = 3.0f)` — spawns death camera, then calls `SetCharacterState(Dead)` (state machine guards against double-entry), then broadcasts `OnCharacterDied`. Notably does **NOT** call `SetGameState(GameOver)` — GameMode listens to `OnCharacterDied` and decides game-over (proper IoC)
- `OnCharacterDied` (BlueprintAssignable) — broadcast inside `Die()`, also on `UPlayerRuntimeState`
- Death montages configured on `URunGameAnimInstance::DeathMontages` (moved from Character)
- `PRS->OnDeathAnimationFinished` — dispatch point for dissolve, bound in Character `BeginPlay`

### Skill System

Three-layer architecture — input binds to Character, Character guards by state, SkillComponent validates and dispatches to transient execution objects.

**Files**: `Skill/RunGameSkillConfigData.h`, `Skill/RunGameSkillExecution.h/.cpp`, `Actor/Component/SkillComponent.h/.cpp`, `HUD/RunGameSkillSlot.h/.cpp`

**Data flow:**
```
InputAction (Started)
  → ARunGameCharacter::ActivateSkillByTag(SkillTag)     [state guard: blocks Dead/Sliding]
    → USkillComponent::TryActivateSkill(SkillTag)       [cooldown + energy + CanExecute checks]
      → USkillExecutionBase::Execute(Owner, SkillTag)    [actual gameplay effect]
      → OnSkillActivated.Broadcast()                     [UI via SkillComponent]
```

#### Core Data Structures

**`FSkillDefinition`** (in `RunGameSkillConfigData.h`) — per-skill configuration:

| Field | Type | Default | Purpose |
|-------|------|---------|---------|
| `SkillTag` | `FGameplayTag` | — | Unique identifier for activation and delegate routing |
| `SkillName` | `FText` | — | Display name in tooltips or UI |
| `SkillIcon` | `TSoftObjectPtr<UTexture2D>` | — | Icon texture — async-loaded by skill slot widget |
| `KeyHint` | `FText` | — | Keyboard hint text (e.g. "Q", "E") |
| `CooldownSeconds` | `float` | `5.0f` | Cooldown duration in seconds. `0` = no cooldown |
| `EnergyCost` | `float` | `100.0f` | Energy consumed on activation |
| `ExecutionClass` | `TSubclassOf<USkillExecutionBase>` | — | Gameplay effect class. Instantiated per-activation, transient |
| `InputAction` | `TObjectPtr<UInputAction>` | — | Bound dynamically in `SetupPlayerInputComponent` |

**`USkillConfigData`** (`UDataAsset`) — holds `TArray<FSkillDefinition> Skills`. Lookup via `FindSkillByTag(FGameplayTag)` returns `const FSkillDefinition*`. Assigned to `USkillComponent::SkillConfig` in editor.

**`FSkillRuntimeState`** (global `USTRUCT` in `SkillComponent.h`) — per-skill runtime state:
- `bool bOnCooldown` — whether the skill is currently cooling down
- `FTimerHandle CooldownTimer` — timer handle managing cooldown expiry
- `TObjectPtr<USkillExecutionBase> ExecutionObject` — cached execution object, created once in `InitializeFromConfig`, reused on activation

Stored in `UPROPERTY() TMap<FGameplayTag, FSkillRuntimeState> SkillStates` — GC-tracked.

#### Skill Execution Hierarchy

Execution objects are cached per-skill (not transient). Created once in `InitializeFromConfig` with `NewObject<USkillExecutionBase>(this, ExecutionClass)`, reused on every activation. `SkillComponent::TryActivateSkill` calls `Reset()` before every `Execute()` — subclasses override `Reset_Implementation()` to clear per-activation state (e.g. `CachedSkillTag`, `RevertTimer`). Base class contract:

| Method | Purpose | Default |
|--------|---------|---------|
| `CanExecute(AActor*, FGameplayTag)` | Additional constraints beyond cooldown/energy | `true` |
| `Execute(AActor*, FGameplayTag)` | Apply gameplay effect | no-op |
| `Cancel(AActor*)` | Cancel active effect (called on death) | no-op |
| `Reset()` | Clear per-activation state before next Execute | no-op |

**Concrete classes:**

| Class | Effect |
|-------|--------|
| `USkillExecution_PlayMontageAndImpulse` | Plays `Montage` on instigator, applies forward `ImpulseStrength` via `AddImpulse` |
| `USkillExecution_Unstoppable` | Speed ×`SpeedMultiplier` (default 1.5×) + invincibility for `Duration` seconds. `Reset_Implementation()` clears `CachedSkillTag` and `RevertTimer` before each activation; `Execute_Implementation()` guards against stale timer handles; `Cancel()` clears timer and reverts effects immediately |

#### SkillComponent — `USkillComponent` (ActorComponent on Character)

**Skill registration** (`InitializeFromConfig`): iterates `SkillConfig->Skills`, creates `FSkillRuntimeState` entry per valid `SkillTag`.

**Activation pipeline** (`TryActivateSkill`, 9-step validation):
1. Config loaded?
2. Cooldown check — reject if `bOnCooldown`
3. Definition lookup — `SkillConfig->FindSkillByTag()`
4. Energy check — reject if `CurrentEnergy < EnergyCost`
5. `CanExecute` on execution object — reject if returns false
6. Apply cooldown — set `bOnCooldown`, arm `FTimerHandle` if duration > 0
7. Deduct energy — `CurrentEnergy -= SkillDef->EnergyCost`
8. Track and execute — save `ActiveExecution`, call `Execute(Owner, SkillTag)`
9. Broadcast — `OnSkillActivated.Broadcast(Tag, Duration)`

**Cooldown expiry** (`OnCooldownExpired`): sets `bOnCooldown = false`, calls `BroadcastSkillReady`.

**Death cleanup** (`ClearAllCooldowns`, triggered by `OnRS_CharacterStateChanged(Dead)`):
1. Cancels `ActiveExecution` (clears Unstoppable's `RevertTimer` + immediately reverts effects)
2. Iterates all `SkillStates`, clears cooldown timers, broadcasts `SkillReady` for each
3. Resets energy to `0.0`

**EndPlay**: clears all cooldown timers + energy regen timer, removes RS delegate bindings.

#### Energy System

Energy is stored in `USkillComponent::CurrentEnergy` (clamped `[0, MaxEnergy]`). `SkillComponent` owns both configuration and the regen timer.

**Configuration** (on `USkillComponent`, EditAnywhere):

| Property | Default | Meaning |
|----------|---------|---------|
| `MaxEnergy` | `100.0` | Maximum energy capacity |
| `InitialEnergy` | `0.0` | Energy at game start |
| `BaseEnergyRegenPerSecond` | `0.333f` | Base regen rate (~5 min from 0 to full) |
| `ScoreRegenMultiplier` | `0.005f` | Score-to-energy multiplier (sqrt-scaled) |

**Regen formula** (tick every 0.1s):
```
ScoreBonus = sqrt(max(0, Score)) × ScoreRegenMultiplier
RegenAmount = (BaseEnergyRegenPerSecond + ScoreBonus + EnergyRegenModifier)
              × EnergyRegenMultiplier × 0.1
```

Energy regen starts in `BeginPlay` and runs continuously via `FTimerHandle EnergyRegenTimer` at 0.1s interval. The energy bar reads `sqrt(Score)` so explosive score growth yields diminishing returns.

**Runtime modifiers** (from scene props): `AddEnergyRegenModifier(float Delta)` adds flat bonus; `SetEnergyRegenMultiplier(float Mult)` sets global multiplier (1.0 = normal).

#### Input Binding (dynamic)

`ARunGameCharacter::SetupPlayerInputComponent` iterates `SkillConfig->Skills`. For each `FSkillDefinition` with valid `InputAction` and `SkillTag`:
```cpp
EnhancedInputComponent->BindAction(
    SkillDef.InputAction,
    ETriggerEvent::Started,
    this, &ARunGameCharacter::ActivateSkillByTag,
    SkillDef.SkillTag   // payload — routed by tag
);
```
All skills share a single C++ handler; the `FGameplayTag` payload determines which skill fires.

#### Character Integration

**`ActivateSkillByTag(FGameplayTag)`** — state guard before SkillComponent:
- Reads `PlayerRuntimeState::GetCharacterState()`
- Rejects if `Dead` or `Sliding`; passes if `Idle`, `Airborne`, `Turning`
- Delegates to `SkillComponent->TryActivateSkill(SkillTag)`

**Speed modifier bridge** — `AddSpeedModifier(Tag, Mult)` / `RemoveSpeedModifier(Tag)` are on `ARunGameCharacter`. Maintains a `TMap<FGameplayTag, float>` and a cached composite multiplier. `URunGameMovementComponent` reads the composite via `GetCompositeSpeedMultiplier()`. Character `Tick` reads this:
```
FinalMaxSpeed = MaxSpeedCurve × CompositeSpeedMultiplier
```

#### RuntimeState Integration

**`UCombatRuntimeState`** holds skill/energy data: `CurrentEnergy`, `MaxEnergy`, and delegates `OnSkillActivated`, `OnSkillReady`, `OnSkillExecuted`, `OnEnergyChanged`. Also hosts `BroadcastSkillActivated()` / `BroadcastSkillReady()` / `BroadcastSkillExecuted()` pure-broadcast methods called by SkillComponent.

**`UPlayerRuntimeState`** holds speed modifiers (`TMap<FGameplayTag, float>` + `CachedCompositeSpeedMultiplier`) and `CurrentCharacterState` (used for skill gating). `AddSpeedModifier`/`RemoveSpeedModifier` maintain the composite product; `ClearSpeedModifiers` empties the map and resets to `1.0f` (called on death).

#### Skill HUD

**`URunGameSkillSlot`** (per-slot widget, created dynamically by `URunGameInGame`):
- Bound widgets: `SkillIcon` (`UImage`), `CooldownOverlay` (`UImage`), `KeyHintText` (`UTextBlock`)
- `SetupSlot(FSkillDefinition, FGameplayTag, USkillComponent*)` — async-loads icon, creates `MaterialInstanceDynamic` for radial cooldown fill, binds to `OnSkillActivated`/`OnSkillReady`/`OnEnergyChanged`
- Cooldown overlay driven by MID scalar parameter (`CooldownPercentParamName`, default `"CooldownPercent"`). `1.0` = full cooldown, `0.0` = ready

### Key Dependencies

From `RunGame.Build.cs` — Public: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `AIModule`, `StateTreeModule`, `GameplayStateTreeModule`, `UMG`, `FunctionalTesting`, `GameplayTags`. Private: `Slate`, `SlateCore`.

## Coding Conventions

See `CODING_STYLE.md` for detailed formatting rules. Key highlights:

- **Allman brace style** — braces on own line; `FORCEINLINE` single-line getters excepted
- **UE reflection macros** — comment above macro, macro directly above declaration, no blank line between
- **Function comments** — `/** ... */` for public, `// ...` for protected/private; inline getters exempt
- **In-function comments** — explain WHY not WHAT; `// ` with space; blank line before comment blocks; tags: `// TODO:`, `// FIXME:`, `// HACK:`, `// NOTE:`
- **Include paths** — relative to module root (e.g. `#include "Actor/Component/HealthComponent.h"`); forward-declare in headers
- **`FGameplayTag`** — include `GameplayTagContainer.h`
- **Logging** — `UE_LOG(LogRunGame, Warning/Error, TEXT("ClassName: message"), *Args)`. Category declared in `RunGame.h`.

## Scope

- Only modify `ARunGameCharacter` and non-variant classes
- Never touch `Variant_Combat/`, `Variant_Platforming/`, `Variant_SideScrolling/`
- 2选择方案A，给出#3的解决方案，#4再探讨一下，更新#1#2的错误描述
- 2选择方案A，给出#3的解决方案，#4再探讨一下，更新#1#2的错误描述
