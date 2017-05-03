#pragma once

#include <GrowingArray.h>
#include <Matrix.h>
#include "Types.h"

namespace Magma
{
	class AssetContainer;
	class GPUContext;
	class GPUData;
	class Font;

	class TextData
	{
	public:
		TextData();
		~TextData();

		void Init(EffectID aEffectID, GPUContext& aGPUContext, AssetContainer& aAssetContainer);
		CU::Vector2<int> SetupBuffers(const CU::String& aString, Font* aFont);

		GPUData* GetGPUData() { return myGPUData; }
		EffectID GetEffectID() { return myEffectID; }
		Font* GetFont() { return myFont; }

	private:
		GPUData* myGPUData;
		GPUContext* myGPUContext;
		EffectID myEffectID;
		Font* myFont;
	};
}