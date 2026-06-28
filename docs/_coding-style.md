# 代码编写规范

## 1. 花括号换行（Allman 风格）

函数、if/else、for、while、switch 等语句的花括号**独占一行**。

```cpp
// 正确
void Foo()
{
    if (Condition)
    {
        // ...
    }
    else
    {
        // ...
    }

    for (int32 i = 0; i < N; ++i)
    {
        // ...
    }
}

// 错误
void Foo() {
    if (Condition) {
        // ...
    }
}
```

**例外**：简单的 getter/setter 内联函数、`FORCEINLINE` 函数可保持单行。

```cpp
FORCEINLINE int32 GetCount() const { return Count; }
```

## 2. 过长的函数参数换行

当函数参数过多无法在一行内容纳时，参数**对齐缩进**到第一个参数位置，或统一缩进一个 Tab。

```cpp
// 正确 — 对齐到第一个参数
void URunGameFloorSubsystem::InitializeFloorSystem(
    const TArray<TSoftClassPtr<AActor>>& InStraightClasses,
    const TArray<TSoftClassPtr<AActor>>& InTurnClasses,
    int32 InPreAllocateCount)
{
    // ...
}

// 也可接受 — 统一缩进一个 Tab
void LongFunctionName(
    int32 Param1,
    float Param2,
    bool Param3)
{
    // ...
}
```

## 3. 函数注释

每个函数（除内联 getter/setter、`FORCEINLINE` 函数外）应当具备简洁注释，说明其用途。

```cpp
// 正确
/** Spawn initial floor chain from start position */
void SpawnInitialFloors(const FTransform& StartTransform, int32 StraightCount, int32 RandomCount);

// 正确 — 简单注释
// Recycle all floors farther than MaxDistance from player
void RecycleDistantFloors(const FVector& PlayerLocation, float MaxDistance);

// 不需要注释 — 内联 getter
FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
```

注释风格：公有函数使用 `/** ... */`，私有/保护函数使用 `// ...`。注释说**做什么**，不解释实现细节。

## 4. Unreal 引擎反射宏（严格紧凑原则）

// 正确：注释在最上方，宏紧贴函数声明，中间无空行
/** 处理角色死亡逻辑并切换至死亡摄像机 */
UFUNCTION(BlueprintCallable, Category = "RunGame|Character")
void Die();

// 正确：宏紧贴变量声明
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RunGame|Speed")
float MaxWalkSpeed;

// 错误：宏与声明之间有空行（会导致大模型代码拼接错误）
UFUNCTION()

void Die();

// 错误：注释写在了宏与声明之间（会导致 UHT 编译失败，且无法在蓝图中正确提取节点注释）
UFUNCTION(BlueprintCallable, Category = "RunGame|Character")
// 处理角色死亡逻辑
void Die();

## 5. 函数内注释（In-Function Comments）

函数内部的注释应当用于解释代码背后的意图（Why）或复杂的业务逻辑，而不是简单地翻译代码（What）。并使用空行将长函数划分为清晰的逻辑块。

规范要点：

重意图，轻翻译：注释应说明这段代码的目的或特殊边缘情况的处理原因，假设阅读者已经懂 C++。

位置与空行：注释应当写在被注释代码的正上方。为了视觉整洁，在逻辑分块的注释上方保留一个空行。

行首加空格：双斜杠后必须保留一个空格符，即 // 注释内容。

慎用行尾注释：除了解释极短的单行变量或枚举，尽量避免将注释写在代码段的末尾右侧。

C++
// 正确（示范逻辑分块与说明意图）：
void ARunGameCharacter::DoMove(float Right, float Forward)
{
    // 验证控制器与输入有效性
    if (GetController() == nullptr || Right == 0.0f)
    {
        return;
    }

    const FRotator Rotation = GetController()->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // 处理 90 度十字路口转弯逻辑
    if (bTurn)
    {
        FRotator NewRotation = FRotator(0.f, 90.f * Right, 0.f);
        FQuat QuatA = FQuat(DesireRotation);
        FQuat QuatB = FQuat(NewRotation);
        DesireRotation = FRotator(QuatA * QuatB);

        // 使用四元数旋转当前速度向量，防止角色转向后由于动量不一致导致侧滑掉速
        if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
        {
            FVector CurrentVelocity = MovementComponent->Velocity;
            MovementComponent->Velocity = QuatB.RotateVector(CurrentVelocity);
        }

        bTurn = false;
    }
    // 处理常规赛道左右平移
    else if (!InTurnBox)
    {
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(RightDirection, Right);
    }
}

// 错误（废话注释与拥挤排版）：
void ARunGameCharacter::AddScore()
{
    Score += 10; // 加上10分
    if (Score > 100) // 判断是否大于100
    {
        LevelUp(); // 升级
    }
}

特定标记词（Tagging）：
当存在未完工或需要特别注意的代码时，必须使用全大写的标准前缀，并在后面紧跟冒号和空格。

