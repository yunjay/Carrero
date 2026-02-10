#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnMeshPathSelected, const FString&);

class SObjectViewerHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SObjectViewerHUDWidget) {}
		SLATE_EVENT(FOnMeshPathSelected, OnMeshPathSelected)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FReply OnOpenMeshClicked();
	FText GetSelectedFileText() const;

	FOnMeshPathSelected OnMeshPathSelected;
	FString SelectedFilePath;
};
