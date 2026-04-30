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
| `ARunGameGameMode` | `InGame` → spawns player via `SpawnPlayer()`; orchestrates floor system init/reset; owns `OnPlayerDeath` delegate. |

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
| `OnHealthChanged` | `UHealthComponent` | `(float CurrentHP, float MaxHP, float Delta)` |
| `OnDeath` | `UHealthComponent` | `(FGameplayTag DamageType, AActor* DeathCauser)` |
| `OnCharacterDied` | `ARunGameCharacter` | `(FGameplayTag DamageType, ARunGameCharacter* DeadCharacter)` |

### Player Lifecycle

1. **Countdown**: `GameMode::StartGameCountDown()` sets state to `CountDown`. `TimerSubsystem` reacts, reads `GameState->DefaultCountdownSeconds`, starts a 1Hz `FTimerHandle` that decrements `GameState->CountdownSeconds` (broadcasting `OnCountdownUpdated` each tick).
2. **Spawn**: When countdown reaches 0, `TimerSubsystem` sets state to `InGame`, broadcasts `OnCountdownComplete`. `GameMode::SpawnPlayer()` spawns `GameCharacterClass`, calls `Possess`, blends view back to character, sets `bAutoManageActiveCameraTarget = true`.
3. **Death flow** — fully managed by `ARunGameCharacter::Die(FGameplayTag DamageType, float DestroyDelay)`:
   - Two entry paths: (A) `ARunGameDeathVolume` calls `Character->Die(FGameplayTag(), DeathDelay)` directly; (B) `HealthComponent::OnDeath` → `OnHealthDepleted` → `Die(DamageType)`.
   - `Die()` executes in this order:
     1. `DisableMovement` + `StopMovementImmediately` + `StopAllMontages` + `DisableInput`
     2. Spawn death `ACameraActor` at follow-camera location, blend view to it **(before animation to avoid camera jitter)**
     3. Play death montage from `DeathMontages` map (looked up by `FGameplayTag DamageType`)
     4. `SetGameState(GameOver)` — reactive listeners (HUD, etc.) respond automatically
     5. `UnPossess()` — prevents engine auto-view-reset on Pawn destruction
     6. `OnCharacterDied.Broadcast(DamageType, this)` → `Controller` reacts with `SetInputModeToUIOnly`
     7. `Destroy()` — immediate or via `FTimerHandle` with `DestroyDelay`
4. **Respawn**: `GameMode::StartNewGame()` → same countdown path → `SpawnPlayer()` re-possesses a new pawn and blends the view back.

### World Subsystems

- **`URunGameFloorSubsystem`** — Procedural floor generation via object pooling. `ARunGameGameMode` holds the configuration (`StraightFloorClasses`, `TurnFloorClasses`, `PreAllocateFloorCount`) and calls `InitializeFloorSystem()` at `BeginPlay`. Async-loads floor classes from `TSoftClassPtr` arrays, then `SpawnInitialFloors()` lays out initial straight + random floors tracking `NextSpawnTransform`. `RequestNextFloor()` (80% straight / 20% turn weighted random) and `RequestFloorAt()` serve new floors; `RecycleDistantFloors()` hides distant floors and returns them to the pool. `ReturnFloor()` and `HideAllActiveFloors()` also recycle floors. Pool is per-type: `TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap`. Active floors tracked in `TArray<TObjectPtr<AFloorBase>> ActiveFloors`. `OnFloorSystemReady` delegate signals async load completion; `GameMode` binds to it for initial floor spawn.
- **`URunGameTimerSubsystem`** — Implements `FTickableGameObject`. **Purely a timer — knows nothing about scoring.** Countdown uses `FTimerManager` at 1Hz; the forward game timer increments `TotalTimeSeconds` every `Tick`. All start/stop is driven reactively by `OnGameStateChanged`. Exposes delegates: `OnCountdownComplete`, `OnTimeChanged` (broadcasts every tick while running).

### Scoring (cubic polynomial, int64-safe)

`ARunGamePlayerState` runs a 0.1s `FTimerHandle` callback (`CalculateScoreProcess`). On each tick it reads elapsed time from `TimerSubsystem` and computes:

```
Multiplier = 1 + minutes² + 0.5·minutes³
ScoreToAdd = floor(10 × Multiplier)
```

