// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Component/CoinSpawnerComponent.h"
#include "Actor/Collectible/Coin.h"
#include "WorldSubsystem/RunGameCoinSubsystem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "RunGame.h"

bool FCoinSpawnConfig::HasValidCoinClass() const
{
	return CoinClass != nullptr;
}

UCoinSpawnerComponent::UCoinSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCoinSpawnerComponent::OnRegister()
{
	Super::OnRegister();

	CacheOwnerSpline();
}

void UCoinSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheOwnerSpline();
}

#if WITH_EDITOR
void UCoinSpawnerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	RefreshEditorPreviewCoins(false);
}
#endif

void UCoinSpawnerComponent::CacheOwnerSpline()
{
	CachedSpline = EnsureSpawnLineComponent(bAutoCreateSpawnLine);
}

USplineComponent* UCoinSpawnerComponent::EnsureSpawnLineComponent(bool bAllowCreate)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	if (CachedSpline && CachedSpline->GetOwner() == Owner)
	{
		return CachedSpline;
	}

	TArray<USplineComponent*> SplineComponents;
	Owner->GetComponents<USplineComponent>(SplineComponents);

	for (USplineComponent* Spline : SplineComponents)
	{
		if (Spline && Spline->GetFName() == SpawnLineComponentName)
		{
			return Spline;
		}
	}

	if (SplineComponents.Num() > 0)
	{
		return SplineComponents[0];
	}

	if (!bAllowCreate)
	{
		return nullptr;
	}

	UWorld* World = Owner->GetWorld();
	if (World && World->IsGameWorld())
	{
		return nullptr;
	}

	USplineComponent* NewSpline = NewObject<USplineComponent>(
		Owner,
		SpawnLineComponentName,
		RF_Transactional);
	if (!NewSpline)
	{
		return nullptr;
	}

	NewSpline->CreationMethod = EComponentCreationMethod::Instance;
	NewSpline->SetMobility(EComponentMobility::Movable);
	NewSpline->SetHiddenInGame(true);

	if (USceneComponent* Root = Owner->GetRootComponent())
	{
		NewSpline->SetupAttachment(Root);
	}

	Owner->AddInstanceComponent(NewSpline);
	NewSpline->RegisterComponent();
	NewSpline->SetRelativeLocation(FVector::ZeroVector);
	NewSpline->ClearSplinePoints(false);
	NewSpline->AddSplinePoint(FVector(0.0f, 0.0f, 100.0f), ESplineCoordinateSpace::Local, false);
	NewSpline->AddSplinePoint(FVector(600.0f, 0.0f, 100.0f), ESplineCoordinateSpace::Local, false);
	NewSpline->UpdateSpline();

	return NewSpline;
}

void UCoinSpawnerComponent::ApplyConfig(const FCoinSpawnConfig& Config)
{
	CoinClass = Config.CoinClass;
	CoinCount = Config.CoinCount;
	RowCount = Config.RowCount;
	RowSpacing = Config.RowSpacing;
	SpawnPattern = Config.SpawnPattern;
	StartOffset = Config.StartOffset;
	LineDirection = Config.LineDirection;
	EndOffset = Config.EndOffset;
	ParabolaHeight = Config.ParabolaHeight;
	CoinSpacing = Config.CoinSpacing;
	RowSpawnChance = Config.RowSpawnChance;
}

bool UCoinSpawnerComponent::ApplyPreviewConfig(bool bLogWarnings)
{
	if (EditorPreviewCoinConfig.HasValidCoinClass())
	{
		ApplyConfig(EditorPreviewCoinConfig);
		return true;
	}

	if (!CoinClass)
	{
		if (bLogWarnings)
		{
			UE_LOG(LogRunGame, Warning, TEXT("CoinSpawnerComponent: PreviewCoins needs PreviewCoinConfig.CoinClass or an already applied runtime config."));
		}
		return false;
	}

	return true;
}

UStaticMeshComponent* UCoinSpawnerComponent::GetPreviewCoinMeshTemplate() const
{
	const ACoin* CoinDefault = CoinClass ? CoinClass->GetDefaultObject<ACoin>() : nullptr;
	return CoinDefault ? CoinDefault->FindComponentByClass<UStaticMeshComponent>() : nullptr;
}

FVector UCoinSpawnerComponent::GetCoinDefaultActorScale() const
{
	const ACoin* CoinDefault = CoinClass ? CoinClass->GetDefaultObject<ACoin>() : nullptr;
	return CoinDefault ? CoinDefault->GetActorScale3D() : FVector::OneVector;
}

