#include "stdafx.h"

#include <D3DX11tex.h>
#include "Texture.h"
#include "GPUContext.h"

namespace Magma
{
	Texture::Texture()
		: myTexture(nullptr)
		, myShaderView(nullptr)
		, myRenderTarget(nullptr)
		, myDepthTexture(nullptr)
		, myDepthShaderView(nullptr)
		, myDepthStencil(nullptr)
	{
	}


	Texture::~Texture()
	{
		SAFE_RELEASE(myShaderView);
	}

	void Texture::InitForShader(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat, GPUContext& aGpuContext)
	{
		myFormat = aFormat;

		if ((aBindFlag & D3D11_BIND_SHADER_RESOURCE) > 0 || (aBindFlag & D3D11_BIND_RENDER_TARGET) > 0)
		{
			CreateShaderViewAndRenderTarget(aWidth, aHeight, aBindFlag, aFormat, aGpuContext);
		}

		if ((aBindFlag & D3D11_BIND_DEPTH_STENCIL) > 0)
		{
			CreateDepthStencil(aWidth, aHeight, aGpuContext);
		}
	}

	void Texture::LoadTexture(const CU::String<64>& aPath, GPUContext& aGpuContext)
	{
		myFilePath = aPath;

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(aGpuContext.GetDevice()
			, myFilePath.c_str(), NULL, NULL, &myShaderView, NULL);
		

		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX(CU::Concatenate("Failed to load: %s,\ntrying to use MissingTexture-texture.", myFilePath.c_str()).c_str()
				, "Failed to load texture", MB_ICONWARNING);

			myFilePath = "Data/Resource/Texture/T_missing_texture.dds";
			hr = D3DX11CreateShaderResourceViewFromFile(aGpuContext.GetDevice()
				, myFilePath.c_str(), NULL, NULL, &myShaderView, NULL);

			
			if (FAILED(hr) != S_OK)
			{
				DL_ASSERT("[Texture]: Failed to load MissingTexture-texture: Data/Resource//T_missing_texture.dds");
			}
		}
	}

	void Texture::Resize(float aWidth, float aHeight, GPUContext& aGpuContext)
	{
		int bindFlag = 0;
		if (myShaderView != nullptr)
		{
			bindFlag |= D3D11_BIND_SHADER_RESOURCE;
			SAFE_RELEASE(myShaderView);
		}

		if (myRenderTarget != nullptr)
		{
			bindFlag |= D3D11_BIND_RENDER_TARGET;
			SAFE_RELEASE(myRenderTarget);
		}

		if (myDepthStencil != nullptr)
		{
			bindFlag |= D3D11_BIND_DEPTH_STENCIL;
			SAFE_RELEASE(myDepthStencil);
			SAFE_RELEASE(myDepthShaderView);
		}

		InitForShader(aWidth, aHeight, bindFlag, myFormat, aGpuContext);
	}

	void Texture::CreateShaderViewAndRenderTarget(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat, GPUContext& aGpuContext)
	{
		D3D11_TEXTURE2D_DESC tempBufferInfo;
		tempBufferInfo.Width = static_cast<unsigned int>(aWidth);
		tempBufferInfo.Height = static_cast<unsigned int>(aHeight);
		tempBufferInfo.MipLevels = 1;
		tempBufferInfo.ArraySize = 1;
		tempBufferInfo.Format = static_cast<DXGI_FORMAT>(aFormat);
		tempBufferInfo.SampleDesc.Count = 1;
		tempBufferInfo.SampleDesc.Quality = 0;
		tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		tempBufferInfo.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tempBufferInfo.CPUAccessFlags = 0;
		tempBufferInfo.MiscFlags = 0;

		HRESULT hr = aGpuContext.GetDevice()->CreateTexture2D(&tempBufferInfo, NULL, &myTexture);

		if ((aBindFlag & D3D11_BIND_SHADER_RESOURCE) > 0)
		{
			hr = aGpuContext.GetDevice()->CreateShaderResourceView(myTexture, NULL, &myShaderView);
			if (FAILED(hr))
				assert(0);
		}

		if ((aBindFlag & D3D11_BIND_RENDER_TARGET) > 0)
		{
			hr = aGpuContext.GetDevice()->CreateRenderTargetView(myTexture, NULL, &myRenderTarget);
			if (FAILED(hr))
				assert(0);
		}
	}

	void Texture::CreateDepthStencil(float aWidth, float aHeight, GPUContext& aGpuContext)
	{
		D3D11_TEXTURE2D_DESC tempBufferInfo;
		tempBufferInfo.Width = static_cast<unsigned int>(aWidth);
		tempBufferInfo.Height = static_cast<unsigned int>(aHeight);
		tempBufferInfo.MipLevels = 1;
		tempBufferInfo.ArraySize = 1;
		tempBufferInfo.Format = DXGI_FORMAT_R32_TYPELESS;
		tempBufferInfo.SampleDesc.Count = 1;
		tempBufferInfo.SampleDesc.Quality = 0;
		tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		tempBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		tempBufferInfo.CPUAccessFlags = 0;
		tempBufferInfo.MiscFlags = 0;

		HRESULT hr = aGpuContext.GetDevice()->CreateTexture2D(&tempBufferInfo, NULL, &myDepthTexture);
		if (FAILED(hr))
			assert(0);


		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthDesc.Texture2D.MipSlice = 0;

		hr = aGpuContext.GetDevice()->CreateDepthStencilView(myDepthTexture, &depthDesc, &myDepthStencil);
		if (FAILED(hr))
			assert(0);


		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = 1;
		viewDesc.Texture2D.MostDetailedMip = 0;

		hr = aGpuContext.GetDevice()->CreateShaderResourceView(myDepthTexture, &viewDesc, &myDepthShaderView);
		if (FAILED(hr))
			assert(0);
	}
}