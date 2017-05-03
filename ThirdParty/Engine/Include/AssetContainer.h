#pragma once
#include <CUMap.h>
#include <CUString.h>
#include "Types.h"
#include <GrowingArray.h>
#include <Vector.h>

#include <functional>
#include <FileWatcher.h>

namespace std
{
	class thread;
}

namespace Magma
{
	class Effect;
	class GPUContext;
	class IModelFactory;
	class ModelData;
	class Texture;
	class Font;

	class AssetContainer
	{
	public:
		AssetContainer(GPUContext& aGPUContext);
		~AssetContainer();

		void FlushFileWatcher();

		ModelID LoadModel(const CU::String& aModelPath, const CU::String& aEffectPath);
		ModelData* GetModel(ModelID aID);

		EffectID LoadEffect(const CU::String& aFilePath);
		Effect* GetEffect(EffectID aID);

		EffectVariableID CreateEffectVariableID(const CU::String& aVariableName);
		const CU::String& GetEffectVariableName(EffectVariableID aVariableID);

		Texture* LoadTexture(const CU::String& aFilePath);

		Font* LoadFont(const CU::String& aFontTexturePath);

		std::function<void(EffectID)> ourReloadShadersCallback;

	private:
		void operator=(AssetContainer&) = delete;

		void Run();
		void OnReloadShader(const CU::String& aFilePath);

		IModelFactory* myModelFactory;

		CU::Map<EffectID, Effect*> myEffects;
		CU::Map<CU::String, EffectID> myEffectsID;
		EffectID myNextEffectID;

		CU::Map<EffectVariableID, CU::String> myEffectVariables;
		EffectVariableID myNextEffectVariableID;

		CU::Map<ModelID, ModelData*> myModels;
		CU::Map<CU::String, ModelID> myModelID;
		ModelID myNextModelID;

		CU::Map<CU::String, Texture*> myTextures;

		CU::String myDummyStringForReturning;
		CU::FileWatcher myFileWatcher;

		GPUContext& myGpuContext;
		static AssetContainer* myInstance;
	};
}