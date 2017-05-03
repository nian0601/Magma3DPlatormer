#pragma once

#include <Matrix.h>

namespace Magma
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void Update();
		void Resize(const CU::Vector2<float>& aSize);

		void Move(const CU::Vector3<float>& aMoveAmount);
		void Rotate(const CU::Matrix44<float>& aRotation);


		const CU::Matrix44<float>& GetView() const;
		const CU::Matrix44<float>& GetNotInvertedView() const;
		const CU::Matrix44<float>& GetProjection() const;
		const CU::Matrix44<float>& GetViewProjection() const;
		const CU::Matrix44<float>& GetOrthagonalProjection() const;

		CU::Vector3<float> GetPosition() const;

	private:
		CU::Matrix44<float> myOrthagonalProjection;
		CU::Matrix44<float> myProjection;
		CU::Matrix44<float> myView;
		CU::Matrix44<float> myViewProjection;

		CU::Matrix44<float> myOrientation;
		bool myIsDirty;
	};

	inline const CU::Matrix44<float>& Camera::GetView() const
	{
		return myView;
	}

	inline const CU::Matrix44<float>& Camera::GetNotInvertedView() const
	{
		return myOrientation;
	}

	inline const CU::Matrix44<float>& Camera::GetProjection() const
	{
		return myProjection;
	}

	inline const CU::Matrix44<float>& Camera::GetViewProjection() const
	{
		return myViewProjection;
	}

	inline const CU::Matrix44<float>& Camera::GetOrthagonalProjection() const
	{
		return myOrthagonalProjection;
	}

	inline CU::Vector3<float> Camera::GetPosition() const
	{
		return myOrientation.GetPos();
	}
}

