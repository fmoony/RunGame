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
| `OnCharacterDied` | `ARunGameCharacter` | `(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter)` |
| `OnCharacterStateChanged` | `ARunGameCharacter` | `(ERunGameCharacterState Old, ERunGameCharacterState New)` — pure, no side effects |
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
     1. `SetGameState(GameOver)` — Controller's `OnGameStateChangedCallback` reactively sets `InputModeUIOnly`, HUD switches to GameOver widget
     2. `StopAllMontages` — halts any ongoing animation
     3. Spawn death `ACameraActor` at follow-camera location, blend view **(before animation to avoid camera jitter)**
     4. `OnCharacterDied.Broadcast(DamageType, this)` — for any external listeners
     5. Look up `DeathMontages[DamageType]`, play if found; bind `OnDeathMontageBlendingOut` to `Destroy()` when montage completes
     6. If no montage found → `Destroy()` immediately or via `FTimerHandle` with `DestroyDelay`
   - **GameMode is completely decoupled** from death handling — no `HandlePlayerDeath`, no `OnPlayerDeath`
4. **Respawn**: `GameMode::StartNewGame()` → same countdown path → `SpawnPlayer()` re-possesses a new pawn and blends the view back.

### Character State Machine

Mirrors the `ARunGameGameState` reactive pattern: `Guard → Validate → Save → Modify → Broadcast`.

**`UPlayerRuntimeState`** owns the state via `SetCharacterState(NewState)`. The method follows `Guard → Validate → Save → Modify → Broadcast` — it validates the transition, updates `CurrentCharacterState`, and broadcasts `OnCharacterStateChanged(Old, New)`. **`ARunGameCharacter`** delegates to `RS->SetCharacterState()` and binds `OnCharacterStateChangedCallback` for reactive side effects (slide crouch/friction, turn flags, etc.).

| State | Description | Entry |
|-------|-------------|-------|
| `Idle` | Default grounded: walking, running, standing | Start of game; landed from Airborne; slide/turn ended |
| `Airborne` | Jumping, falling, walking off ledge | `Tick()` detects `IsFalling()` from Idle |
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
² Sliding input while Airborne is buffered in `PendingInputState`; consumed on `Landed()` → Idle → Sliding.

**Reactive callback** (`OnCharacterStateChangedCallback`):
- Entering `Sliding` → Crouch, set `GroundFriction=0`, bind `OnSlideBlendingOut`, play slide montage
- Leaving `Sliding` → UnCrouch, restore `GroundFriction`, reset `AnimRootMotionTranslationScale`, unbind montage callback
- Entering `Turning` → set `bTurn=true`, `InTurnBox=true`
- Leaving `Turning` → set `bTurn=false`, `InTurnBox=false`
- Entering `Dead` → clear `bTurn`, `InTurnBox`

**Input buffering**: `PendingInputState` stores a rejected transition request. On `Landed()` (Airborne→Idle), if `PendingInputState == Sliding`, it's consumed and `SetCharacterState(Sliding)` is retried.

**Skill gating**: `ActivateSkillByTag()` reads `PlayerRuntimeState::GetCharacterState()` and blocks activation while `Dead` or `Sliding`. All other states (`Idle`, `Airborne`, `Turning`) allow skill input through to `SkillComponent::TryActivateSkill`.

**Invincibility**: `UHealthComponent::SetInvincible()` follows the same Guard→Modify→Broadcast pattern, broadcasting `OnInvincibilityChanged(bool)` when the value changes. Invincibility is orthogonal — it can coexist with any character state.

**Cleanup**: `BeginPlay` binds `OnCharacterStateChanged.AddDynamic`; `EndPlay` calls `RemoveDynamic`.

### World Subsystems

