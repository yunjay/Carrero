#include "SObjectViewerHUDWidget.h"

#include "Framework/Application/SlateApplication.h"
#include "GenericPlatform/GenericWindow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SWindow.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <wrl/client.h>
#include <shobjidl.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#define LOCTEXT_NAMESPACE "SObjectViewerHUDWidget"

void SObjectViewerHUDWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBox)
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		[
			SNew(SBorder)
			.Padding(8.0f)
			[
				SNew(SVerticalBox)

				// Title
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CarreroTitle", "Carrero Visualizer"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 6.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(this, &SObjectViewerHUDWidget::GetSelectedFileText)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 6.0f, 0.0f, 0.0f)
				[
					SNew(SButton)
					.Text(LOCTEXT("OpenMeshButton", "Open Mesh"))
					.OnClicked(this, &SObjectViewerHUDWidget::OnOpenMeshClicked)
				]
			]
		]
	];
}

FReply SObjectViewerHUDWidget::OnOpenMeshClicked()
{
	// OS native loading as DesktopPlatform can't be used in packaged executable
#if PLATFORM_WINDOWS
	HRESULT CoInitResult = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	Microsoft::WRL::ComPtr<IFileOpenDialog> FileDialog;
	HRESULT CreateResult = ::CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&FileDialog));
	if (FAILED(CreateResult))
	{
		if (SUCCEEDED(CoInitResult))
		{
			::CoUninitialize();
		}
		return FReply::Handled();
	}

	DWORD Options = 0;
	if (SUCCEEDED(FileDialog->GetOptions(&Options)))
	{
		FileDialog->SetOptions(Options | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST);
	}
	const COMDLG_FILTERSPEC FileTypes[] =
	{
		{
			L"Mesh Files",
			L"*.fbx;*.obj;*.dae;*.gltf;*.glb;*.blend;*.3ds;*.ase;*.ifc;*.x;*.dxf;*.ply;*.stl;*.lwo;*.lws;*.lxo;*.ac;*.ms3d;*.cob;*.scn;*.bvh;*.csm;*.xml;*.irrmesh;*.irr;*.mdl;*.md2;*.md3;*.pk3;*.mdc;*.md5*;*.smd;*.vta;*.ogex;*.3d;*.b3d;*.q3d;*.q3s;*.nff;*.off;*.raw;*.ter;*.hmp;*.ndo"
		},
		{ L"All Files (*.*)", L"*.*" }
	};
	FileDialog->SetFileTypes(static_cast<UINT>(UE_ARRAY_COUNT(FileTypes)), FileTypes);
	FileDialog->SetTitle(L"Open Mesh");

	HWND ParentHandle = nullptr;
	TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindBestParentWindowForDialogs(nullptr);
	if (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid())
	{
		ParentHandle = static_cast<HWND>(ParentWindow->GetNativeWindow()->GetOSWindowHandle());
	}

	const HRESULT ShowResult = FileDialog->Show(ParentHandle);
	if (SUCCEEDED(ShowResult))
	{
		Microsoft::WRL::ComPtr<IShellItem> SelectedItem;
		if (SUCCEEDED(FileDialog->GetResult(&SelectedItem)) && SelectedItem)
		{
			PWSTR SelectedPath = nullptr;
			if (SUCCEEDED(SelectedItem->GetDisplayName(SIGDN_FILESYSPATH, &SelectedPath)) && SelectedPath)
			{
				SelectedFilePath = SelectedPath;
				::CoTaskMemFree(SelectedPath);
			}
		}
	}

	if (SUCCEEDED(CoInitResult))
	{
		::CoUninitialize();
	}
#endif

	return FReply::Handled();
}

FText SObjectViewerHUDWidget::GetSelectedFileText() const
{
	if (SelectedFilePath.IsEmpty())
	{
		return LOCTEXT("NoFileSelected", "No mesh selected");
	}

	return FText::FromString(SelectedFilePath);
}

#undef LOCTEXT_NAMESPACE
