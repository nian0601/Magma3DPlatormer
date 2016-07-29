#include "stdafx.h"

#include "Backbuffer.h"
#include "DirectX.h"
#include <d3d11.h>

namespace Magma
{
	DirectX::DirectX()
		: myDevice(nullptr)
		, myContext(nullptr)
		, mySwapChain(nullptr)
		, myDebugInterface(nullptr)
		, myViewPort(nullptr)
		, myBackbuffer(nullptr)
		, myBackbufferTarget(nullptr)
		, myBackbufferView(nullptr)
		, myDepthStencil(nullptr)
		, myDepthStencilView(nullptr)
	{
		myClearColor[0] = 0.2f;
		myClearColor[1] = 0.2f;
		myClearColor[2] = 0.2f;
		myClearColor[3] = 1.f;
	}


	DirectX::~DirectX()
	{
		mySwapChain->SetFullscreenState(FALSE, NULL);

		SAFE_RELEASE(mySwapChain);

		SAFE_RELEASE(myBackbuffer);
		SAFE_RELEASE(myBackbufferTarget);
		SAFE_RELEASE(myBackbufferView);

		SAFE_RELEASE(myDepthStencil);
		SAFE_RELEASE(myDepthStencilView);

		SAFE_RELEASE(myDevice);

		SAFE_DELETE(myViewPort);

		myContext->ClearState();
		myContext->Flush();
		SAFE_RELEASE(myContext);

#if _DEBUG
		myDebugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		SAFE_RELEASE(myDebugInterface);
#endif
	}

	void DirectX::Init(const HWND& aHWND, const CU::Vector2<float>& aScreenSize)
	{
		SetupSwapChain(aHWND, aScreenSize);
		SetupDebugInterface();
		SetupBackbuffer(aScreenSize);
		SetupViewport(aScreenSize);
	}

	void DirectX::FinishFrame()
	{
		mySwapChain->Present(1, 0);
		myContext->ClearRenderTargetView(myBackbufferTarget, myClearColor);
		myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	ID3D11Device* DirectX::GetDevice() const
	{
		return myDevice;
	}

	ID3D11DeviceContext* DirectX::GetContext() const
	{
		return myContext;
	}


	void DirectX::SetBackbufferAsRenderTarget()
	{
		myContext->OMSetRenderTargets(1, &myBackbufferTarget, myDepthStencilView);
	}

	void DirectX::GetBackbuffer(Backbuffer& aBackbufferOut)
	{
		aBackbufferOut.myBackbufferTexture = myBackbuffer;
		aBackbufferOut.myBackbufferTarget = myBackbufferTarget;
		aBackbufferOut.myBackbufferView = myBackbufferView;

		aBackbufferOut.myDepthStencilTexture = myDepthStencil;
		aBackbufferOut.myDepthStencilView = myDepthStencilView;
	}

	void DirectX::SetupSwapChain(const HWND& aHWND, const CU::Vector2<float>& aScreenSize)
	{
		int numerator = 0;
		int denominator = 0;
		GetRefreshRate(aScreenSize, numerator, denominator);

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(swapDesc));

		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Width = int(aScreenSize.x);
		swapDesc.BufferDesc.Height = int(aScreenSize.y);
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		swapDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapDesc.BufferDesc.RefreshRate.Denominator = denominator;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		swapDesc.OutputWindow = aHWND;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.Windowed = true;

		HRESULT result = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
#ifdef _DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			NULL,
#endif
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&swapDesc,
			&mySwapChain,
			&myDevice,
			NULL,
			&myContext);