FTransform UCoinSpawnerComponent::MakeCoinTransform(const FTransform& RelativeTransform, const FTransform& ParentTransform) const
{
	FTransform AdjustedRelativeTransform = RelativeTransform;
	AdjustedRelativeTransform.SetScale3D(RelativeTransform.GetScale3D() * GetCoinDefaultActorScale());
	return AdjustedRelativeTransform * ParentTransform;
}

void UCoinSpawnerComponent::SpawnCoins()
{
	if (!bEnableCoinSpawn || !CoinClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogRunGame, Warning, TEXT("CoinSpawnerComponent: SpawnCoins cannot run without a valid world."));
		return;
	}

	URunGameCoinSubsystem* CoinSubsystem = World->GetSubsystem<URunGameCoinSubsystem>();
	if (!CoinSubsystem)
	{
		UE_LOG(LogRunGame, Error, TEXT("CoinSpawnerComponent: No URunGameCoinSubsystem available"));
		return;
	}

	CacheOwnerSpline();
	DespawnCoins();

	const TArray<FTransform> Transforms = CalculateCoinTransforms();

	for (const FTransform& CoinTransform : Transforms)
	{
		ACoin* Coin = CoinSubsystem->AcquireCoin(CoinClass);
		if (Coin)
		{
			Coin->ActivateCollectible(CoinTransform);
			SpawnedCoins.Add(Coin);
		}
	}
}

void UCoinSpawnerComponent::DespawnCoins()
{
	if (SpawnedCoins.Num() == 0)
	{
		return;
	}

	UWorld* World = GetWorld();
	URunGameCoinSubsystem* CoinSubsystem = World ? World->GetSubsystem<URunGameCoinSubsystem>() : nullptr;

	for (ACoin* Coin : SpawnedCoins)
	{
		if (!Coin)
		{
			continue;
		}

		if (!Coin->IsCollected())
		{
			Coin->DeactivateCollectible();
		}

		if (CoinSubsystem)
		{
			CoinSubsystem->ReturnCoin(Coin);
		}
	}

	SpawnedCoins.Empty();
}

void UCoinSpawnerComponent::RefreshEditorPreviewCoins(bool bLogWarnings)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		if (bLogWarnings)
		{
			UE_LOG(LogRunGame, Warning, TEXT("CoinSpawnerComponent: PreviewCoins cannot run without a valid editor world."));
		}
		return;
	}

	if (World->IsGameWorld())
	{
		return;
	}

#if WITH_EDITOR
	if (!bEnableEditorPreview || !bEnableCoinSpawn || !ApplyPreviewConfig(bLogWarnings))
	{
		ClearEditorPreviewCoins();
		return;
	}

	CacheOwnerSpline();

	const TArray<FTransform> Transforms = CalculateCoinTransforms();
	if (Transforms.Num() == 0)
	{
		ClearEditorPreviewCoins();
		if (bLogWarnings)
		{
			UE_LOG(LogRunGame, Warning, TEXT("CoinSpawnerComponent: PreviewCoins found no coin transforms. Check spline points or fallback coin placement settings."));
		}
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		if (bLogWarnings)
		{
			UE_LOG(LogRunGame, Warning, TEXT("CoinSpawnerComponent: PreviewCoins needs an owner actor."));
		}
		return;
	}

	UStaticMeshComponent* MeshTemplate = GetPreviewCoinMeshTemplate();
	UStaticMesh* PreviewMesh = MeshTemplate ? MeshTemplate->GetStaticMesh() : nullptr;
	if (!PreviewMesh)
	{
		ClearEditorPreviewCoins();
		if (bLogWarnings)
		{
			UE_LOG(LogRunGame, Warning, TEXT("CoinSpawnerComponent: PreviewCoins needs a StaticMeshComponent on the coin class default object."));
		}
		return;
	}

	if (!PreviewCoinInstances)
	{
		PreviewCoinInstances = NewObject<UInstancedStaticMeshComponent>(
			Owner,
			TEXT("CoinPreviewInstances"),
			RF_Transient | RF_TextExportTransient);
		if (!PreviewCoinInstances)
		{
			return;
		}

		PreviewCoinInstances->CreationMethod = EComponentCreationMethod::Instance;
		PreviewCoinInstances->SetMobility(EComponentMobility::Movable);
		PreviewCoinInstances->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PreviewCoinInstances->SetGenerateOverlapEvents(false);
		PreviewCoinInstances->SetHiddenInGame(false);
		PreviewCoinInstances->bIsEditorOnly = true;

		if (USceneComponent* Root = Owner->GetRootComponent())
		{
			PreviewCoinInstances->SetupAttachment(Root);
		}

		Owner->AddInstanceComponent(PreviewCoinInstances);
		PreviewCoinInstances->RegisterComponentWithWorld(World);
	}

	PreviewCoinInstances->ClearInstances();
	PreviewCoinInstances->SetStaticMesh(PreviewMesh);
	for (int32 MaterialIndex = 0; MaterialIndex < MeshTemplate->GetNumMaterials(); ++MaterialIndex)
	{
		UMaterialInterface* Material = MeshTemplate->GetMaterial(MaterialIndex);
		if (!Material || Material->CheckMaterialUsage_Concurrent(MATUSAGE_InstancedStaticMeshes))
		{
			PreviewCoinInstances->SetMaterial(MaterialIndex, Material);
		}
	}

	const ACoin* CoinDefault = CoinClass ? CoinClass->GetDefaultObject<ACoin>() : nullptr;
	const bool bMeshIsRoot = CoinDefault && MeshTemplate == CoinDefault->GetRootComponent();
	const FTransform MeshRelativeTransform = bMeshIsRoot ? FTransform::Identity : MeshTemplate->GetRelativeTransform();
	for (const FTransform& CoinTransform : Transforms)
	{
		const FTransform PreviewTransform = MeshRelativeTransform * CoinTransform;
		PreviewCoinInstances->AddInstance(PreviewTransform, true);
	}

	PreviewCoinInstances->MarkRenderStateDirty();

	if (bLogWarnings)
	{
		UE_LOG(LogRunGame, Warning, TEXT("CoinSpawnerComponent: PreviewCoins created %d preview instances."), Transforms.Num());
	}
