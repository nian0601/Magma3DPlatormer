#pragma once
#include <Entity/Include/BaseProcessor.h>

class InputProcessor : public Magma::BaseProcessor
{
public:
	InputProcessor(Magma::World& aWorld);
	~InputProcessor();

	void Update(float aDelta) override;
};