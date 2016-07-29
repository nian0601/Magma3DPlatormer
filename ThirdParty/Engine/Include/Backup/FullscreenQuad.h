#pragma once
#include "EngineTypes.h"
#include "GfxStructs.h"
#include <GrowingArray.h>
#include "GPUData.h"

namespace Magma
{
	class Effect;
	class GPUContext;

	class FullscreenQuad
	{
	public:
		FullscreenQuad();
		~FullscreenQuad();

		void InitFullscreenQuad(EffectID aEffect, GPUContext& aGPUContext, AssetContainer& aAssetContainer);
		void ActivateFullscreenQuad(GPUContext& aGPUContext);
		void RenderFullscreenQuad(EffectID aEffect, const CU::String<30>& aTechnique, GPUContext& aGPUContext, AssetContainer& aAssetContainer);

	private:
		GPUData myGPUData;
		
	};
}