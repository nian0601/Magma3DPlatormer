#pragma once

#include <EngineTypes.h>

namespace Magma
{
	class Renderer;
	class RendererProxy
	{
	public:
		RendererProxy(Renderer& aRenderer);
		~RendererProxy();

		void RenderModel(ModelID aModelID, EffectID aEffectID
			, const CU::Matrix44<float>& aOrientation, const CU::Vector3<float>& aScale = CU::Vector3<float>());
	private:
		void operator=(RendererProxy&) = delete;

		Renderer& myRenderer;
	};
}