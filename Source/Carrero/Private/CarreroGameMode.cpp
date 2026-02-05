#include "CarreroGameMode.h"
#include "AssetViewerHUD.h"

ACarreroGameMode::ACarreroGameMode()
{
	// Set HUD to my slate ui
	HUDClass = AAssetViewerHUD::StaticClass();
}
