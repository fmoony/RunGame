// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoinSpawnerComponent.generated.h"

class ACoin;
class USplineComponent;
class UStaticMeshComponent;
class UInstancedStaticMeshComponent;

/** Fallback pattern when spline has no points */
UENUM()
enum class ECoinSpawnPattern : uint8
{
	StraightLine,
	DiagonalLine,
	Parabolic,
	MAX,
};

/** Per-floor-class coin spawn configuration, driven by data asset */
USTRUCT(BlueprintType)
struct FCoinSpawnConfig
{
	GENERATED_BODY()

	/** Coin class to spawn. nullptr = no coins */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	TSubclassOf<ACoin> CoinClass;

	/** Coins per row */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	int32 CoinCount = 3;

	/** Number of parallel rows (left/center/right lanes) */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	int32 RowCount = 1;

	/** Lateral spacing between rows in world units */
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	float RowSpacing = 100.0f;

	/** Probability each non-center row spawns (0-1, 1 = always all rows) */
	UPROPERTY(EditDefaultsOnly, Category = "Coin", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RowSpawnChance = 1.0f;

	/** Fallback pattern when spline is empty */
	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	ECoinSpawnPattern SpawnPattern = ECoinSpawnPattern::StraightLine;

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector StartOffset = FVector(0.0f, 0.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector LineDirection = FVector(100.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	FVector EndOffset = FVector(200.0f, 100.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	float ParabolaHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Coin|Fallback")
	float CoinSpacing = 80.0f;

	bool HasValidCoinClass() const;
};

UCLASS(ClassGroup = (RunGame), meta = (BlueprintSpawnableComponent))
class RUNGAME_API UCoinSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoinSpawnerComponent();

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Applies coin spawn configuration from the data asset */
	void ApplyConfig(const FCoinSpawnConfig& Config);

	/** 刷新编辑器预览实例，由 Construction 或按钮调用 / Refresh editor preview instances from construction or buttons */
	void RefreshEditorPreviewCoins(bool bLogWarnings = false);

	/** Acquires coins from subsystem and places them on this floor */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Coin")
	void SpawnCoins();

	/** Returns all managed coins to the pool */
	UFUNCTION(BlueprintCallable, Category = "RunGame|Coin")
	void DespawnCoins();

protected:
	/** 编辑器预览用金币配置；运行时仍由 FloorSubsystem 注入配置 / Coin config for editor preview; runtime config is still injected by FloorSubsystem */
	UPROPERTY(EditAnywhere, Category = "RunGame|Placement", meta = (DisplayName = "Preview Coin Config"))
	FCoinSpawnConfig EditorPreviewCoinConfig;

	/** 编辑器中自动创建 SpawnLine 样条 / Automatically create a SpawnLine spline in editor */
	UPROPERTY(EditAnywhere, Category = "RunGame|Placement")
	bool bAutoCreateSpawnLine = true;

	/** 自动创建或查找的样条组件名称 / Component name used when auto-creating or finding the spline */
	UPROPERTY(EditAnywhere, Category = "RunGame|Placement")
	FName SpawnLineComponentName = TEXT("CoinSpawnLine");

	/** Construction 刷新时自动显示编辑器预览 / Automatically show editor preview during construction refresh */
	UPROPERTY(EditAnywhere, Category = "RunGame|Placement")
	bool bEnableEditorPreview = true;

	/** 金币自身缩放是否继承地板缩放 / Whether spawned coin scale should inherit owner floor scale */
	TSubclassOf<ACoin> CoinClass;

	int32 CoinCount = 3;

	int32 RowCount = 1;

	float RowSpacing = 100.0f;

	float RowSpawnChance = 1.0f;

	ECoinSpawnPattern SpawnPattern = ECoinSpawnPattern::StraightLine;

	FVector StartOffset = FVector(0.0f, 0.0f, 100.0f);

	FVector LineDirection = FVector(100.0f, 0.0f, 0.0f);

	FVector EndOffset = FVector(200.0f, 100.0f, 0.0f);

	float ParabolaHeight = 200.0f;

	float CoinSpacing = 80.0f;

	bool bEnableCoinSpawn = true;

private:
	/** 缓存 Owner 上的样条，供运行时和编辑器按钮共用 / Cache owner spline for runtime and editor tool calls */
	void CacheOwnerSpline();

	/** 查找或创建 SpawnLine 样条 / Find or create the SpawnLine spline */
	USplineComponent* EnsureSpawnLineComponent(bool bAllowCreate);

	/** 应用编辑器预览配置，保持预览与运行时地板配置一致 / Apply editor preview config so preview matches runtime floor config */
	bool ApplyPreviewConfig(bool bLogWarnings);

	/** 获取金币蓝图默认网格，供编辑器预览组件复用 / Get the coin class default mesh for editor preview components */
	UStaticMeshComponent* GetPreviewCoinMeshTemplate() const;

	FVector GetCoinDefaultActorScale() const;

	/** 构造最终金币 Transform，位置已计算完成，仅统一补齐旋转和缩放 / Build final coin transform after position calculation, normalizing rotation and scale */
	FTransform MakeCoinTransform(const FTransform& RelativeTransform, const FTransform& ParentTransform) const;

	void ClearEditorPreviewCoins();

	/** Computes world-space transforms: spline first, enum fallback, multi-row */
	TArray<FTransform> CalculateCoinTransforms() const;

	/** Applies row offset perpendicular to the given local direction */
	void ApplyRowOffset(FVector& InOutWorldPos, const FVector& PerpendicularWorldDir, int32 RowIndex) const;

	/** Computes world-space transforms along the spline for one row */
	TArray<FTransform> CalculateSplineTransforms() const;

	void CalculateStraightLine(TArray<FTransform>& OutTransforms) const;
	void CalculateDiagonalLine(TArray<FTransform>& OutTransforms) const;
	void CalculateParabolic(TArray<FTransform>& OutTransforms) const;

	/** Coins currently managed by this component */
	UPROPERTY(Transient)
	TArray<TObjectPtr<ACoin>> SpawnedCoins;

	/** 编辑器预览用临时网格组件，不保存到资产 / Transient mesh components used only for editor preview */
	UPROPERTY(Transient)
	TObjectPtr<UInstancedStaticMeshComponent> PreviewCoinInstances;

	/** Cached spline from owner, populated in BeginPlay */
	UPROPERTY()
	TObjectPtr<USplineComponent> CachedSpline;
};
