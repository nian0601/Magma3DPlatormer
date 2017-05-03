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
		void LoadTexture(const CU::String& aPath, GPUContext& aGpuContext);
		void Resize(float aWidth, float aHeight, GPUContext& aGpuContext);

		const CU::Vector2<float>& GetSize() const {	return mySize; }

		ID3D11ShaderResourceView* GetShaderView() { return myShaderView; }
		ID3D11RenderTargetView* GetRenderTarget() { return myRenderTarget; }

		ID3D11ShaderResourceView* GetDepthShaderView() { return myDepthShaderView; }
		ID3D11DepthStencilView* GetDepthStencil() { return myDepthStencil; }

	private:
		void CreateShaderViewAndRenderTarget(float aWidth, float aHeight, unsigned int aBindFlag
			, unsigned int aFormat, GPUContext& aGpuContext);
		void CreateDepthStencil(float aWidth, float aHeight, GPUContext& aGpuContext);

		CU::String myFilePath;
		unsigned int myFormat;

		ID3D11Texture2D* myTexture;
		ID3D11ShaderResourceView* myShaderView;
		ID3D11RenderTargetView* myRenderTarget;

		ID3D11Texture2D* myDepthTexture;
		ID3D11ShaderResourceView* myDepthShaderView;
		ID3D11DepthStencilView* myDepthStencil;

		CU::Vector2<float> mySize;
	};
}