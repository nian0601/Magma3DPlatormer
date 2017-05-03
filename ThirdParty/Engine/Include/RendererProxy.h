#pragma once

#include "Types.h"

namespace Magma
{
	class Renderer;
	class Texture;
	class Font;
	class RendererProxy
	{
	public:
		RendererProxy(Renderer& aRenderer);
		~RendererProxy();

		void RenderModel(ModelID aModelID, EffectID aEffectID
			, const CU::Matrix44<float>& aOrientation, const CU::Vector3<float>& aScale = CU::Vector3<float>());
		void RenderSprite(Texture* aTexture, const CU::Matrix44<float>& aOrientation
			, const CU::Vector4<float>& aSizeAndHotSpot, const CU::Vector4<float>& aPositionAndScale);
		void RenderText(const CU::String& aString, const CU::Vector2<float>& aPosition);
		void RenderText(Font* aFont, const CU::String& aString, const CU::Vector2<float>& aPosition);

	private:
		void operator=(RendererProxy&) = delete;

		Renderer& myRenderer;
	};
}