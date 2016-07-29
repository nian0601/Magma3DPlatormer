#include "stdafx.h"

#include <D3DX11tex.h>
#include "Effect.h"
#include "GBuffer.h"
#include "GPUContext.h"
#include "Texture.h"


namespace Magma
{
	GBuffer::GBuffer(GPUContext& aGPUContext, const CU::Vector2<float>& aWindowSize)
		: myGPUContext(aGPUContext)
	{
		myAlbedoAndMetalness = new Texture();
		myAlbedoAndMetalness->InitForShader(aWindowSize.x, aWindowSize.y
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM, myGPUContext);

		myNormalAndRoughness = new Texture();
		myNormalAndRoughness->InitForShader(aWindowSize.x, aWindowSize.y
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM, myGPUContext);

		myDepth = new Texture();
		myDepth->InitForShader(aWindowSize.x, aWindowSize.y
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R32_FLOAT, myGPUContext);

		myDepthStencil = new Texture();
		myDepthStencil->InitForShader(aWindowSize.x, aWindowSize.y
			, D3D11_BIND_DEPTH_STENCIL
			, DXGI_FORMAT_R32_TYPELESS, myGPUContext);
	}


	GBuffer::~GBuffer()
	{
		SAFE_DELETE(myAlbedoAndMetalness);
		SAFE_DELETE(myNormalAndRoughness);
		SAFE_DELETE(myDepth);
		SAFE_DELETE(myDepthStencil);
	}

	void GBuffer::Set(Effect* aEffect)
	{
		aEffect->SetAlbedoMetalness(myAlbedoAndMetalness->GetShaderView());
		aEffect->SetNormalRoughness(myNormalAndRoughness->GetShaderView());
		aEffect->SetDepth(myDepth->GetShaderView());
	}

	void GBuffer::UnSet(Effect* aEffect)
	{
		aEffect->SetAlbedoMetalness(nullptr);
		aEffect->SetNormalRoughness(nullptr);
		aEffect->SetDepth(nullptr);
	}

	void GBuffer::Clear(const CU::Vector4<float>& aColor)
	{
		float clearColor[4];
		clearColor[0] = aColor.x;
		clearColor[1] = aColor.y;
		clearColor[2] = aColor.z;
		clearColor[3] = aColor.w;


		myGPUContext.GetContext()->ClearRenderTargetView(myAlbedoAndMetalness->GetRenderTarget(), clearColor);
		myGPUContext.GetContext()->ClearRenderTargetView(myNormalAndRoughness->GetRenderTarget(), clearColor);
		myGPUContext.GetContext()->ClearRenderTargetView(myDepth->GetRenderTarget(), clearColor);
		
		myGPUContext.GetContext()->ClearDepthStencilView(myDepthStencil->GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void GBuffer::Resize(float aWidth, float aHeight)
	{
		myAlbedoAndMetalness->Resize(aWidth, aHeight, myGPUContext);
		myNormalAndRoughness->Resize(aWidth, aHeight, myGPUContext);
		myDepth->Resize(aWidth, aHeight, myGPUContext);
	}

}