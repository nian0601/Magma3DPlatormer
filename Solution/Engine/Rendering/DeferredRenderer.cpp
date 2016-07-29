#include "stdafx.h"

#include "AssetContainer.h"
#include "Camera.h"
#include "GBuffer.h"
#include "DeferredRenderer.h"
#include "Renderer.h"
#include "PointLight.h"

namespace Magma
{
	DeferredRenderer::DeferredRenderer(AssetContainer& aAssetContainer, GPUContext& aGPUContext
		, Renderer& aRenderer, const CU::Vector2<float>& aWindowSize)
		: myRenderer(aRenderer)
	{
		myGBuffer = new GBuffer(aGPUContext, aWindowSize);

		myFullscreenEffect = aAssetContainer.LoadEffect("Data/Resource/Shader/S_effect_fullscreen.fx");
		myPointLightEffect = aAssetContainer.LoadEffect("Data/Resource/Shader/S_effect_deferred_pointlight.fx");

		myCubemap = aAssetContainer.RequestTexture("Data/Resource/Texture/church_cubemap.dds");

		//myPointLightInstance = aAssetContainer.CreateInstance("Data/Resource/Model/Light_Mesh/SM_sphere.fbx", "Data/Resource/Shader/S_effect_deferred_pointlight.fx");
	}


	DeferredRenderer::~DeferredRenderer()
	{
		SAFE_DELETE(myGBuffer);
	}

	void DeferredRenderer::Render(const Camera& aCamera)
	{
		RenderToGBuffer(aCamera);
		RenderAmbientPass(aCamera);
		//RenderPointLights(aScene);
	}

	void DeferredRenderer::Resize(float aWidth, float aHeight)
	{
		myGBuffer->Resize(aWidth, aHeight);
	}

	void DeferredRenderer::RenderToGBuffer(const Camera& aCamera)
	{
		myRenderer.SetDepthStencilState(ENABLED);
		myRenderer.SetRasterizerState(CULL_BACK);

		myRenderer.ClearRenderTarget(myGBuffer->myAlbedoAndMetalness);
		myRenderer.ClearRenderTarget(myGBuffer->myNormalAndRoughness);
		myRenderer.ClearRenderTarget(myGBuffer->myDepth);
		myRenderer.ClearDepthStencil(myGBuffer->myDepthStencil);

		myRenderer.AddRenderTarget(myGBuffer->myAlbedoAndMetalness);
		myRenderer.AddRenderTarget(myGBuffer->myNormalAndRoughness);
		myRenderer.AddRenderTarget(myGBuffer->myDepth);
		myRenderer.SetDepthStencil(myGBuffer->myDepthStencil);
		myRenderer.ApplyRenderTargetAndDepthStencil();

		myRenderer.RenderModels(aCamera);
	}

	void DeferredRenderer::RenderAmbientPass(const Camera& aCamera)
	{
		myRenderer.UseOriginalRenderTarget();
		myRenderer.UseOriginalDepthStencil();
		myRenderer.ApplyRenderTargetAndDepthStencil();

		myRenderer.SetDepthStencilState(NO_READ_NO_WRITE);

		myRenderer.SetEffect(myFullscreenEffect);
		myRenderer.SetTexture("AlbedoMetalnessTexture", myGBuffer->myAlbedoAndMetalness);
		myRenderer.SetTexture("NormalRoughnessTexture", myGBuffer->myNormalAndRoughness);
		myRenderer.SetTexture("DepthTexture", myGBuffer->myDepth);
		myRenderer.SetTexture("Cubemap", myCubemap);
		myRenderer.SetMatrix("InvertedProjection", CU::InverseReal(aCamera.GetProjection()));
		myRenderer.SetMatrix("NotInvertedView", aCamera.GetNotInvertedView());

		myRenderer.RenderFullScreen("Deferred_Ambient");
	}

	void DeferredRenderer::RenderPointLights(const Camera& aCamera)
	{
		myRenderer.UseOriginalRenderTarget();
		myRenderer.SetDepthStencil(myGBuffer->myDepthStencil);
		myRenderer.ApplyRenderTargetAndDepthStencil();

		myRenderer.SetRasterizerState(NO_CULLING);
		myRenderer.SetDepthStencilState(READ_NO_WRITE);

		myRenderer.SetEffect(myPointLightEffect);
		myRenderer.SetTexture("AlbedoMetalnessTexture", myGBuffer->myAlbedoAndMetalness);
		myRenderer.SetTexture("NormalRoughnessTexture", myGBuffer->myNormalAndRoughness);
		myRenderer.SetTexture("DepthTexture", myGBuffer->myDepth);
		myRenderer.SetMatrix("InvertedProjection", CU::InverseReal(aCamera.GetProjection()));
		myRenderer.SetMatrix("NotInvertedView", aCamera.GetNotInvertedView());

		/*const CU::GrowingArray<PointLight*>& pointLights = aScene->GetPointLights();
		for each (const PointLight* light in pointLights)
		{
		myRenderer.SetRawData("PointLight", sizeof(light->GetData()), &light->GetData());
		myPointLightInstance->SetPosition(light->GetPosition());
		myPointLightInstance->Render(&myRenderer, aScene->GetCamera());
		}*/


		myRenderer.SetDepthStencilState(ENABLED);
		myRenderer.SetRasterizerState(CULL_BACK);
	}

}