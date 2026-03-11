#include "CarreroRenderSVE.h"
#include "RenderPass/DStretchPostProcessPass.h"
#include "SceneView.h"
#include "SceneViewExtension.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "ScreenPass.h"
#include "PostProcess/PostProcessMaterialInputs.h"

FCarreroRenderSVE::FCarreroRenderSVE(const FAutoRegister& AutoRegister)
	: FSceneViewExtensionBase(AutoRegister)
{
}

bool FCarreroRenderSVE::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const {
	// always on
	return true;
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
}

void FCarreroRenderSVE::PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)
{
}

void FCarreroRenderSVE::PostRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)
{
}

void FCarreroRenderSVE::SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& InView, FPostProcessingPassDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
{
	switch (PostProcessEffect_RT) {
		case ECarreroPostProcessEffect::DStretch:
			// BeforeDOF - Linear (HDR)
			if (Pass == EPostProcessingPass::BeforeDOF)
			{
				InOutPassCallbacks.Add(
					FPostProcessingPassDelegate::CreateRaw(
					this, &FCarreroRenderSVE::AddDStretchPass_RenderThread)
				);
			}
			break;
		case ECarreroPostProcessEffect::SSAO:
			break;
		case ECarreroPostProcessEffect::None:
			break;
		default:;
	}
}

FScreenPassTexture FCarreroRenderSVE::AddDStretchPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessMaterialInputs& Inputs)
{
	FScreenPassTextureSlice SceneColor = Inputs.GetInput(EPostProcessMaterialInput::SceneColor);
	if (!SceneColor.IsValid()) {
		return FScreenPassTexture{};
	}

	FRDGTextureRef SceneColorTexture = SceneColor.TextureSRV->Desc.Texture;

	// If the engine has set an OverrideOutput (i.e. this is the last pass), you HAVE to render into it. 
	// Ignoring it causes the output to be discarded in-game and not submitted to swapchain.
	FScreenPassRenderTarget Output;
	if (Inputs.OverrideOutput.IsValid()) {
		Output = Inputs.OverrideOutput;
	}
	else
	{
		FRDGTextureDesc OutputDesc = FRDGTextureDesc::Create2D(
			SceneColorTexture->Desc.Extent,
			SceneColorTexture->Desc.Format,
			FClearValueBinding::Black,
			TexCreate_RenderTargetable | TexCreate_ShaderResource);
		Output = FScreenPassRenderTarget(
			GraphBuilder.CreateTexture(OutputDesc, TEXT("DStretch.Output")),
			SceneColor.ViewRect,
			ERenderTargetLoadAction::ENoAction);
	}

	const FScreenPassTextureViewport Viewport(SceneColorTexture, SceneColor.ViewRect);

	TShaderMapRef<FDStretchPostProcessPassPS> PixelShader(GetGlobalShaderMap(InView.GetFeatureLevel()));

	FDStretchPostProcessPassParameters* PassParameters = GraphBuilder.AllocParameters<FDStretchPostProcessPassParameters>();
	PassParameters->SceneColorTexture = SceneColorTexture;
	PassParameters->SceneColorSampler = TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
	PassParameters->RenderTargets[0] = Output.GetRenderTargetBinding();

	AddDrawScreenPass(
		GraphBuilder,
		RDG_EVENT_NAME("DStretchPostProcessPass"),
		InView,
		FScreenPassTextureViewport(Output.Texture, Output.ViewRect),
		Viewport,
		PixelShader,
		PassParameters
	);

	return MoveTemp(Output);
}