#endif
}

void UCoinSpawnerComponent::ClearEditorPreviewCoins()
{
#if WITH_EDITOR
	if (PreviewCoinInstances)
	{
		PreviewCoinInstances->ClearInstances();
		if (!IsValid(PreviewCoinInstances))
		{
			PreviewCoinInstances = nullptr;
		}
	}

	for (ACoin* Coin : SpawnedCoins)
	{
		if (IsValid(Coin))
		{
			Coin->Destroy();
		}
	}

	SpawnedCoins.Empty();
#endif
}

void UCoinSpawnerComponent::ApplyRowOffset(FVector& InOutWorldPos, const FVector& PerpendicularWorldDir, int32 RowIndex) const
{
	if (RowCount <= 1)
	{
		return;
	}

	const float CenterOffset = (RowCount - 1) * 0.5f;
	const float Offset = (static_cast<float>(RowIndex) - CenterOffset) * RowSpacing;
	InOutWorldPos += PerpendicularWorldDir * Offset;
}

TArray<FTransform> UCoinSpawnerComponent::CalculateSplineTransforms() const
{
	TArray<FTransform> OutTransforms;

	if (!CachedSpline)
	{
		return OutTransforms;
	}

	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return OutTransforms;
	}

	const float TotalLength = CachedSpline->GetSplineLength();
	if (TotalLength <= 0.0f)
	{
		return OutTransforms;
	}

	OutTransforms.Reserve(CoinCount * RowCount);

	const FTransform OwnerTransform = Owner->GetActorTransform();
	const int32 Steps = FMath::Max(CoinCount - 1, 1);
	for (int32 Row = 0; Row < RowCount; ++Row)
	{
		if (RowSpawnChance < 1.0f && FMath::FRand() >= RowSpawnChance)
		{
			continue;
		}

		for (int32 i = 0; i < CoinCount; ++i)
		{
			const float Distance = TotalLength * static_cast<float>(i) / static_cast<float>(Steps);
			const FVector SplineWorldPos = CachedSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			const FVector SplineRightWorld = CachedSpline->GetRightVectorAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			FVector OwnerLocalPos = OwnerTransform.InverseTransformPosition(SplineWorldPos);
			const FVector OwnerLocalRight = OwnerTransform.InverseTransformVectorNoScale(SplineRightWorld).GetSafeNormal();
			ApplyRowOffset(OwnerLocalPos, OwnerLocalRight, Row);

			const FTransform OwnerLocalTransform(FRotator::ZeroRotator, OwnerLocalPos, FVector::OneVector);
			OutTransforms.Add(MakeCoinTransform(OwnerLocalTransform, OwnerTransform));
		}
	}

	return OutTransforms;
}

