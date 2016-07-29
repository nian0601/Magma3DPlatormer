#pragma once

#include <IGame.h>
#include <GrowingArray.h>
#include <Entity/Include/World.h>

#include <Subscriber.h>

namespace Magma
{
	class AssetContainer;
	class Camera;
	class RendererProxy;
}

class Game : public Magma::IGame, public Subscriber
{
public:
	Game();
	~Game();

	void Init(Magma::Engine& aEngine) override;
	bool Update(float aDelta) override;
	void OnResize(float aWidth, float aHeight) override;

	void ReceiveMessage(const RenderMessage& aMessage);

private:
	void UpdateCamera(float aDelta);
	void LoadLevel(Magma::AssetContainer& aAssetContainer);

	Magma::Camera* myCamera;
	Magma::RendererProxy* myRendererProxy;

	Magma::World myWorld;
};

