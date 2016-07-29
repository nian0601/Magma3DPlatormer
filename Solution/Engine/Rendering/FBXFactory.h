#pragma once

#include "IModelFactory.h"
#include "GfxStructs.h"
#include <GrowingArray.h>
#include <CUMap.h>

struct D3D11_INPUT_ELEMENT_DESC;

namespace FBX
{
	struct AnimationData;
	struct Bone;
	struct ModelData;

	class FBXLoader;
	class FbxModelData;
}

namespace Magma
{
	class AssetContainer;
	class ModelData;
	class GPUData;
	class Texture;

	class FBXFactory : public IModelFactory
	{
	public:
		FBXFactory(AssetContainer& aAssetContainer, GPUContext& aGPUContext);
		~FBXFactory();

		ModelData* LoadModel(const CU::String<64>& aFilePath, EffectID aEffect) override;
		
	private:
		void operator=(FBXFactory&) = delete;

		ModelData* CreateModel(FBX::FbxModelData* someModelData);
		void LoadData(GPUData& someGPUData, FBX::ModelData* someFBXData);

		AssetContainer& myAssetContainer;
		GPUContext& myGPUContext;

		FBX::FBXLoader *myLoader;
		CU::Map<CU::String<64>, ModelData*> myModels;
	};
}