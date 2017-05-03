#pragma once

namespace Magma
{
	class AssetContainer;
	class Camera;
	class Effect;
	class GBuffer;
	class GPUContext;
	class Texture;
	class Renderer;

	class DeferredRenderer
	{
	public:
		DeferredRenderer(AssetContainer& aAssetContainer, GPUContext& aGPUContext,
			Renderer& aRenderer, const CU::Vector2<float>& aWindowSize);
		~DeferredRenderer();

		void Render(const Camera& aCamera);

		void Resize(float aWidth, float aHeight);

	private:
		void operator=(DeferredRenderer&) = delete;

		void RenderToGBuffer(const Camera& aCamera);
		void RenderAmbientPass(const Camera& aCamera);
		void RenderPointLights(const Camera& aCamera);

		EffectID myFullscreenEffect;
		EffectID myPointLightEffect;
		GBuffer* myGBuffer;
		Texture* myCubemap;

		Renderer& myRenderer;
		//Instance* myPointLightInstance;
	};
}