# 性能对比测试 Performance Benchmark

## 目标 Goal

证明对象池和异步加载对地板驱动的运行时负载有效：

- 生成 `FloorSegmentCount` 个地板段，默认 `200`。
- 金币不单独生成，由每个地板的 `CoinSpawnerComponent` 按 `TestFloorConfig.GlobalCoinConfig` 生成。
- 陷阱不单独生成，只统计地板蓝图/子 Actor 附带的陷阱。
- 对比 `Pool ON / Pool OFF`。
- 可选对比 `Async Load / Sync Load`。

## 测试入口 Test Entry

推荐创建一个 `ATest_PoolAsyncPerformance` 的蓝图子类，例如：

`/Game/_Test/PoolAsyncBenchmark/BP_Test_PoolAsyncPerformance`

然后在测试关卡中放置该蓝图实例，并配置：

- `TestFloorConfig`: 使用真实地板配置资产。
- `FloorSegmentCount`: 默认 `200`。
- `bIncludeSynchronousLoadComparison`: 默认开启，额外输出 `Pool_ON_SyncLoad` 行。
- `bAutoRunOnBeginPlay`: C++ 默认关闭；测试蓝图里建议显式开启。

测试资产建议放在独立目录：

`/Game/_Test/PoolAsyncBenchmark/BP_Test_PoolPerformance`

如果已有资产在别的测试目录中，请在 Unreal Editor 的 Content Browser 里使用 Move 操作迁移，避免直接移动 `.uasset` 导致关卡引用或重定向器异常。

放置 Actor、配置好资产，并在测试蓝图中开启 `bAutoRunOnBeginPlay` 后，直接 PIE，测试会在 BeginPlay 下一帧自动启动并输出 CSV。

如果同一关卡已有 `BP_Test_FloorRecycling`，建议临时禁用或移除它，只保留 `BP_Test_PoolAsyncPerformance`，避免两个测试同时操作 `URunGameFloorSubsystem`。

## 输出 Output

测试完成后写入：

`Saved/Profiling/RunGamePoolBenchmark/PoolAsyncBenchmark.csv`

## 指标口径 Metrics

- `GameThreadMs`: 按地板配置生成 `FloorSegmentCount` 个地板段的主线程耗时，包含这些地板自动生成的金币和附带陷阱。
- `SpawnActorCount`: 玩法负载阶段由地板系统和金币系统记录到的 `SpawnActor` 次数，不包含预分配阶段。
- `GCCount`: 本测试主动触发的 GC 次数。
- `ActiveActorCount`: 负载生成完成、回收前的非隐藏 Actor 数量。
- `ActiveFloorCount`: 地板系统当前激活的地板数量。
- `ActiveCoinCount`: 金币系统当前激活的金币数量，由地板配置决定。
- `ActiveTrapCount`: 当前非隐藏陷阱 Actor 数量，由地板蓝图/子 Actor 配置决定。
- `PeakUsedPhysicalMB`: 测试阶段采样到的进程物理内存峰值。
- `RecycleMs`: 调用 `RecycleDistantFloors` 回收地板的耗时。
- `LoadMs`: `FloorConfig` 中软引用地板类加载完成耗时。
- `PreAllocateMs`: 地板池预分配耗时。

## 预期判断 Expected Reading

- `Pool_ON_AsyncLoad` 的 `SpawnActorCount` 应低于 `Pool_OFF_AsyncLoad`，因为地板从池中取出；地板自动生成的金币也会通过金币池参与回收/复用。
- `Pool_ON_AsyncLoad` 的 `GameThreadMs` 应低于 `Pool_OFF_AsyncLoad`，预分配成本体现在 `PreAllocateMs`。
- `RecycleMs` 应在 Pool ON 下明显更低，因为地板回收到池中，而 Pool OFF 会销毁地板。
- `LoadMs` 用于观察异步加载完成成本；`Pool_ON_SyncLoad` 用于同步加载对照。
