#pragma once
#include "Types.h"
#include "GfxStructs.h"
#include <GrowingArray.h>
#include "GPUData.h"
#include <Matrix.h>
#include <Vector.h>
#include "TextData.h"

namespace Magma
{
	class Camera;
	class Effect;
	class GPUContext;
	class TextData;
	class Texture;
	class Renderer;

	class QuadRenderer
	{
	public:
		QuadRenderer(AssetContainer& aAssetContainer, GPUContext& aGPUContext);
		~QuadRenderer();

		void AddSpriteCommand(Texture* aTexture, const CU::Matrix44<float>& aOrientation
			, const CU::Vector4<float>& aSizeAndHotSpot, const CU::Vector4<float>& aPositionAndScale);
		void AddTextCommand(const CU::String& aString, const CU::Vector2<float>& aPosition);
		void AddTextCommand(Font* aFont, const CU::String& aString, const CU::Vector2<float>& aPosition);

		void Activate();

		void RenderSprites(const Camera& aCamera, Renderer& aRenderer);
		void RenderTexts(const Camera& aCamera, Renderer& aRenderer);
		void Render(EffectID aEffect, const CU::String& aTechnique);

		void RenderText(TextData* someTextData, const Camera& aCamera, Renderer& aRenderer, const CU::Vector4<float>& aPositionAndScale);

		float GetTextHeight() const;

	private:
		void operator=(QuadRenderer&) = delete;

		AssetContainer& myAssetContainer;
		GPUContext& myGPUContext;
		GPUData myGPUData;

		struct SpriteCommand
		{
			SpriteCommand()
			{}
			SpriteCommand(Texture* aTexture, const CU::Matrix44<float>& aOrientation
				, const CU::Vector4<float>& aSizeAndHotSpot
				, const CU::Vector4<float>& aPositionAndScale)
				: myTexture(aTexture)
				, myOrientation(aOrientation)
				, mySizeAndHotSpot(aSizeAndHotSpot)
				, myPositionAndScale(aPositionAndScale)
			{}

			Texture* myTexture;
			CU::Matrix44<float> myOrientation;
			CU::Vector4<float> mySizeAndHotSpot;
			CU::Vector4<float> myPositionAndScale;
		};
		CU::GrowingArray<SpriteCommand> mySpriteCommands;

		struct TextCommand
		{
			TextCommand()
			{}
			TextCommand(Font* aFont, const CU::String& aString, const CU::Vector2<float>& aPosition)
				: myFont(aFont)
				, myString(aString)
				, myPosition(aPosition)
			{}

			Font* myFont;
			CU::String myString;
			CU::Vector2<float> myPosition;
		};
		CU::GrowingArray<TextCommand> myTextCommands;

		EffectID mySpriteEffect;
		EffectVariableID myProjectionVariable;
		EffectVariableID myOrientationVariable;
		EffectVariableID mySizeAndHotSpotVariable;
		EffectVariableID myPosAndScaleVarible;
		EffectVariableID myAlbedoTextureVariable;

		Font* myFont;
		EffectID myFontEffect;
		TextData myTextData;
	};
}