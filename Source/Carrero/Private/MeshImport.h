#pragma once
/*
* Import and process mesh data.
*/
#include "CoreMinimal.h"

struct FImportedMeshData
{
	TArray<FVector3f> Positions;
	TArray<FVector3f> Normals;
	TArray<FVector3f> Tangents;
	TArray<FVector2f> UVs;
	TArray<int32> Indices;
	FBox Bounds = FBox(ForceInit);
};

class FMeshImport
{
public:
	bool LoadMesh(const FString& FilePath, FImportedMeshData& OutMeshData) const;
	void ProcessMesh(FImportedMeshData& MeshData) const;
};
