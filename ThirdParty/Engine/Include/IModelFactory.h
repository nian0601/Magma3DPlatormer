#pragma once
#include <CUString.h>
#include "Types.h"

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

		virtual ModelData* LoadModel(const CU::String& aFilePath, EffectID aEffect) = 0;

	protected:
		IModelFactory(){};
	};
}