#include "UI/AssetViewerHUD.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "UI/SObjectViewerHUDWidget.h"
#include "Viewer/ViewerPlayerController.h"
#include "Widgets/SWeakWidget.h"

void AAssetViewerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->GameViewport)
	{
		// Create and add the HUD widget to the viewport
		SAssignNew(HudWidget, SObjectViewerHUDWidget)
			.OnMeshPathSelected(FOnMeshPathSelected::CreateUObject(this, &AAssetViewerHUD::HandleMeshPathSelected));
		HudWidgetContainer = SNew(SWeakWidget).PossiblyNullContent(HudWidget.ToSharedRef());
		GEngine->GameViewport->AddViewportWidgetContent(HudWidgetContainer.ToSharedRef(), 0);
	}
}

void AAssetViewerHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GEngine && GEngine->GameViewport && HudWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(HudWidgetContainer.ToSharedRef());
	}

	HudWidget.Reset();
	HudWidgetContainer.Reset();

	Super::EndPlay(EndPlayReason);
}

void AAssetViewerHUD::HandleMeshPathSelected(const FString& Path)
{
	if (AViewerPlayerController* ViewerController = Cast<AViewerPlayerController>(GetOwningPlayerController()))
	{
		ViewerController->HandleMeshPathSelected(Path);
	}
}
