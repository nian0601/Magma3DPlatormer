#pragma once
#include <Entity/Include/BaseComponent.h>
#include <EngineTypes.h>

class RenderComponent : public Magma::BaseComponent
{
public:
	RenderComponent();
	~RenderComponent();

	Magma::ModelID myModelID;
	Magma::EffectID myEffectID;
};

