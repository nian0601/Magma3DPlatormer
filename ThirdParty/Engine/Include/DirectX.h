#pragma once

#include <Vector.h>

struct D3D11_VIEWPORT;
struct ID3D11Debug;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct IDXGISwapChain;

namespace Magma
{
	struct Backbuffer;

	class DirectX
	{
	public:
		DirectX();
		~DirectX();

		void Init(const HWND& aHWND, const CU::Vector2<float>& aScreenSize);

		void FinishFrame();

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetContext() const;

		void SetBackbufferAsRenderTarget();
		void GetBackbuffer(Backbuffer& aBackbufferOut);

	private:
		void SetupSwapChain(const HWND& aHWND, const CU::Vector2<float>& aScreenSize);
		void SetupDebugInterface();
		void SetupBackbuffer(const CU::Vector2<float>& aScreenSize);
		void SetupViewport(const CU::Vector2<float>& aScreenSize);

		void GetRefreshRate(const CU::Vector2<float>& aScreenSize, int& aNumeratorOut, int& aDenominatorOut) const;

		void CreateTexture2D(ID3D11Texture2D*& aTextureOut, const CU::Vector2<float>& aSize, unsigned int aFormat, unsigned int aBindFlags, unsigned int aUsage = 0);
		void CreateDepthStencilView(ID3D11DepthStencilView*& aViewOut, unsigned int aFormat, unsigned int aViewDimension, ID3D11Texture2D* aTexture);


		ID3D11Debug* myDebugInterface;
		ID3D11Device* myDevice;
		ID3D11DeviceContext* myContext;
		IDXGISwapChain* mySwapChain;
		D3D11_VIEWPORT* myViewPort;


		ID3D11Texture2D* myBackbuffer;
		ID3D11RenderTargetView* myBackbufferTarget;
		ID3D11ShaderResourceView* myBackbufferView;

		ID3D11Texture2D* myDepthStencil;
		ID3D11DepthStencilView* myDepthStencilView;

		float myClearColor[4];
	};
}