#include "stdafx.h"

#include "AssetContainer.h"
#include "Camera.h"
#include "DeferredRenderer.h"
#include "Engine.h"
#include "GPUContext.h"
#include "IGame.h"
#include <InputWrapper.h>
#include "Renderer.h"
#include "RendererProxy.h"
#include <TimerManager.h>
#include "WindowHandler.h"

namespace Magma
{
	Engine::Engine(const CU::Vector2<float>& aSize, IGame& aGame)
		: myGPUContext(nullptr)
		, myWindowSize(aSize)
		, myGame(aGame)
	{
		myWindowHandler = new WindowHandler(myWindowSize);
		myGPUContext = new GPUContext(myWindowSize, myWindowHandler->GetHwnd());
		myAssetContainer = new AssetContainer(*myGPUContext);

		myRenderer = new Renderer(*myAssetContainer, *myGPUContext);
		myRendererProxy = new RendererProxy(*myRenderer);
		myRenderer->SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		myDeferredRenderer = new DeferredRenderer(*myAssetContainer, *myGPUContext
			, *myRenderer, myWindowSize);

		myCamera = new Camera();
		myCamera->Resize(myWindowSize);

		myTimerManager = new CU::TimerManager();

		CU::InputWrapper::Create(myWindowHandler->GetHwnd(), GetModuleHandle(NULL), DISCL_NONEXCLUSIVE
			| DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		myGame.Init(*this);
	}


	Engine::~Engine()
	{
		SAFE_DELETE(myTimerManager);
		SAFE_DELETE(myCamera);
		SAFE_DELETE(myDeferredRenderer);
		SAFE_DELETE(myRendererProxy);
		SAFE_DELETE(myRenderer);
		SAFE_DELETE(myAssetContainer);
		SAFE_DELETE(myGPUContext);
		SAFE_DELETE(myWindowHandler);
		DL_Debug::Debug::Destroy();
		CU::InputWrapper::Destroy();
	}

	void Engine::Run()
	{
		myIsRunning = true;
		while (myIsRunning == true)
		{
			if (myWindowHandler->PumpEvent() == false)
			{
				myIsRunning = false;
			}
			else
			{
				if (myWindowHandler->GetShouldResize() == true)
				{
					myWindowHandler->HasResized();
					myWindowSize = myWindowHandler->GetNewSize();

					//OnResize();
					myGame.OnResize(myWindowSize.x, myWindowSize.y);
				}

				myTimerManager->Update();
				CU::InputWrapper::GetInstance()->Update();

				myIsRunning = myGame.Update(myTimerManager->GetMasterTimer().GetTime().GetFrameTime());

				myDeferredRenderer->Render(*myCamera);
				myGPUContext->FinishFrame();

				myTimerManager->CapFrameRate(60.f);
			}
		}
	}

	void Engine::OnResize()
	{
		myDeferredRenderer->Resize(myWindowSize.x, myWindowSize.y);
		myCamera->Resize(myWindowSize);
	}

}