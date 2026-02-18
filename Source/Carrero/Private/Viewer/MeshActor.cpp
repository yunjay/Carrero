#include "Viewer/MeshActor.h"

#include "MeshImport.h"
#include "RealtimeMeshComponent.h"
#include "RealtimeMeshSimple.h"
#include "Materials/MaterialInterface.h"

using namespace RealtimeMesh;

AMeshActor::AMeshActor()
	: MeshBounds(ForceInit)
{
	PrimaryActorTick.bCanEverTick = false;

	RealtimeMeshComponent = CreateDefaultSubobject<URealtimeMeshComponent>(TEXT("RealtimeMeshComponent"));
	RealtimeMeshComponent->SetMobility(EComponentMobility::Movable);
	SetRootComponent(RealtimeMeshComponent);
}

bool AMeshActor::LoadMeshFromFile(const FString& FilePath)
{
	FMeshImport ImportService;
	FImportedMeshData MeshData;
	if (!ImportService.LoadMesh(FilePath, MeshData))
	{
		return false;
	}

	// Process mesh before creating RealtimeMeshComponent
	ImportService.ProcessMesh(MeshData);

	URealtimeMeshSimple* RealtimeMesh = RealtimeMeshComponent->InitializeRealtimeMesh<URealtimeMeshSimple>();

	// Hardcoded to opaque two sided material for now
	RealtimeMesh->SetupMaterialSlot(0, TEXT("CarreroDefaultOpaqueMaterial"));

	UMaterialInterface* DefaultMaterial = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/CarreroDefaultOpaqueMaterial.CarreroDefaultOpaqueMaterial")));
	if (DefaultMaterial)
	{
		RealtimeMeshComponent->SetMaterial(0, DefaultMaterial);
	}

	FRealtimeMeshStreamSet StreamSet;
	TRealtimeMeshBuilderLocal<uint32, FPackedNormal, FVector2DHalf, 1> Builder(StreamSet);
	Builder.EnableTangents();
	Builder.EnableTexCoords();
	Builder.EnableColors();
	Builder.EnablePolyGroups();

	for (int32 VertexIndex = 0; VertexIndex < MeshData.Positions.Num(); ++VertexIndex)
	{
		const FVector3f Position = MeshData.Positions[VertexIndex];
		const FVector3f Normal = MeshData.Normals.IsValidIndex(VertexIndex) ? MeshData.Normals[VertexIndex] : FVector3f::UpVector;
		const FVector3f Tangent = MeshData.Tangents.IsValidIndex(VertexIndex) ? MeshData.Tangents[VertexIndex] : FVector3f::CrossProduct(Normal, FVector3f::RightVector);
		const FVector2f UV = MeshData.UVs.IsValidIndex(VertexIndex) ? MeshData.UVs[VertexIndex] : FVector2f::ZeroVector;
		const FColor Color = MeshData.Colors.IsValidIndex(VertexIndex) ? MeshData.Colors[VertexIndex] : FColor::White;

		Builder.AddVertex(Position)
			.SetNormalAndTangent(Normal, Tangent)
			.SetTexCoord(UV)
			.SetColor(Color);
	}

	for (int32 Index = 0; Index + 2 < MeshData.Indices.Num(); Index += 3)
	{
		Builder.AddTriangle(MeshData.Indices[Index], MeshData.Indices[Index + 1], MeshData.Indices[Index + 2], 0);
	}

	const FRealtimeMeshSectionGroupKey GroupKey = FRealtimeMeshSectionGroupKey::Create(0, FName("ImportedMesh"));
	RealtimeMesh->CreateSectionGroup(GroupKey, StreamSet);
	RealtimeMesh->UpdateSectionConfig(FRealtimeMeshSectionKey::CreateForPolyGroup(GroupKey, 0), FRealtimeMeshSectionConfig(0));

	MeshBounds = FBoxSphereBounds(MeshData.Bounds);
	SetActorLocation(MeshBounds.Origin);
	return true;
}

FBoxSphereBounds AMeshActor::GetMeshBounds() const
{
	return MeshBounds;
}