All score variables are `int64`. The timer is paused/unpaused reactively by `OnGameStateChanged`. `Tick` is disabled (`bCanEverTick = false`). Public API: `SetScoringActive(bool)`, `AddScore(int64)`, `SetRunGameScore(int64)`, `GetRunGameScore()`. Delegate: `OnScoreChanged(int64 NewScore)`.

### Input

`ARunGamePlayerController` manages `EnhancedInput` mapping contexts. On possess it binds `SetInputModeToUIOnly` to `Character->OnCharacterDied`. Input mode and view target are self-managed via `OnGameStateChangedCallback` (the big switch on state). `MainMenuCameraTag` (`FName`, editor-configurable) identifies the main-menu camera placed in the level via tag-based actor search.

### HUD / UI

`ARunGameHUD` owns `CurrentUIMap` mapping each game state to a widget class. Reactively switches widgets via `OnGameStateChangedCallback → UpdateUIForState → ShowUI`. Widget classes (in `RunGame/HUD/`): `RunGameMainMenu`, `RunGameCountDown`, `RunGameInGame`, `RunGamePause`, `RunGameGameOver`.

### Cleanup Pattern

All classes that bind to `GameState::OnGameStateChanged` unbind in `EndPlay` using `RemoveDynamic`. Subsystems unbind in `Deinitialize`. Timer handles (`FTimerHandle`) are cleared on teardown.

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
- Properties: `MaxHP` (EditAnywhere, ClampMin=1), `CurrentHP`, `bIsDead`
- Public API: `ApplyDamage(Damage, FGameplayTag DamageType, Causer)`, `Heal(Amount, Healer)`, `Revive(RestoreHP)`
- Delegates: `FOnHealthChanged(CurrentHP, MaxHP, Delta)` — positive Delta=heal, negative=damage; `FOnDeath(DeathCauser)`
- Tick disabled (`bCanEverTick = false`)
- Does NOT inherit from `IDamagable` — the owning Actor implements the interface and forwards to this component

**`UDamageDealerComponent`** (UBoxComponent, `RunGame/Actor/Component/`):
- Trigger overlap → checks `OtherActor->Implements<UDamagable>()` → calls `IDamagable::Execute_OnTakeDamage`
- Properties: `DamageAmount` (float), `DamageType` (FGameplayTag)

**`ARunGameCharacter`** integration:
- Implements `IDamagable`, owns `UHealthComponent*` via `CreateDefaultSubobject`
- `BeginPlay` binds `HealthComponent->OnDeath` → `OnHealthDepleted` → `Die(DamageType)`
- All interface `_Implementation` functions forward to `HealthComponent`
- `Die(FGameplayTag, DestroyDelay)` is the single death entry point — handles stop → camera → montage → GameState → UnPossess → broadcast → destroy
- `OnCharacterDied` (BlueprintAssignable) — broadcast by `Die()`, Controller binds for input mode change
- `DeathMontages` (`TMap<FGameplayTag, UAnimMontage*>`) — maps damage type to death animation
- Two entry paths converge: (1) HP depletion → `OnHealthDepleted`, (2) death volume → `DeathVolume` calls `Die()` directly
- `GameMode` no longer participates in death handling; `HandlePlayerDeath` and `OnPlayerDeath` are removed

### Key Dependencies

From `RunGame.Build.cs` — Public: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `AIModule`, `StateTreeModule`, `GameplayStateTreeModule`, `UMG`, `FunctionalTesting`, `GameplayTags`. Private: `Slate`, `SlateCore`.

### Include Conventions

- Forward-declare classes in headers where possible; include in .cpp
- Include paths are relative to the module root (e.g. `#include "Actor/Component/HealthComponent.h"`, `#include "Interfaces/Damagable.h"`)
- For `FGameplayTag`, include `GameplayTagContainer.h`

### Component Naming

- Components live under subdirectories of their owner type: `Actor/Component/` for actor-level components
- Prefix with their domain (e.g. `UHealthComponent` not `URunGameHealthComponent`)

### Scope

- Only modify `ARunGameCharacter` and non-variant classes
- Never touch `Variant_Combat/`, `Variant_Platforming/`, `Variant_SideScrolling/`

## Logging

Use `UE_LOG(LogRunGame, ...)`. Category declared in `RunGame.h`, defined in `RunGame.cpp`. Format: `UE_LOG(LogRunGame, Warning/Error, TEXT("ClassName: message with %s formatting"), *Args)`.
