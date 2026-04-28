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
| `ARunGameGameMode` | Binds `SpawnPlayer` to `OnCountdownComplete`; no longer controls input modes or score. Owns `OnPlayerDeath` delegate (not on GameState). |

### Key Delegates (who declares what)

| Delegate | Owner | Signature |
|---|---|---|
| `OnGameStateChanged` | `ARunGameGameState` | `(ERunGameGameState Old, ERunGameGameState New)` |
| `OnCountdownUpdated` | `ARunGameGameState` | `(int32 CountdownSeconds)` |
| `OnCharacterDeath` | `ARunGameGameState` | `()` — no params |
| `OnPlayerDeath` | `ARunGameGameMode` | `(ARunGameCharacter* PlayerCharacter)` |
| `OnCountdownComplete` | `URunGameTimerSubsystem` | `()` — no params |
| `OnTimeChanged` | `URunGameTimerSubsystem` | `(float NewTime)` |
| `OnScoreChanged` | `ARunGamePlayerState` | `(int64 NewScore)` |
| `OnFloorSystemReady` | `URunGameFloorSubsystem` | `()` — no params |
| `OnInteractionBegin` | `ARunGameInteractiveVolume` | `(ARunGameCharacter* PlayerCharacter)` |
| `OnInteractionEnd` | `ARunGameInteractiveVolume` | `(ARunGameCharacter* PlayerCharacter)` |

### Player Lifecycle

1. **Countdown**: `GameMode::StartGameCountDown()` sets state to `CountDown`. `TimerSubsystem` reacts, reads `GameState->DefaultCountdownSeconds`, starts a 1Hz `FTimerHandle` that decrements `GameState->CountdownSeconds` (broadcasting `OnCountdownUpdated` each tick).
2. **Spawn**: When countdown reaches 0, `TimerSubsystem` sets state to `InGame`, broadcasts `OnCountdownComplete`. `GameMode::SpawnPlayer()` spawns `GameCharacterClass`, calls `Possess`, blends view back to character, sets `bAutoManageActiveCameraTarget = true`.
3. **Death flow** (ordering matters):
   - `ARunGameDeathVolume` → `GameMode::HandlePlayerDeath` → broadcasts `OnPlayerDeath` (GameMode), sets state to `GameOver`.
   - Then broadcasts `GameState::OnCharacterDeath` (bound to `Character::Die` + `Controller::SetInputModeToUIOnly`).
   - `Character::Die()` spawns a death `ACameraActor` at the follow-camera location and blends the view to it.
   - `GameMode` calls `UnPossess()` **before** destroying the character (prevents engine auto-view-reset).
   - Character is destroyed (immediately or delayed via `FTimerHandle`).
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

`ARunGamePlayerController` manages `EnhancedInput` mapping contexts. On possess it binds `Die` + `SetInputModeToUIOnly` to `GameState::OnCharacterDeath`. Input mode and view target are self-managed via `OnGameStateChangedCallback` (the big switch on state). `MainMenuCameraTag` (`FName`, editor-configurable) identifies the main-menu camera placed in the level via tag-based actor search.

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

### Key Dependencies

From `RunGame.Build.cs` — Public: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `AIModule`, `StateTreeModule`, `GameplayStateTreeModule`, `UMG`, `FunctionalTesting`. Private: `Slate`, `SlateCore`.

## Logging

Use `UE_LOG(LogRunGame, ...)`. Category declared in `RunGame.h`, defined in `RunGame.cpp`. Format: `UE_LOG(LogRunGame, Warning/Error, TEXT("ClassName: message with %s formatting"), *Args)`.