TArray<FTransform> UCoinSpawnerComponent::CalculateCoinTransforms() const
{
	if (CachedSpline && CachedSpline->GetNumberOfSplinePoints() > 0)
	{
		return CalculateSplineTransforms();
	}

	// Enum fallback
	TArray<FTransform> OutTransforms;
	switch (SpawnPattern)
	{
	case ECoinSpawnPattern::StraightLine:
		CalculateStraightLine(OutTransforms);
		break;
	case ECoinSpawnPattern::DiagonalLine:
		CalculateDiagonalLine(OutTransforms);
		break;
	case ECoinSpawnPattern::Parabolic:
		CalculateParabolic(OutTransforms);
		break;
	default:
		break;
	}

	return OutTransforms;
}

void UCoinSpawnerComponent::CalculateStraightLine(TArray<FTransform>& OutTransforms) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FTransform OwnerTransform = Owner->GetActorTransform();
	const FVector LocalDir = LineDirection.GetSafeNormal();

	// Perpendicular direction for multi-row offset
	const FVector PerpendicularLocal = FVector::CrossProduct(LocalDir, FVector::UpVector).GetSafeNormal();

	OutTransforms.Reserve(CoinCount * RowCount);

	for (int32 Row = 0; Row < RowCount; ++Row)
	{
		if (RowSpawnChance < 1.0f && FMath::FRand() >= RowSpawnChance)
		{
			continue;
		}

		for (int32 i = 0; i < CoinCount; ++i)
		{
			FVector LocalPos = StartOffset + LineDirection * static_cast<float>(i) * CoinSpacing;
			ApplyRowOffset(LocalPos, PerpendicularLocal, Row);
			const FTransform LocalTransform(FRotator::ZeroRotator, LocalPos, FVector::OneVector);
			OutTransforms.Add(MakeCoinTransform(LocalTransform, OwnerTransform));
		}
	}
}

void UCoinSpawnerComponent::CalculateDiagonalLine(TArray<FTransform>& OutTransforms) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FTransform OwnerTransform = Owner->GetActorTransform();
	const FVector LocalDir = LineDirection.GetSafeNormal();

	const FVector PerpendicularLocal = FVector::CrossProduct(LocalDir, FVector::UpVector).GetSafeNormal();

	OutTransforms.Reserve(CoinCount * RowCount);

	for (int32 Row = 0; Row < RowCount; ++Row)
	{
		if (RowSpawnChance < 1.0f && FMath::FRand() >= RowSpawnChance)
		{
			continue;
		}

		for (int32 i = 0; i < CoinCount; ++i)
		{
			FVector LocalPos = StartOffset + LineDirection * static_cast<float>(i) * CoinSpacing;
			ApplyRowOffset(LocalPos, PerpendicularLocal, Row);
			const FTransform LocalTransform(FRotator::ZeroRotator, LocalPos, FVector::OneVector);
			OutTransforms.Add(MakeCoinTransform(LocalTransform, OwnerTransform));
		}
	}
}

void UCoinSpawnerComponent::CalculateParabolic(TArray<FTransform>& OutTransforms) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FTransform OwnerTransform = Owner->GetActorTransform();
	OutTransforms.Reserve(CoinCount * RowCount);

	const FVector P0 = StartOffset;
	const FVector P2 = EndOffset;
	const FVector P1 = (P0 + P2) * 0.5f + FVector(0.0f, 0.0f, ParabolaHeight);

	// Perpendicular to the chord direction (P0→P2)
	const FVector ChordDir = (P2 - P0).GetSafeNormal();
	const FVector PerpendicularLocal = FVector::CrossProduct(ChordDir, FVector::UpVector).GetSafeNormal();

	const int32 Steps = FMath::Max(CoinCount - 1, 1);
	for (int32 Row = 0; Row < RowCount; ++Row)
	{
		if (RowSpawnChance < 1.0f && FMath::FRand() >= RowSpawnChance)
		{
			continue;
		}

		for (int32 i = 0; i < CoinCount; ++i)
		{
			const float t = static_cast<float>(i) / static_cast<float>(Steps);
			const float OneMinusT = 1.0f - t;
			FVector LocalPos = OneMinusT * OneMinusT * P0 + 2.0f * OneMinusT * t * P1 + t * t * P2;
			ApplyRowOffset(LocalPos, PerpendicularLocal, Row);
			const FTransform LocalTransform(FRotator::ZeroRotator, LocalPos, FVector::OneVector);
			OutTransforms.Add(MakeCoinTransform(LocalTransform, OwnerTransform));
		}
	}
}


