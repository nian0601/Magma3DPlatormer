#pragma once
#include <CUString.h>
#include "EngineTypes.h"

namespace Magma
{
	class Effect;
	class GPUContext;
	class Model;
	class ModelData;

	class IModelFactory
	{
	public:
		virtual ~IModelFactory(){};

		virtual ModelData* LoadModel(const CU::String<64>& aFilePath, EffectID aEffect) = 0;

	protected:
		IModelFactory(){};
	};
}