// TODO: （待办事项，后续需要完善的功能）

// FIXME: （已知存在缺陷或 Bug，需要修复）

// HACK: （不够优雅的临时妥协解法，需解释原因）

// NOTE: （重要提示，提醒其他开发者注意这里的特殊逻辑）

## 6. 中英双语注释（Bilingual Comments）

所有注释和文档字符串必须同时包含中文和英文，格式为：**中文在前，英文在后，以空格分隔**。

```cpp
// 正确 — 函数注释（中文在前，英文在后）
/** 触发角色死亡逻辑 Trigger character death logic */
void Die();

// 正确 — 变量注释
/** 角色最大移动速度 Max walk speed of the character */
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RunGame|Speed")
float MaxWalkSpeed;

// 正确 — 函数内注释
// 验证控制器与输入有效性 Validate controller and input validity
if (GetController() == nullptr || Right == 0.0f)
{
    return;
}

// 正确 — 标记词
// TODO: 需要实现可配置的冷却时间 Need to implement configurable cooldown duration

// 正确 — 内联 getter
/** 获取摄像机吊臂 Get the camera boom */
FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

// 错误 — 仅有英文，缺少中文
/** Get the camera boom */
FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

// 错误 — 仅有中文，缺少英文
/** 获取摄像机吊臂 */
FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
```

**规则要点**：
- 中文描述在前，英文描述在后，中间以空格分隔
- 两类描述内容应对等，表达相同的含义
- 已有代码中的注释暂不要求补齐双语，后续修改时顺带补充
- 标记词（TODO/FIXME/HACK/NOTE）同样适用此规则

## 7. Git 提交规范（Commit Message Convention）

### 格式

```
<主题概括>

 - <具体改动1>
 - <具体改动2>
 - <具体改动3>
```

### 主题（Subject Line）

- **中文**，一行概括本次提交做了什么
- 格式：`<做了什么事> — <关键细节>` 或 `<做了什么事>`
- 可选类型前缀：`feat:` / `fix:` / `refactor:` / `docs:` / `chore:`——新功能/修复/重构/文档/杂项，视改动性质而定
- 不超过 72 字符，结尾不加句号

**示例：**

```
CameraComponent 镜头系统 + Character 重生全链路修复
Trap 自动绑定父 Floor 池子生命周期
修复暂停恢复后的输入阻塞、首帧卡顿、技能冷却显示
feat: CollisionAbility 组件 + IImpactReceiver 接口 + 速度配置蓝图化
```

### 正文（Body）

- 每条改动以 ` - ` 开头（空格+短横+空格）
- 每条描述一个**具体改动区域**，格式：`ClassName/SystemName: 具体改了什么`
- **必须包含**：改了什么文件/类、怎么改的、达到什么目的
- **排序**：核心改动在前，附属改动在后；新增在前，修复在后
- 复杂改动可换行续写（续行缩进对齐正文首字符）
- 纯中文，无需英文翻译

**示例：**

```
CameraComponent 镜头系统 + Character 重生全链路修复

 - GameMode: SpawnPlayer 提前到 CountDown 执行(定位+复位+PRS全复活)，
   运动通过 SetActive(false/true) 由 InGame 控制启停
 - CameraComponent: CountDown 时挂回 SpringArm 并切主菜单视点，
   InGame 仅 Blend 无跳变
 - 各子系统 Dead→Idle 自愈: Health(Revive)、Movement(DesireRotation+Walking)、
   SKill(能量复位)、AnimInstance(停蒙太奇+解冻骨骼)、
   EffectComponent(清定时器+还原材质+重置DissolveElapsed)
```

```
Trap 可破坏 Actor + 碰撞冷却制 + 技能双 Tag 效果

 - ATrap：IImpactReceiver + HealthComponent + DamageDealerComponent，
   Tag Query 过滤、Floor 委托恢复、Mesh 绑定伤害形状
 - CollisionAbilityComponent：float 冷却替代 Timer 黑名单，
   死亡清冷却+阻止重叠、ActiveSkillTag 归零清空、
   OnCollisionStateChanged 委托驱动 Debug UI 实时刷新
 - Debug UI 新增 [Skill Tags] 面板，绑 PRS+碰撞双委托
 - FSkillDefinition 新增 SpeedEffectTag/DefenseEffectTag，
   Unstoppable 双 Tag 两步撤：Duration 到期撤速度→插值完成撤无敌
```

### 不要做的事

- ❌ 笼统描述：`修复bug`、`更新代码`、`改了一些东西`
- ❌ 只写 What 不写 Where：`新增了一个委托`——得说清哪个类、什么委托
- ❌ 英文主题（本项目面向中文开发者）
- ❌ Markdown 标题分段——正文用扁平 bullet list
- ❌ 一条巨型 commit 塞多个不相关的改动——拆成多个 commit

