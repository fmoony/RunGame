# Health / Damage System — 健康/伤害系统

> 从 `CLAUDE.md` 提取，描述自定义伤害系统（不使用 UE 内置 TakeDamage）。

Does NOT use UE's built-in `TakeDamage`/`ApplyDamage`/`UDamageType`. Custom system via `IDamagable` interface + `UHealthComponent` + `UDamageDealerComponent`.

## Data Flow

```
DamageDealerComponent::OnOverlap
  → OtherActor->Implements<UDamagable>()?
    → IDamagable::Execute_OnTakeDamage(Actor, Damage, FGameplayTag DamageType, Causer)
      → Actor::OnTakeDamage_Implementation → HealthComponent->ApplyDamage(...)
        → CurrentHP -= Damage → broadcast OnHealthChanged / OnDeath
```

## `IDamagable`

Blueprintable interface, `RunGame/Interfaces/`:
- `OnTakeDamage(float, FGameplayTag, AActor*)`, `OnTakeHealing(float, AActor*)`, `OnDeath(AActor*)` — all `BlueprintNativeEvent`
- `GetCurrentHP()`, `GetMaxHP()`, `IsDead()` — query functions, `BlueprintNativeEvent`

## `UHealthComponent`

ActorComponent, `RunGame/Actor/Component/`:
- Properties: `MaxHP` (EditAnywhere, ClampMin=1), `CurrentHP`, `bIsDead`, `bIsInvincible`
- Public API: `ApplyDamage(Damage, FGameplayTag DamageType, Causer)`, `Heal(Amount, Healer)`, `Revive(RestoreHP)`, `SetInvincible(bool)`, `IsInvincible()`
- Delegates: `FOnHealthChanged(CurrentHP, MaxHP, Delta)`, `FOnDamageTaken(Damage, DamageType, DamageCauser)`, `FOnDeath(FGameplayTag DamageType, AActor* DeathCauser)`, `FOnInvincibilityChanged(bool bNewInvincible)`
- Tick disabled (`bCanEverTick = false`)
- `bIsInvincible` is orthogonal to the character state machine — it can coexist with any `ERunGameCharacterState`. `SetInvincible()` follows Guard→Modify→Broadcast, broadcasting `OnInvincibilityChanged` when the value changes.
- Does NOT inherit from `IDamagable` — the owning Actor implements the interface and forwards to this component

## `UDamageDealerComponent`

UBoxComponent, `RunGame/Actor/Component/`:
- Trigger overlap → checks `OtherActor->Implements<UDamagable>()` → calls `IDamagable::Execute_OnTakeDamage`
- Properties: `DamageAmount` (float), `DamageType` (FGameplayTag)

## `ARunGameCharacter` Integration

- Implements `IDamagable`, owns `UHealthComponent*` via `CreateDefaultSubobject`
- Character status tracked by `UPlayerRuntimeState::CurrentCharacterState` — authoritative source (see `docs/character.md`)
- `BeginPlay` binds `HealthComponent->OnDeath` → `OnHealthDepleted` → `Die(DamageType)`, and `OnCharacterStateChanged` → `OnCharacterStateChangedCallback` for reactive slide/turn/death behavior
- All `IDamagable` `_Implementation` functions forward to `HealthComponent`; `IsDead_Implementation()` delegates to `HealthComponent::IsDead()` which checks `CurrentHP <= 0.0f` (single source of truth — no separate `bIsDead` flag)
- `Die(FGameplayTag DamageType, float DestroyDelay = 3.0f)` — spawns death camera, then calls `SetCharacterState(Dead)` (state machine guards against double-entry), then broadcasts `OnCharacterDied`. Notably does **NOT** call `SetGameState(GameOver)` — GameMode listens to `OnCharacterDied` and decides game-over (proper IoC)
- `OnCharacterDied` (BlueprintAssignable) — broadcast inside `Die()`, also on `UPlayerRuntimeState`
- Death montages configured on `URunGameAnimInstance::DeathMontages` (moved from Character)
- `PRS->OnDeathAnimationFinished` — dispatch point for dissolve, bound in Character `BeginPlay`
- **Double jump**: `bAirJumpAvailable` flag managed by `OnCharacterStateChangedCallback` (granted on CoyoteTime/ground-jump/landing, consumed on air-jump). `CanJumpInternal_Implementation` overridden to allow jump during CoyoteTime or Airborne with flag set. `DoJumpStart` bypasses buffer for Airborne double-jump.
