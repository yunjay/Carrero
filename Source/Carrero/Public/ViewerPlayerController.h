#pragma once
/*
* Controller coupled with ViewerPawn
*/
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ViewerPlayerController.generated.h"

UCLASS()
class CARRERO_API AViewerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AViewerPlayerController();
	void HandleMeshPathSelected(const FString& Path);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	void BeginOrbit();
	void EndOrbit();
	void BeginPan();
	void EndPan();
	void HandleMouseWheel(float Value);
	void ApplyInputMode(bool bIsDragging);

	TWeakObjectPtr<class AMeshActor> MeshActor;
	bool bIsOrbiting;
	bool bIsPanning;
};
