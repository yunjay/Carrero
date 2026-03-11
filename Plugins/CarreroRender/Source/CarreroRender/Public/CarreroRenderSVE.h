#pragma once
#include "SceneViewExtension.h"
// Scene View Extension for CarreroRender

enum class ECarreroRenderMode : uint8 {
	Default     UMETA(DisplayName = "Default"), 
	XShade		UMETA(DisplayName = "XShade") // Exaggerated Shading
	// ...
};
enum class ECarreroPostProcessEffect : uint8 {
	None        UMETA(DisplayName = "None"),
	DStretch    UMETA(DisplayName = "DStretch"),
	SSAO		UMETA(DisplayName = "SSAO")
	// ...
};
class CARRERORENDER_API FCarreroRenderSVE : public FSceneViewExtensionBase
{
public:
	FCarreroRenderSVE(const FAutoRegister& AutoRegister);

#pragma region ISceneViewExtension interface
	// ISceneViewExtension interface	
	// Game Thread
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;

	// Render Thread
	virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;
	virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override;
	virtual void PostRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override;
	virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& InView, FPostProcessingPassDelegateArray& InOutPassCallbacks, bool bIsPassEnabled) override;
#pragma endregion

protected:
	// Interface for controlling enabled status// This can be safely read/written from any thread
	virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;

private:
	ECarreroRenderMode RenderMode_RT = ECarreroRenderMode::Default;
	ECarreroPostProcessEffect PostProcessEffect_RT = ECarreroPostProcessEffect::None;

	// I could add data to FSceneView if it makes sense, but I'll just keep everything in-project for easier git management.

	FScreenPassTexture AddDStretchPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessMaterialInputs& Inputs);
};
