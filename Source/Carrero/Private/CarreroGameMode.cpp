#include "CarreroGameMode.h"
#include "AssetViewerHUD.h"
#include "ViewerPawn.h"
#include "ViewerPlayerController.h"

ACarreroGameMode::ACarreroGameMode()
{
	// Set HUD to my slate ui
	HUDClass = AAssetViewerHUD::StaticClass();
	DefaultPawnClass = AViewerPawn::StaticClass();
	PlayerControllerClass = AViewerPlayerController::StaticClass();
}
