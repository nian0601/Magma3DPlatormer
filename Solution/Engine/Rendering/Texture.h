#pragma once

#include <CUString.h>

struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;

namespace Magma
{
	class GPUContext;

	class Texture
	{
	public:
		Texture();
		~Texture();

		void InitForShader(float aWidth, float aHeight, unsigned int aBindFlag
			, unsigned int aFormat, GPUContext& aGpuContext);
		void LoadTexture(const CU::String<64>& aPath, GPUContext& aGpuContext);
		void Resize(float aWidth, float aHeight, GPUContext& aGpuContext);

		ID3D11ShaderResourceView* GetShaderView();
		ID3D11RenderTargetView* GetRenderTarget();

		ID3D11ShaderResourceView* GetDepthShaderView();
		ID3D11DepthStencilView* GetDepthStencil();

	private:
		void CreateShaderViewAndRenderTarget(float aWidth, float aHeight, unsigned int aBindFlag
			, unsigned int aFormat, GPUContext& aGpuContext);
		void CreateDepthStencil(float aWidth, float aHeight, GPUContext& aGpuContext);

		CU::String<64> myFilePath;
		unsigned int myFormat;

		ID3D11Texture2D* myTexture;
		ID3D11ShaderResourceView* myShaderView;
		ID3D11RenderTargetView* myRenderTarget;

		ID3D11Texture2D* myDepthTexture;
		ID3D11ShaderResourceView* myDepthShaderView;
		ID3D11DepthStencilView* myDepthStencil;
	};

	inline ID3D11ShaderResourceView* Texture::GetShaderView()
	{
		return myShaderView;
	}

	inline ID3D11RenderTargetView* Texture::GetRenderTarget()
	{
		return myRenderTarget;
	}

	inline ID3D11ShaderResourceView* Texture::GetDepthShaderView()
	{
		return myDepthShaderView;
	}

	inline ID3D11DepthStencilView* Texture::GetDepthStencil()
	{
		return myDepthStencil;
	}
}