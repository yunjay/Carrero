#include "CarreroRender.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FCarreroRenderModule"

void FCarreroRenderModule::StartupModule()
{
	// This will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	// Add shaders directory
	auto thisPlugin = IPluginManager::Get().FindPlugin(TEXT("CarreroRender"));
	auto thisShadersDir = FPaths::Combine(thisPlugin->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Shaders"), thisShadersDir);
}

void FCarreroRenderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module. For modules that support dynamic reloading, we call this function before unloading the module.

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCarreroRenderModule, CarreroRender)