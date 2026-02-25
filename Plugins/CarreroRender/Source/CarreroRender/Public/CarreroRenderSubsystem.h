#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "CarreroRenderSVE.h"
#include "CarreroRenderSubsystem.generated.h"

/**
 * Engine subsystem that owns the CarreroRender Scene View Extension.
 * Access via GEngine->GetEngineSubsystem<UCarreroRenderSubsystem>().
 */
UCLASS()
class CARRERORENDER_API UCarreroRenderSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	TSharedPtr<FCarreroRenderSVE, ESPMode::ThreadSafe> GetSceneViewExtension() const { return RenderSVE; }

private:
	TSharedPtr<FCarreroRenderSVE, ESPMode::ThreadSafe> RenderSVE;
};
