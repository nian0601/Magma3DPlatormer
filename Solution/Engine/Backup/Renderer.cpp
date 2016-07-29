#include "stdafx.h"

#include "AssetContainer.h"
#include "Camera.h"
#include <d3d11.h>
#include <D3DX11async.h>
#include <d3dx11effect.h>
#include "Effect.h"
#include "Renderer.h"
#include "Texture.h"
#include "GPUContext.h"
#include "GPUData.h"

#include <PostMaster.h>

namespace Magma
{
	Renderer::Renderer(AssetContainer& aAssetContainer, GPUContext& aGPUContext)
		: myRenderBuffer(128)
		, myGPUContext(aGPUContext)
		, myAssetContainer(aAssetContainer)
	{
		InitFullscreenQuad(myAssetContainer.LoadEffect("Data/Resource/Shader/S_effect_fullscreen.fx"), myGPUContext, myAssetContainer);
		myGPUContext.GetBackbuffer(myBackbuffer);

		CreateDepthStencilStates();
		CreateRasterizerStates();
	}


	Renderer::~Renderer()
	{
	}

	void Renderer::AddRenderCommand(ModelID aModelID, EffectID aEffectID
		, const CU::Matrix44<float>& aOrientation, const CU::Vector3<float>& aScale)
	{
		myRenderBuffer.Add(RenderCommand(aModelID, aEffectID, aOrientation, aScale));
	}


	void Renderer::RenderModels(const Camera& aCamera)
	{
		for each (const RenderCommand& command in myRenderBuffer)
		{
			SetEffect(command.myEffectID);
			SetMatrix("ViewProjection", aCamera.GetViewProjection());
			SetMatrix("World", command.myOrientation);
			SetVector("CameraPosition", aCamera.GetPosition());
			SetVector("Scale", command.myScale);

			RenderModel(command.myModelID);
		}

		myRenderBuffer.RemoveAll();
	}

	void Renderer::SetEffect(EffectID aEffect)
	{
		myCurrentEffect = aEffect;
	}

	void Renderer::SetTexture(const CU::String<64>& aName, Texture* aTexture)
	{
		ID3DX11EffectVariable* var = GetEffectVariable(aName);
		var->AsShaderResource()->SetResource(aTexture->GetShaderView());
	}

	void Renderer::SetMatrix(const CU::String<64>& aName, const CU::Matrix44<float>& aMatrix)
	{
		ID3DX11EffectVariable* var = GetEffectVariable(aName);
		var->AsMatrix()->SetMatrix(&aMatrix.myMatrix[0]);
	}

	void Renderer::SetVector(const CU::String<64>& aName, const CU::Vector3<float>& aVector)
	{
		ID3DX11EffectVariable* var = GetEffectVariable(aName);
		var->AsVector()->SetFloatVector(&aVector.x);
	}

	void Renderer::SetVector(const CU::String<64>& aName, const CU::Vector4<float>& aVector)
	{
		ID3DX11EffectVariable* var = GetEffectVariable(aName);
		var->AsVector()->SetFloatVector(&aVector.x);
	}

	void Renderer::SetRawData(const CU::String<64>& aName, int aDataSize, const void* someData)
	{
		ID3DX11EffectVariable* var = GetEffectVariable(aName);
		var->SetRawValue(someData, 0, aDataSize);
	}

	void Renderer::SetClearColor(const CU::Vector4<float>& aColor)
	{
		myClearColor[0] = aColor.x;
		myClearColor[1] = aColor.y;
		myClearColor[2] = aColor.z;
		myClearColor[3] = aColor.w;
	}

	void Renderer::SetRasterizerState(eRasterizer aState)
	{
		myGPUContext.GetContext()->RSSetState(myRasterizerStates[aState]);
	}

	void Renderer::SetDepthStencilState(eDepthState aState)
	{
		myGPUContext.GetContext()->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
	}

	void Renderer::AddRenderTarget(Texture* aTexture)
	{
		DL_ASSERT_EXP(myRenderTargetCount < 4, CU::Concatenate<256>("Added to many RenderTargets, only %i supported", 4));
		myRenderTargets[myRenderTargetCount] = aTexture->GetRenderTarget();
		++myRenderTargetCount;
	}

	void Renderer::ClearRenderTarget(Texture* aTexture)
	{
		myGPUContext.GetContext()->ClearRenderTargetView(aTexture->GetRenderTarget(), myClearColor);
	}

	void Renderer::SetDepthStencil(Texture* aTexture)
	{
		myDepthStencil = aTexture->GetDepthStencil();
	}

