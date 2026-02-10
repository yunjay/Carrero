#pragma once

/*
* Actor that holds a realtime mesh component.
*/
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshActor.generated.h"

class UMaterialInterface;
class URealtimeMeshComponent;

UCLASS()
class CARRERO_API AMeshActor : public AActor
{
	GENERATED_BODY()

public:
	AMeshActor();
	
	// 
	bool LoadMeshFromFile(const FString& FilePath);
	
	FBoxSphereBounds GetMeshBounds() const;

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UMaterialInterface* MaterialBase;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	bool bForceTwoSided;

	UPROPERTY(VisibleAnywhere)
	URealtimeMeshComponent* RealtimeMeshComponent;

	FBoxSphereBounds MeshBounds;
};
