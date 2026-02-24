#pragma once

#include "Modules/ModuleManager.h"

class FCarreroRenderModule : public IModuleInterface
{
public:

	// IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
