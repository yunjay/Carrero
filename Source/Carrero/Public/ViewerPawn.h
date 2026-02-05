#pragma once

/*
* Camera attached to SpringArmComponent
*/

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ViewerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class CARRERO_API AViewerPawn : public APawn
{
	GENERATED_BODY()

public:
	AViewerPawn();

	void Orbit(const FVector2D& Delta);
	void Pan(const FVector2D& Delta);
	void Zoom(float Delta);
	void SetOrbitTarget(const FVector& NewTarget);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	FVector OrbitTarget;
	float OrbitYaw;
	float OrbitPitch;
	float OrbitSpeed;
	float PanSpeed;
	float ZoomSpeed;
	float MinZoom;
	float MaxZoom;
};
