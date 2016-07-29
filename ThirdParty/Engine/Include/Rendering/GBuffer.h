#pragma once

#include <Vector.h>

namespace Magma
{
	class Effect;
	class GPUContext;
	class Texture;

	class GBuffer
	{
	public:
		GBuffer(GPUContext& aGPUContext, const CU::Vector2<float>& aWindowSize);
		~GBuffer();

		void Set(Effect* aEffect);
		void UnSet(Effect* aEffect);
		void Clear(const CU::Vector4<float>& aColor);

		void Resize(float aWidth, float aHeight);

		Texture* myAlbedoAndMetalness;
		Texture* myNormalAndRoughness;
		Texture* myDepth;

		Texture* myDepthStencil;

		GPUContext& myGPUContext;

	private:
		void operator=(GBuffer&) = delete;
	};
}
