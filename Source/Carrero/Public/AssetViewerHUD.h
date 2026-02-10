#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AssetViewerHUD.generated.h"

class SObjectViewerHUDWidget;

UCLASS()
class CARRERO_API AAssetViewerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void HandleMeshPathSelected(const FString& Path);

	TSharedPtr<SObjectViewerHUDWidget> HudWidget;
	TSharedPtr<SWidget> HudWidgetContainer;
};
