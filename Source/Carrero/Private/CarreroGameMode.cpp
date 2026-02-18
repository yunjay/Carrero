#include "CarreroGameMode.h"
#include "UI/AssetViewerHUD.h"
#include "Viewer/ViewerPawn.h"
#include "Viewer/ViewerPlayerController.h"

ACarreroGameMode::ACarreroGameMode()
{
	// Set HUD to my slate ui
	HUDClass = AAssetViewerHUD::StaticClass();
	DefaultPawnClass = AViewerPawn::StaticClass();
	PlayerControllerClass = AViewerPlayerController::StaticClass();
}
