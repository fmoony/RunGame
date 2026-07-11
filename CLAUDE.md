# CLAUDE.md

> UE 5.6 3D 跑酷游戏 — 反应式状态机架构，C++ 原生开发，集成 MCP 自动化桥接。
> UE 5.6 3D runner game — reactive state machine architecture, native C++, MCP automation bridge.

## 构建 / 运行 Build / Run

- 右键 `RunGame.uproject` → Generate Visual Studio project files → 编译 `RunGameEditor` 目标
- 或在 Unreal Editor 中直接打开 `.uproject`，Ctrl+Alt+F11 热重载
- 无 CI/测试套件。FunctionalTest（如 `Test_FloorRecycling`）通过 Editor 的 **Functional Test** 面板运行
- Open `RunGame.uproject` (UE 5.6), build `RunGameEditor` target. No CI. Functional Tests via Editor panel.

## 最高优先级架构约束 Top-Level Architecture Rules

1. **反应式状态机 Reactive state machine** — `ARunGameGameState` 是全局游戏状态唯一真相源，`SetGameState()` 广播 `OnGameStateChanged`。跨系统状态变化使用事件响应；Character 内部控制栈由 Pipeline 调用明确的领域入口。`ARunGameGameState` is the global state source; the local Character control stack uses explicit Pipeline-to-domain calls.
2. **状态变更流程 State change pattern** — `Guard → Validate → Save → Modify → Broadcast`，适用于 GameState 和 CharacterState。
3. **通信边界 Communication boundary** — GameMode、WorldSubsystem、HUD 等跨系统通信走委托；同一 Character 内允许 `Input → Pipeline → Locomotion/Movement/Skill` 的显式领域调用。GameMode 不直接处理死亡——它订阅 `OnCharacterDied`。
4. **单一数据源 Single source of truth** — 游戏状态看 `ARunGameGameState`，角色状态看 `UPlayerRuntimeState`。
5. **双语注释 Bilingual comments** — 新代码/修改代码必须中文在前、英文在后。详见 `docs/_coding-style.md`。Chinese first, English second.

## 模块导航 Module Index

| 想看什么系统 What to read | 去看哪个文件 File |
|--------------------------|-------------------|
| 反应式状态机、核心类型、委托表、玩家生命周期 Architecture, types, delegates, lifecycle | `docs/architecture.md` |
| 角色状态机（过渡矩阵、土狼时间、二段跳）Character state machine | `docs/character.md` |
| 世界子系统（地板/Timer/金币/PlayerRuntimeState）Subsystems | `docs/subsystems.md` |
| 健康/伤害系统 Health & damage | `docs/health-system.md` |
| 技能系统（含能量系统）Skill system & energy | `docs/skill-system.md` |
| 金币/收集品 + 计分 Coin system & scoring | `docs/coin-and-scoring.md` |
| 输入、输入缓冲、动画、HUD/UI Input, animation, UI | `docs/input-animation-ui.md` |
| 编码规范 Coding conventions | `docs/_coding-style.md` |
| 项目概览 Project overview | `README.md` |

## 禁止事项 Rules

- **禁止修改 Never touch** `Source/RunGame/Variant_Combat/`、`Variant_Platforming/`、`Variant_SideScrolling/`
- **禁止无边界跨系统调用 No unbounded cross-system calls** — 跨系统走委托；Character 控制栈只允许 Pipeline 调用领域入口
- **禁止绕过 Set 函数修改状态 Never modify GameState/CharacterState outside their Set functions**
- **禁止使用 UE 内置伤害系统 No UE built-in damage** — 用 `IDamagable` + `UHealthComponent` instead of `TakeDamage`/`ApplyDamage`/`UDamageType`

## 常用入口 Quick Tools

| 场景 Scenario | 命令 Command |
|------|------|
| **代码符号查询 Code symbol lookup** | `codegraph.cmd explore "<query>"`（Windows PowerShell 必须显式使用 `.cmd`，避免执行策略拦截 `codegraph.ps1`） |
| 代码审查 Code review | `/code-review` |
| 简化重构 Simplify | `/simplify` |
| 验证改动 Verify | `/verify` |
| 安全审查 Security review | `/security-review` |
| 追踪委托链 Trace delegates | `.reasonix/skills/trace-delegate.md` |

## 开发流程 Dev Flow

```
改代码 → 热重载编译 → PIE 验证 → FunctionalTest → 提交
Edit C++ → Ctrl+Alt+F11 → Alt+P → Test panel → Commit
```
