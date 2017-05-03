#pragma once

#include <CUMap.h>

namespace Magma
{
	class AssetContainer;
	class Texture;

	struct CharData
	{
		void LoadFromLine(const std::string& aLine, const CU::Vector2<float>& aTextureSize);
		std::string LoadToken(const std::string& aLine, const std::string& aToken);

		char myID;
		int myX;
		int myY;
		int myWidth;
		int myHeight;
		float myXOffset;
		float myYOffset;
		float myXAdvance;

		CU::Vector2<float> myTopLeftUV;
		CU::Vector2<float> myBottomRightUV;
	};

	class Font
	{
	public:
		Font();
		void LoadFromFile(const CU::String& aFilePath, AssetContainer& aAssetContainer);

		const CharData& GetCharData(char aCharacter) const;
		Texture* GetTexture() { return myTexture; }

		float GetScale() const { return myScale; }
		void SetScale(float aScale) { myScale = aScale; }

		float GetMaxHeight() const { return myMaxHeight * GetScale(); }

	private:

		CU::Map<char, CharData> myCharacters;
		CharData myEmptyCharData;
		Texture* myTexture;
		float myScale;
		float myMaxHeight;
	};
}