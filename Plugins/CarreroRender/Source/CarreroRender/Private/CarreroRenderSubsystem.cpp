#include "CarreroRenderSubsystem.h"
#include "SceneViewExtension.h"

void UCarreroRenderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RenderSVE = FSceneViewExtensions::NewExtension<FCarreroRenderSVE>();
}

void UCarreroRenderSubsystem::Deinitialize()
{
	RenderSVE.Reset();

	Super::Deinitialize();
}
