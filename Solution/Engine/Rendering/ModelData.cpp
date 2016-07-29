#include "stdafx.h"
#include "ModelData.h"
#include "GPUData.h"

namespace Magma
{

	ModelData::ModelData()
		: myChildren(4)
		, myChildTransforms(4)
		, myIsNullObject(true)
		, myGPUData(nullptr)
	{

	}

	ModelData::~ModelData()
	{

	}

	void ModelData::Init(EffectID aEffectID, GPUContext& aGPUContext, AssetContainer& aAssetContainer)
	{
		if (myIsNullObject == false)
		{
			myGPUData->Init(aEffectID, aGPUContext, aAssetContainer);
		}

		for each (ModelData* child in myChildren)
		{
			child->Init(aEffectID, aGPUContext, aAssetContainer);
		}
	}

	void ModelData::AddChild(ModelData* aModelData)
	{
		myChildren.Add(aModelData);
		myChildTransforms.Add(aModelData->myOrientation);
	}
}