		DL_ASSERT_EXP(SUCCEEDED(result) == TRUE, "Failed to CreateDeviceAndSwapChain");
	}

	void DirectX::SetupDebugInterface()
	{
#ifdef _DEBUG
		myDebugInterface = nullptr;
		HRESULT result = myDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&myDebugInterface);
		DL_ASSERT_EXP(SUCCEEDED(result) == TRUE, "Failed to Query DebugInterface");


		ID3D11InfoQueue* infoQueue = nullptr;
		DL_ASSERT_EXP(SUCCEEDED(myDebugInterface->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&infoQueue)) == TRUE, "Failed to Query InfoQueue");
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

		D3D11_MESSAGE_ID hide[] =
		{
			D3D11_MESSAGE_ID_DEVICE_PSSETSHADERRESOURCES_HAZARD,
			D3D11_MESSAGE_ID_DEVICE_OMSETRENDERTARGETS_HAZARD
			// Add more message IDs here as needed
		};

		D3D11_INFO_QUEUE_FILTER filter;
		memset(&filter, 0, sizeof(filter));
		filter.DenyList.NumIDs = _countof(hide);
		filter.DenyList.pIDList = hide;
		infoQueue->AddStorageFilterEntries(&filter);
		infoQueue->Release();
#endif
	}

	void DirectX::SetupBackbuffer(const CU::Vector2<float>& aScreenSize)
	{
		mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&myBackbuffer);
		myDevice->CreateRenderTargetView(myBackbuffer, NULL, &myBackbufferTarget);
		myDevice->CreateShaderResourceView(myBackbuffer, NULL, &myBackbufferView);

		CreateTexture2D(myDepthStencil, aScreenSize, DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL);
		CreateDepthStencilView(myDepthStencilView, DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, myDepthStencil);

		myContext->OMSetRenderTargets(1, &myBackbufferTarget, myDepthStencilView);
	}

	void DirectX::SetupViewport(const CU::Vector2<float>& aScreenSize)
	{
		SAFE_DELETE(myViewPort);

		myViewPort = new D3D11_VIEWPORT();
		ZeroMemory(myViewPort, sizeof(D3D11_VIEWPORT));

		myViewPort->TopLeftX = 0;
		myViewPort->TopLeftY = 0;
		myViewPort->Width = aScreenSize.x;
		myViewPort->Height = aScreenSize.y;
		myViewPort->MinDepth = 0.f;
		myViewPort->MaxDepth = 1.f;

		myContext->RSSetViewports(1, myViewPort);
	}

	void DirectX::GetRefreshRate(const CU::Vector2<float>& aScreenSize, int& aNumeratorOut, int& aDenominatorOut) const
	{
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes;
		DXGI_MODE_DESC* displayModeList;

		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		result = factory->EnumAdapters(0, &adapter);
		result = adapter->EnumOutputs(0, &adapterOutput);
		adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		displayModeList = new DXGI_MODE_DESC[numModes];
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

		for (unsigned int i = 0; i < numModes; ++i)
		{
			if (displayModeList[i].Width == aScreenSize.x && displayModeList[i].Height == aScreenSize.y)
			{
				aNumeratorOut = displayModeList[i].RefreshRate.Numerator;
				aDenominatorOut = displayModeList[i].RefreshRate.Denominator;
			}
		}

		SAFE_ARRAY_DELETE(displayModeList);
		SAFE_RELEASE(adapterOutput);
		SAFE_RELEASE(adapter);
		SAFE_RELEASE(factory);
	}

	void DirectX::CreateTexture2D(ID3D11Texture2D*& aTextureOut, const CU::Vector2<float>& aSize, unsigned int aFormat, unsigned int aBindFlags, unsigned int aUsage)
	{
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));

		texDesc.Width = int(aSize.x);
		texDesc.Height = int(aSize.y);
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT(aFormat);
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE(aUsage);
		texDesc.BindFlags = aBindFlags;

		DL_ASSERT_EXP(SUCCEEDED(myDevice->CreateTexture2D(&texDesc, NULL, &aTextureOut)) == TRUE, "Failed to CreateTexture2D");
	}

	void DirectX::CreateDepthStencilView(ID3D11DepthStencilView*& aViewOut, unsigned int aFormat, unsigned int aViewDimension, ID3D11Texture2D* aTexture)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.Format = DXGI_FORMAT(aFormat);
		stencilDesc.ViewDimension = D3D11_DSV_DIMENSION(aViewDimension);

		stencilDesc.Texture2D.MipSlice = 0;

		DL_ASSERT_EXP(SUCCEEDED(myDevice->CreateDepthStencilView(aTexture, &stencilDesc, &aViewOut)) == TRUE, "Failed to CreateDepthStencilView");
	}
}