#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SObjectViewerHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SObjectViewerHUDWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnOpenMeshClicked();
	FText GetSelectedFileText() const;

	FString SelectedFilePath;
};
