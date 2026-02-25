#include "CarreroRenderSVE.h"
#include "SceneView.h"
#include "SceneViewExtension.h"
#include "RenderGraphBuilder.h"
#include "RenderCommandFence.h"

FCarreroRenderSVE::FCarreroRenderSVE(const FAutoRegister& AutoRegister)
	: FSceneViewExtensionBase(AutoRegister)
{
}

void FCarreroRenderSVE::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FCarreroRenderSVE::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
}

void FCarreroRenderSVE::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FCarreroRenderSVE::PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{
	// ENQUEUE_RENDER_COMMAND to enqueue a render command from the render thread context.
}

void FCarreroRenderSVE::PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)
{
}

void FCarreroRenderSVE::PostRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)
{
}
