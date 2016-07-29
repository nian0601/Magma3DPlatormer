#include "stdafx.h"

#include "DirectX.h"
#include "GPUContext.h"

namespace Magma
{
	GPUContext::GPUContext(const CU::Vector2<float>& aScreenSize, const HWND& aHwnd)
	{
		myDirectX = new DirectX();
		myDirectX->Init(aHwnd, aScreenSize);
	}


	GPUContext::~GPUContext()
	{
		SAFE_DELETE(myDirectX);
	}

	void GPUContext::FinishFrame()
	{
		myDirectX->FinishFrame();
	}

	ID3D11Device* GPUContext::GetDevice() const
	{
		return myDirectX->GetDevice();
	}

	ID3D11DeviceContext* GPUContext::GetContext() const
	{
		return myDirectX->GetContext();
	}

	void GPUContext::SetBackbufferAsRenderTarget()
	{
		myDirectX->SetBackbufferAsRenderTarget();
	}

	void GPUContext::GetBackbuffer(Backbuffer& aBackbufferOut)
	{
		myDirectX->GetBackbuffer(aBackbufferOut);
	}


}