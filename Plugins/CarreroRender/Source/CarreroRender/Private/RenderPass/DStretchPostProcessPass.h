#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "ScreenPass.h"

BEGIN_SHADER_PARAMETER_STRUCT(FDStretchPostProcessPassParameters, CARRERORENDER_API)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTexture)
	SHADER_PARAMETER_SAMPLER(SamplerState, SceneColorSampler)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

/// SRV
// SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FSomeData>, SomeBuffer)
// UAV
// SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FSomeData>, SomeBuffer)

class FDStretchPostProcessPassPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FDStretchPostProcessPassPS);
	SHADER_USE_PARAMETER_STRUCT(FDStretchPostProcessPassPS, FGlobalShader);

	using FParameters = FDStretchPostProcessPassParameters;

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}
};
