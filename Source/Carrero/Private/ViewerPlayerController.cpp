#include "ViewerPlayerController.h"

#include "Framework/Application/SlateApplication.h"
#include "ViewerPawn.h"

AViewerPlayerController::AViewerPlayerController()
	: bIsOrbiting(false)
	, bIsPanning(false)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AViewerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ApplyInputMode(false);
}

void AViewerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &AViewerPlayerController::BeginOrbit);
	InputComponent->BindKey(EKeys::RightMouseButton, IE_Released, this, &AViewerPlayerController::EndOrbit);
	InputComponent->BindKey(EKeys::MiddleMouseButton, IE_Pressed, this, &AViewerPlayerController::BeginPan);
	InputComponent->BindKey(EKeys::MiddleMouseButton, IE_Released, this, &AViewerPlayerController::EndPan);
	InputComponent->BindAxisKey(EKeys::MouseWheelAxis, this, &AViewerPlayerController::HandleMouseWheel);
}

void AViewerPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	float DeltaX = 0.0f;
	float DeltaY = 0.0f;
	GetInputMouseDelta(DeltaX, DeltaY);

	AViewerPawn* ViewerPawn = Cast<AViewerPawn>(GetPawn());
	if (!ViewerPawn)
	{
		return;
	}

	if (bIsOrbiting)
	{
		ViewerPawn->Orbit(FVector2D(DeltaX, DeltaY));
	}
	else if (bIsPanning)
	{
		ViewerPawn->Pan(FVector2D(DeltaX, DeltaY));
	}
}

void AViewerPlayerController::BeginOrbit()
{
	bIsOrbiting = true;
	ApplyInputMode(true);
}

void AViewerPlayerController::EndOrbit()
{
	bIsOrbiting = false;
	ApplyInputMode(bIsPanning);
}

void AViewerPlayerController::BeginPan()
{
	bIsPanning = true;
	ApplyInputMode(true);
}

void AViewerPlayerController::EndPan()
{
	bIsPanning = false;
	ApplyInputMode(bIsOrbiting);
}

void AViewerPlayerController::HandleMouseWheel(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	AViewerPawn* ViewerPawn = Cast<AViewerPawn>(GetPawn());
	if (ViewerPawn)
	{
		ViewerPawn->Zoom(-Value);
	}
}

void AViewerPlayerController::ApplyInputMode(bool bIsDragging)
{
	if (bIsDragging)
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
	}
	else
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}
