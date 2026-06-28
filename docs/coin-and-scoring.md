# Coin System & Scoring — 金币系统与计分

> 从 `CLAUDE.md` 提取，描述收集品体系、金币生成器和计分公式。

## Coin System

### `ACollectibleBase`

`Actor/Collectible/`:
- Abstract base for collectible items. Components: `UStaticMeshComponent` (visual, root) + `USphereComponent` (trigger, `ECC_Pawn` overlap only).
- Tick-driven animation: `AddActorLocalRotation` self-rotation + sinusoidal float oscillation (`FloatAmplitude`, `FloatFrequency`).
- Pool lifecycle: `ActivateCollectible(FTransform)` sets location only (preserves blueprint scale/rotation), unhides, enables collision + tick. `DeactivateCollectible()` hides and flags `bIsCollected = true`.
- `OnCollected(ARunGameCharacter*)` — BlueprintNativeEvent. Base impl: VFX (`SpawnEmitterAtLocation`), SFX (`PlaySoundAtLocation`), broadcast `OnCollectedDelegate`, hide + disable.
- Delegate: `FOnCollectibleCollectedSignature(ACollectibleBase*)`.

### `ACoin`

`Actor/Collectible/`:
- Inherits `ACollectibleBase`. Overrides `OnCollected_Implementation` → calls `ARunGamePlayerState::AddScore(CoinValue)`. `CoinValue` (int64, EditDefaultsOnly).

### `UCoinSpawnerComponent`

`Actor/Component/`, attached to every `FloorBase`:
- `CreateDefaultSubobject` in `FloorBase` constructor. `BeginPlay` caches owner's `USplineComponent` → `CachedSpline`.
- `ApplyConfig(FCoinSpawnConfig)` — copies config from data asset. `SpawnCoins()` checks `bEnableCoinSpawn && CoinClass`, acquires coins from `URunGameCoinSubsystem`, places at computed transforms. `DespawnCoins()` deactivates and returns all managed coins.
- **Multi-row**: `RowCount` parallel lanes offset perpendicular to path direction by `(RowIndex - CenterOffset) * RowSpacing`. `RowSpawnChance` (0-1) controls per-row independent probability.
- **Placement**: Spline-first — if owner has `CoinPathSpline` with points, distribute `CoinCount` coins evenly by distance, offset by `GetRightVectorAtDistanceAlongSpline` per row. Enum fallback — `StraightLine`/`DiagonalLine` (linear interpolation along `LineDirection`) or `Parabolic` (quadratic Bezier with `ParabolaHeight` peak).

### `FCoinSpawnConfig`

In `CoinSpawnerComponent.h`:
- `CoinClass`, `CoinCount`, `RowCount`, `RowSpacing`, `RowSpawnChance`, `SpawnPattern`, `StartOffset`, `LineDirection`, `EndOffset`, `ParabolaHeight`, `CoinSpacing`.

### `UFloorConfigData::GlobalCoinConfig`

Single shared `FCoinSpawnConfig` applied to all floor classes. If `CoinClass` is null, no coins spawn.

### `FFloorClassEntry::bEnableCoinSpawn`

Per-floor-class toggle in data asset. Checked in `RequestFloorAt` before calling `SpawnCoins()`.

### Integration Flow

```
RequestFloorAt → FindLoadedEntry(InClass) → bEnableCoinSpawn?
  → ApplyConfig(FloorConfig->GlobalCoinConfig) → SpawnCoins()
  → CoinSubsystem::AcquireCoin → ActivateCollectible(Transform)
ReturnFloor → DespawnCoins() → DeactivateCollectible → CoinSubsystem::ReturnCoin
```

## Scoring (cubic polynomial, int64-safe)

`ARunGamePlayerState` runs a 0.1s `FTimerHandle` callback (`CalculateScoreProcess`). On each tick it reads elapsed time from `TimerSubsystem` and computes:

```
Multiplier = 1 + minutes² + 0.5·minutes³
ScoreToAdd = floor(10 × Multiplier)
```

All score variables are `int64`. The timer is paused/unpaused reactively by `OnGameStateChanged`. `Tick` is disabled (`bCanEverTick = false`). Public API: `SetScoringActive(bool)`, `AddScore(int64)`, `SetRunGameScore(int64)`, `GetRunGameScore()`. Delegate: `OnScoreChanged(int64 NewScore)`.
