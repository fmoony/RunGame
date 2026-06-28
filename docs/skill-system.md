# Skill System — 技能系统

> 从 `CLAUDE.md` 提取，描述三层技能架构：输入绑定 → 状态门控 → SkillComponent 验证分发。

Three-layer architecture — input binds to Character, Character guards by state, SkillComponent validates and dispatches to transient execution objects.

**Files**: `Skill/RunGameSkillConfigData.h`, `Skill/RunGameSkillExecution.h/.cpp`, `Actor/Component/SkillComponent.h/.cpp`, `HUD/RunGameSkillSlot.h/.cpp`

## Data Flow

```
InputAction (Started)
  → ARunGameCharacter::ActivateSkillByTag(SkillTag)     [state guard: blocks Dead/Sliding]
    → USkillComponent::TryActivateSkill(SkillTag)       [cooldown + energy + CanExecute checks]
      → USkillExecutionBase::Execute(Owner, SkillTag)    [actual gameplay effect]
      → OnSkillActivated.Broadcast()                     [UI via SkillComponent]
```

## Core Data Structures

### `FSkillDefinition`

In `RunGameSkillConfigData.h` — per-skill configuration:

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

## Skill Execution Hierarchy

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

## SkillComponent — `USkillComponent`

ActorComponent on Character.

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

## Energy System

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

## Input Binding (dynamic)

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

## Character Integration

**`ActivateSkillByTag(FGameplayTag)`** — state guard before SkillComponent:
- Reads `PlayerRuntimeState::GetCharacterState()`
- Rejects if `Dead` or `Sliding`; passes if `Idle`, `Airborne`, `Turning`
- Delegates to `SkillComponent->TryActivateSkill(SkillTag)`

**Speed modifier bridge** — `AddSpeedModifier(Tag, Mult)` / `RemoveSpeedModifier(Tag)` are on `ARunGameCharacter`. Maintains a `TMap<FGameplayTag, float>` and a cached composite multiplier. `URunGameMovementComponent` reads the composite via `GetCompositeSpeedMultiplier()`. Character `Tick` reads this:
```
FinalMaxSpeed = MaxSpeedCurve × CompositeSpeedMultiplier
```

## RuntimeState Integration

**`UCombatRuntimeState`** holds skill/energy data: `CurrentEnergy`, `MaxEnergy`, and delegates `OnSkillActivated`, `OnSkillReady`, `OnSkillExecuted`, `OnEnergyChanged`. Also hosts `BroadcastSkillActivated()` / `BroadcastSkillReady()` / `BroadcastSkillExecuted()` pure-broadcast methods called by SkillComponent.

**`UPlayerRuntimeState`** holds speed modifiers (`TMap<FGameplayTag, float>` + `CachedCompositeSpeedMultiplier`) and `CurrentCharacterState` (used for skill gating). `AddSpeedModifier`/`RemoveSpeedModifier` maintain the composite product; `ClearSpeedModifiers` empties the map and resets to `1.0f` (called on death).

## Skill HUD

**`URunGameSkillSlot`** (per-slot widget, created dynamically by `URunGameInGame`):
- Bound widgets: `SkillIcon` (`UImage`), `CooldownOverlay` (`UImage`), `KeyHintText` (`UTextBlock`)
- `SetupSlot(FSkillDefinition, FGameplayTag, USkillComponent*)` — async-loads icon, creates `MaterialInstanceDynamic` for radial cooldown fill, binds to `OnSkillActivated`/`OnSkillReady`/`OnEnergyChanged`
- Cooldown overlay driven by MID scalar parameter (`CooldownPercentParamName`, default `"CooldownPercent"`). `1.0` = full cooldown, `0.0` = ready
