#pragma once
#include <Entity/Include/BaseComponent.h>
#include <Matrix.h>
#include <Vector.h>

class TranslationComponent : public Magma::BaseComponent
{
public:
	TranslationComponent();
	~TranslationComponent();

	CU::Matrix44<float> myOrientation;
	CU::Vector3<float> myScale;
};

