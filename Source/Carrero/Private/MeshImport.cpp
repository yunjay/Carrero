#include "MeshImport.h"

#pragma push_macro("check")
#undef check // UE5 uses check
THIRD_PARTY_INCLUDES_START
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <CGAL/Simple_cartesian.h>
THIRD_PARTY_INCLUDES_END
#pragma pop_macro("check")

bool FMeshImport::LoadMesh(const FString& FilePath, FImportedMeshData& OutMeshData) const
{
	Assimp::Importer Importer;
	const unsigned int Flags = aiProcess_Triangulate 
		| aiProcess_MakeLeftHanded // UE5
		| aiProcess_GenNormals 
		| aiProcess_CalcTangentSpace
		| aiProcess_JoinIdenticalVertices 
		| aiProcess_FixInfacingNormals // Necessary for the scanned .ply's but CGAL might be better.
		| aiProcess_ImproveCacheLocality;

	const aiScene* Scene = Importer.ReadFile(TCHAR_TO_UTF8(*FilePath), Flags);
	if (!Scene || Scene->mNumMeshes == 0)
	{
		return false;
	}

	const aiMesh* Mesh = Scene->mMeshes[0];
	OutMeshData.Positions.Reserve(Mesh->mNumVertices);
	OutMeshData.Normals.Reserve(Mesh->mNumVertices);
	OutMeshData.Tangents.Reserve(Mesh->mNumVertices);
	OutMeshData.UVs.Reserve(Mesh->mNumVertices);
	OutMeshData.Colors.Reserve(Mesh->mNumVertices);

	const bool bHasColors = Mesh->HasVertexColors(0);

	for (unsigned int VertexIndex = 0; VertexIndex < Mesh->mNumVertices; ++VertexIndex)
	{
		const aiVector3D& Position = Mesh->mVertices[VertexIndex];
		OutMeshData.Positions.Add(FVector3f(Position.x, Position.y, Position.z));

		if (Mesh->HasNormals())
		{
			const aiVector3D& Normal = Mesh->mNormals[VertexIndex];
			OutMeshData.Normals.Add(FVector3f(Normal.x, Normal.y, Normal.z));
		}
		else
		{
			OutMeshData.Normals.Add(FVector3f::UpVector);
		}

		if (Mesh->HasTangentsAndBitangents())
		{
			const aiVector3D& Tangent = Mesh->mTangents[VertexIndex];
			OutMeshData.Tangents.Add(FVector3f(Tangent.x, Tangent.y, Tangent.z));
		}
		else
		{
			OutMeshData.Tangents.Add(FVector3f::RightVector);
		}

		if (Mesh->HasTextureCoords(0))
		{
			const aiVector3D& UV = Mesh->mTextureCoords[0][VertexIndex];
			OutMeshData.UVs.Add(FVector2f(UV.x, UV.y));
		}
		else
		{
			OutMeshData.UVs.Add(FVector2f::ZeroVector);
		}

		if (bHasColors)
		{
			const aiColor4D& Color = Mesh->mColors[0][VertexIndex];
			OutMeshData.Colors.Add(FColor(
				static_cast<uint8>(FMath::Clamp(Color.r, 0.0f, 1.0f) * 255.0f),
				static_cast<uint8>(FMath::Clamp(Color.g, 0.0f, 1.0f) * 255.0f),
				static_cast<uint8>(FMath::Clamp(Color.b, 0.0f, 1.0f) * 255.0f),
				static_cast<uint8>(FMath::Clamp(Color.a, 0.0f, 1.0f) * 255.0f)));
		}
		else
		{
			OutMeshData.Colors.Add(FColor::White);
		}

		OutMeshData.Bounds += FVector(Position.x, Position.y, Position.z);
	}

	OutMeshData.Indices.Reset();
	OutMeshData.Indices.Reserve(Mesh->mNumFaces * 3);
	for (unsigned int FaceIndex = 0; FaceIndex < Mesh->mNumFaces; ++FaceIndex)
	{
		const aiFace& Face = Mesh->mFaces[FaceIndex];
		if (Face.mNumIndices == 3)
		{
			OutMeshData.Indices.Add(static_cast<int32>(Face.mIndices[0]));
			OutMeshData.Indices.Add(static_cast<int32>(Face.mIndices[1]));
			OutMeshData.Indices.Add(static_cast<int32>(Face.mIndices[2]));
		}
	}

	return OutMeshData.Positions.Num() > 0 && OutMeshData.Indices.Num() > 0;
}

void FMeshImport::ProcessMesh(FImportedMeshData& MeshData) const
{
	using Kernel = CGAL::Simple_cartesian<double>;
	using Point = Kernel::Point_3;

	TArray<Point> Points;
	Points.Reserve(MeshData.Positions.Num());
	for (const FVector3f& Position : MeshData.Positions)
	{
		Points.Emplace(Position.X, Position.Y, Position.Z);
	}

	(void)Points;
}
