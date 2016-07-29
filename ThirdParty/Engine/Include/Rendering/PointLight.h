#pragma once

#include <Matrix.h>
#include <Vector.h>

namespace Magma
{
	struct PointLightData
	{
		CU::Vector4<float> myPosition;
		CU::Vector4<float> myColor;
		float myRange;
	};

	class Instance;
	class PointLight
	{
	public:
		PointLight(const CU::Vector3<float>& aPosition, const CU::Vector4<float>& aColor, float aRange);
		~PointLight();

		const PointLightData& GetData() const;
		CU::Vector3<float> GetPosition() const;

	private:
		PointLightData myData;
	};

	inline const PointLightData& PointLight::GetData() const
	{
		return myData;
	}

	inline CU::Vector3<float> PointLight::GetPosition() const
	{
		return myData.myPosition.GetVector3();
	}
}