	void Renderer::ClearDepthStencil(Texture* aTexture)
	{
		myGPUContext.GetContext()->ClearDepthStencilView(aTexture->GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void Renderer::UseOriginalDepthStencil()
	{
		myDepthStencil = myBackbuffer.myDepthStencilView;
	}

	void Renderer::UseOriginalRenderTarget()
	{
		DL_ASSERT_EXP(myRenderTargetCount < 4, CU::Concatenate<256>("Added to many RenderTargets, only %i supported", 4));
		myRenderTargets[myRenderTargetCount] = myBackbuffer.myBackbufferTarget;
		++myRenderTargetCount;
	}

	void Renderer::ApplyRenderTargetAndDepthStencil()
	{
		myGPUContext.GetContext()->OMSetRenderTargets(myRenderTargetCount, myRenderTargets, myDepthStencil);
		myRenderTargetCount = 0;
	}

	void Renderer::RenderFullScreen(const CU::String<30>& aTechnique)
	{
		ActivateFullscreenQuad(myGPUContext);
		RenderFullscreenQuad(myCurrentEffect, aTechnique, myGPUContext, myAssetContainer);
	}

	void Renderer::RenderModel(ModelID aModelID)
	{
		ModelData* modelData = myAssetContainer.GetModel(aModelID);
		RenderModelData(*modelData);
	}

	ID3DX11EffectVariable* Renderer::GetEffectVariable(const CU::String<64>& aName)
	{
		if (myEffectVariables[myCurrentEffect].KeyExists(aName) == false)
		{
			Effect* effect = myAssetContainer.GetEffect(myCurrentEffect);

			DL_ASSERT_EXP(effect != nullptr, "Cant GetEffectVariable without an Effect");
			ID3DX11EffectVariable* var = effect->GetEffect()->GetVariableByName(aName.c_str());
			DL_ASSERT_EXP(var->IsValid() == TRUE, CU::Concatenate<256>("ShaderVar: %s not found", aName.c_str()));
			
			myEffectVariables[myCurrentEffect][aName] = var;
		}

		return myEffectVariables[myCurrentEffect][aName];
	}

	void Renderer::RenderModelData(const ModelData& someData)
	{
		if (someData.myIsNullObject == false)
		{
			const GPUData& gpuData = *someData.myGPUData;
			for (int i = 0; i < gpuData.myTextures.Size(); ++i)
			{
				SetTexture(gpuData.myShaderResourceNames[i], gpuData.myTextures[i]);
			}

			RenderGPUData(gpuData);
		}

		for (ModelData* child : someData.myChildren)
		{
			RenderModelData(*child);
		}
	}

	void Renderer::RenderGPUData(const GPUData& someData)
	{
		ID3D11DeviceContext* context = myGPUContext.GetContext();

		const unsigned int byteOffset = 0;

		context->IASetInputLayout(someData.myInputLayout);
		context->IASetVertexBuffers(0, 1, &someData.myVertexBuffer->myVertexBuffer, &someData.myVertexBuffer->myStride, &byteOffset);
		context->IASetIndexBuffer(someData.myIndexBuffer->myIndexBuffer, DXGI_FORMAT(someData.myIndexBuffer->myIndexBufferFormat), byteOffset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY(someData.myPrimitiveTopology));

		Effect* effect = myAssetContainer.GetEffect(myCurrentEffect);
		D3DX11_TECHNIQUE_DESC techDesc;
		effect->GetTechnique(someData.GetTechniqueName())->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			effect->GetTechnique(someData.GetTechniqueName())->GetPassByIndex(i)->Apply(0, context);
			context->DrawIndexed(someData.myIndexData->myNumberOfIndices, 0, 0);
		}
	}

	void Renderer::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = false;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = false;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;


		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_BACK;
		myGPUContext.GetDevice()->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(eRasterizer::WIRE_FRAME)]);


		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		myGPUContext.GetDevice()->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(eRasterizer::CULL_BACK)]);

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		myGPUContext.GetDevice()->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(eRasterizer::NO_CULLING)]);
	}

	void Renderer::CreateDepthStencilStates()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		HRESULT hr = myGPUContext.GetDevice()->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::ENABLED)]);
		DL_ASSERT_EXP(FAILED(hr) == false, "Failed to create ENABLED depthstate");

		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		hr = myGPUContext.GetDevice()->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::DISABLED)]);
		DL_ASSERT_EXP(FAILED(hr) == false, "Failed to create DISABLED depthstate");

		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;
		hr = myGPUContext.GetDevice()->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::NO_READ_NO_WRITE)]);
		DL_ASSERT_EXP(FAILED(hr) == false, "Failed to create NO_READ_NO_WRITE depthstate");

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		hr = myGPUContext.GetDevice()->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::READ_NO_WRITE)]);
		DL_ASSERT_EXP(FAILED(hr) == false, "Failed to create READ_NO_WRITE depthstate");
	}

}