- **`URunGameFloorSubsystem`** — Procedural floor generation via object pooling. `ARunGameGameMode` holds the configuration (`StraightFloorClasses`, `TurnFloorClasses`, `PreAllocateFloorCount`) and calls `InitializeFloorSystem()` at `BeginPlay`. Async-loads floor classes from `TSoftClassPtr` arrays, then `SpawnInitialFloors()` lays out initial straight + random floors tracking `NextSpawnTransform`. `RequestNextFloor()` (80% straight / 20% turn weighted random) and `RequestFloorAt()` serve new floors; `RecycleDistantFloors()` hides distant floors and returns them to the pool. `ReturnFloor()` and `HideAllActiveFloors()` also recycle floors. Pool is per-type: `TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap`. Active floors tracked in `TArray<TObjectPtr<AFloorBase>> ActiveFloors`. `OnFloorSystemReady` delegate signals async load completion; `GameMode` binds to it for initial floor spawn.
- **`URunGameTimerSubsystem`** — Implements `FTickableGameObject`. **Purely a timer — knows nothing about scoring.** Countdown uses `FTimerManager` at 1Hz; the forward game timer increments `TotalTimeSeconds` every `Tick`. All start/stop is driven reactively by `OnGameStateChanged`. Exposes delegates: `OnCountdownComplete`, `OnTimeChanged` (broadcasts every tick while running).
- **`URunGameCoinSubsystem`** — Coin object pooling via LIFO stack (`TArray::Pop`/`Add`). `PreAllocateCoins(CoinClass, Count)` spawns hidden coins into per-class sub-pools (`TMap<TSubclassOf<ACoin>, TArray<ACoin*>>`). `AcquireCoin()` pops from pool or spawns new; `ReturnCoin()` pushes back via `RemoveSwap` from `ActiveCoins` then `Add` to pool. Coins stay hidden in-place (no off-screen relocation). Delegates: `OnCoinSubsystemReady`.
- **`UGameFlowRuntimeState`** — Holds game state, countdown, and timer data. Delegates: `OnGameStateChanged`, `OnCountdownUpdated`, `OnCountdownComplete`, `OnTimeChanged`. `ARunGameGameState` and `URunGameTimerSubsystem` forward into it.
- **`UPlayerRuntimeState`** — Holds score, character state, turn flags, speed modifiers, cached character reference. Delegates: `OnScoreChanged`, `OnCharacterStateChanged`, `OnCharacterDied`. `ARunGamePlayerState` and `ARunGameCharacter` forward into it. `SetCharacterState()` enforces the transition matrix. `ResetForNewGame()` called on respawn to clear Dead state and reset score/modifiers.
- **`UCombatRuntimeState`** — Holds HP, invincibility, energy, and skill state. Delegates: `OnHealthChanged`, `OnDamageTaken`, `OnDeath`, `OnInvincibilityChanged`, `OnSkillActivated`, `OnSkillReady`, `OnSkillExecuted`, `OnEnergyChanged`. `UHealthComponent` and `USkillComponent` forward into it. `ApplyDamage()` contains invincibility/death guard logic.

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
- All `IDamagable` `_Implementation` functions forward to `HealthComponent`; `IsDead_Implementation()` checks `CurrentCharacterState == Dead`
- `Die(FGameplayTag DamageType, float DestroyDelay = 3.0f)` — first calls `SetCharacterState(Dead)` (state machine guards against double-entry), then proceeds with death sequence
- `OnCharacterDied` (BlueprintAssignable) — broadcast inside `Die()`, available for external binding if needed
- `DeathMontages` (`TMap<FGameplayTag, UAnimMontage*>`, EditAnywhere) — damage type to death animation mapping
- `OnDeathMontageBlendingOut` — callback when death montage ends, triggers `Destroy()`
- If no matching montage found in `DeathMontages`, immediate or delayed `Destroy()` via `FTimerHandle`

### Skill System

Three-layer architecture — input binds to Character, Character guards by state, SkillComponent validates and dispatches to transient execution objects.

**Files**: `Skill/RunGameSkillConfigData.h`, `Skill/RunGameSkillExecution.h/.cpp`, `Actor/Component/SkillComponent.h/.cpp`, `HUD/RunGameSkillSlot.h/.cpp`

