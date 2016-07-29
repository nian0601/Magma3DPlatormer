#include "stdafx.h"
#include "Camera.h"

namespace Magma
{
	Camera::Camera()
		: myIsDirty(true)
	{
	}


	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		if (myIsDirty == true)
		{
			myIsDirty = false;

			myView = CU::InverseSimple(myOrientation);
			myViewProjection = myView * myProjection;
		}
	}

	void Camera::Resize(const CU::Vector2<float>& aSize)
	{
		myProjection = CU::Matrix44<float>::CreateProjectionMatrixLH(0.1f, 100.f, aSize.y / aSize.x, PI * 0.5f);
		myIsDirty = true;
	}

	void Camera::Move(const CU::Vector3<float>& aMoveAmount)
	{
		myOrientation.SetPos(myOrientation.GetPos() + aMoveAmount * myOrientation);
		myIsDirty = true;
	}

	void Camera::Rotate(const CU::Matrix44<float>& aRotation)
	{
		CU::Vector3<float> pos = myOrientation.GetPos();
		myOrientation.SetPos(CU::Vector3<float>());
		myOrientation = aRotation * myOrientation;
		myOrientation.SetPos(pos);
		myIsDirty = true;
	}

}