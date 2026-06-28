# World Subsystems — 世界子系统

> 从 `CLAUDE.md` 提取，描述四个世界子系统：地板生成、计时器、金币池、角色运行时状态。

- **`URunGameFloorSubsystem`** — Procedural floor generation via object pooling. `ARunGameGameMode` holds the configuration (`StraightFloorClasses`, `TurnFloorClasses`, `PreAllocateFloorCount`) and calls `InitializeFloorSystem()` at `BeginPlay`. Async-loads floor classes from `TSoftClassPtr` arrays, then `SpawnInitialFloors()` lays out initial straight + random floors tracking `NextSpawnTransform`. `RequestNextFloor()` (80% straight / 20% turn weighted random) and `RequestFloorAt()` serve new floors; `RecycleDistantFloors()` hides distant floors and returns them to the pool. `ReturnFloor()` and `HideAllActiveFloors()` also recycle floors. Pool is per-type: `TMap<TSubclassOf<AActor>, TArray<AFloorBase*>> PooledFloorsMap`. Active floors tracked in `TArray<TObjectPtr<AFloorBase>> ActiveFloors`. `OnFloorSystemReady` delegate signals async load completion; `GameMode` binds to it for initial floor spawn.

- **`URunGameTimerSubsystem`** — Implements `FTickableGameObject`. **Purely a timer — knows nothing about scoring.** Countdown uses `FTimerManager` at 1Hz; the forward game timer increments `TotalTimeSeconds` every `Tick`. All start/stop is driven reactively by `OnGameStateChanged`. Exposes delegates: `OnCountdownComplete`, `OnTimeChanged` (broadcasts every tick while running).

- **`URunGameCoinSubsystem`** — Coin object pooling via LIFO stack (`TArray::Pop`/`Add`). `PreAllocateCoins(CoinClass, Count)` spawns hidden coins into per-class sub-pools (`TMap<TSubclassOf<ACoin>, TArray<ACoin*>>`). `AcquireCoin()` pops from pool or spawns new; `ReturnCoin()` pushes back via `RemoveSwap` from `ActiveCoins` then `Add` to pool. Coins stay hidden in-place (no off-screen relocation). Delegates: `OnCoinSubsystemReady`.

- **`UPlayerRuntimeState`** — Character state machine + animation event bus. Holds `CurrentCharacterState` and enforces the transition matrix via `IsCharacterStateTransitionAllowed`. Delegates: `OnCharacterStateChanged`, `OnCharacterDied`, `OnHitReaction`, `OnDeathAnimationFinished`. `ResetForNewGame()` clears state to Idle.
