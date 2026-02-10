#include "ViewerPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AViewerPawn::AViewerPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Root);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->TargetArmLength = 300.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	OrbitTarget = FVector::ZeroVector;
	OrbitYaw = 0.0f;
	OrbitPitch = -20.0f;
	OrbitSpeed = 0.25f;
	PanSpeed = 0.75f;
	ZoomSpeed = 25.0f;
	MinZoom = 5.0f;
	MaxZoom = 2000.0f;
}

void AViewerPawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(OrbitTarget);
	SpringArm->SetRelativeRotation(FRotator(OrbitPitch, OrbitYaw, 0.0f));
}

void AViewerPawn::Orbit(const FVector2D& Delta)
{
	OrbitYaw += Delta.X * OrbitSpeed;
	OrbitPitch = FMath::Clamp(OrbitPitch + Delta.Y * OrbitSpeed, -85.0f, 85.0f);
	SpringArm->SetRelativeRotation(FRotator(OrbitPitch, OrbitYaw, 0.0f));
}

void AViewerPawn::Pan(const FVector2D& Delta)
{
	const FVector Right = Camera->GetRightVector();
	const FVector Up = Camera->GetUpVector();
	OrbitTarget += (-Right * Delta.X + Up * Delta.Y) * PanSpeed;
	SetActorLocation(OrbitTarget);
}

void AViewerPawn::Zoom(float Delta)
{
	// SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + Delta * ZoomSpeed, MinZoom, MaxZoom);
	const float Distance = SpringArm->TargetArmLength;
	const float DistanceScale = FMath::Max(0.1f, Distance / 300.0f);
	SpringArm->TargetArmLength = FMath::Clamp(Distance + Delta * ZoomSpeed * DistanceScale, MinZoom, MaxZoom);
}

void AViewerPawn::SetOrbitTarget(const FVector& NewTarget)
{
	OrbitTarget = NewTarget;
	SetActorLocation(OrbitTarget);
}