**Data flow:**
```
InputAction (Started)
  → ARunGameCharacter::ActivateSkillByTag(SkillTag)     [state guard: blocks Dead/Sliding]
    → USkillComponent::TryActivateSkill(SkillTag)       [cooldown + energy + CanExecute checks]
      → USkillExecutionBase::Execute(Owner, SkillTag)    [actual gameplay effect]
      → UCombatRuntimeState::BroadcastSkillActivated()   [UI via SkillComponent forwarder]
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

**`FSkillRuntimeState`** (private struct inside `USkillComponent`) — per-skill runtime state:
- `bool bOnCooldown` — whether the skill is currently cooling down
- `FTimerHandle CooldownTimer` — timer handle managing cooldown expiry

Stored in `TMap<FGameplayTag, FSkillRuntimeState> SkillStates`.

#### Skill Execution Hierarchy

All execution classes are transient (`NewObject` on `GetTransientPackage()`) — created on activation, GC'd later. Base class contract:

| Method | Purpose | Default |
|--------|---------|---------|
| `CanExecute(AActor*, FGameplayTag)` | Additional constraints beyond cooldown/energy | `true` |
| `Execute(AActor*, FGameplayTag)` | Apply gameplay effect | no-op |
| `Cancel(AActor*)` | Cancel active effect (called on death) | no-op |

**Concrete classes:**

| Class | Effect |
|-------|--------|
| `USkillExecution_PlayMontageAndImpulse` | Plays `Montage` on instigator, applies forward `ImpulseStrength` via `AddImpulse` |
| `USkillExecution_Unstoppable` | Speed ×`SpeedMultiplier` (default 1.5×) + invincibility for `Duration` seconds. Arms `RevertTimer`; `Cancel()` clears timer and reverts effects immediately |

#### SkillComponent — `USkillComponent` (ActorComponent on Character)

**Skill registration** (`InitializeFromConfig`): iterates `SkillConfig->Skills`, creates `FSkillRuntimeState` entry per valid `SkillTag`.

**Activation pipeline** (`TryActivateSkill`, 9-step validation):
1. Config loaded?
2. Cooldown check — reject if `bOnCooldown`
3. Definition lookup — `SkillConfig->FindSkillByTag()`
4. Energy check — reject if `CurrentEnergy < EnergyCost`
5. `CanExecute` on execution object — reject if returns false
6. Apply cooldown — set `bOnCooldown`, arm `FTimerHandle` if duration > 0
7. Deduct energy — `CombatRuntimeState::SetEnergy(Current - Cost)`
8. Track and execute — save `ActiveExecution`, call `Execute(Owner, SkillTag)`
9. Broadcast — `CombatRuntimeState::BroadcastSkillActivated(Tag, Duration)`

**Cooldown expiry** (`OnCooldownExpired`): sets `bOnCooldown = false`, calls `BroadcastSkillReady`.

**Death cleanup** (`ClearAllCooldowns`, triggered by `OnRS_CharacterStateChanged(Dead)`):
1. Cancels `ActiveExecution` (clears Unstoppable's `RevertTimer` + immediately reverts effects)
2. Iterates all `SkillStates`, clears cooldown timers, broadcasts `SkillReady` for each
3. Resets energy to `0.0` via `CombatRuntimeState`

**EndPlay**: clears all cooldown timers + energy regen timer, removes RS delegate bindings.

#### Energy System

Energy is stored in `UCombatRuntimeState::CurrentEnergy` (clamped `[0, MaxEnergy]`). `SkillComponent` owns configuration and the regen timer.

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

**Forwarder pattern**: `CombatRuntimeState` holds the value and broadcasts. `SkillComponent` binds to RS delegates and re-broadcasts through its own delegates for UI binding compatibility.

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

**Speed modifier bridge** — `AddSpeedModifier(Tag, Mult)` / `RemoveSpeedModifier(Tag)` forward to `PlayerRuntimeState`, which maintains a `TMap<FGameplayTag, float>` and a cached composite multiplier. Character `Tick` reads this:
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
