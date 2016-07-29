#include "stdafx.h"

#include "AssetContainer.h"
#include <d3dx11effect.h>
#include <d3d11.h>
#include "Effect.h"
#include "GfxStructs.h"
#include "GPUContext.h"
#include "GPUData.h"


namespace Magma
{
	GPUData::GPUData()
		: myShaderResourceNames(8)
		, myTextures(8)
		, myVertexFormat(8)
		, myVertexBuffer(nullptr)
		, myVertexData(nullptr)
		, myIndexBuffer(nullptr)
		, myIndexData(nullptr)
	{
	}


	GPUData::~GPUData()
	{
		SAFE_DELETE(myIndexData);
		SAFE_DELETE(myVertexData);

		if (myVertexBuffer != nullptr && myVertexBuffer->myVertexBuffer != nullptr)
		{
			myVertexBuffer->myVertexBuffer->Release();
			delete myVertexBuffer;
		}

		if (myIndexBuffer != nullptr && myIndexBuffer->myIndexBuffer != nullptr)
		{
			myIndexBuffer->myIndexBuffer->Release();
			delete myIndexBuffer;
		}

		myVertexFormat.DeleteAll();
	}

	void GPUData::Init(EffectID aEffect, GPUContext& aGPUContext, AssetContainer& aAssetContainer)
	{
		const int size = myVertexFormat.Size();
		D3D11_INPUT_ELEMENT_DESC* vertexDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (int i = 0; i < myVertexFormat.Size(); ++i)
		{
			vertexDesc[i] = *myVertexFormat[i];
		}

		InitInputLayout(vertexDesc, size, aEffect, aGPUContext, aAssetContainer);
		delete[] vertexDesc;

		InitVertexBuffer(myVertexData->myStride, D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();

		SetupVertexBuffer(myVertexData->myNumberOfVertices, myVertexData->myVertexData, aGPUContext);
		SetupIndexBuffer(myIndexData->myNumberOfIndices, myIndexData->myIndexData, aGPUContext);

		myTechniqueName = "Render";
	}

	void GPUData::Init(EffectID aEffect, int aIndexCount, char* aIndexData
		, int aVertexCount, int aVertexStride, char* aVertexData, GPUContext& aGPUContext, AssetContainer& aAssetContainer)
	{
		const int size = myVertexFormat.Size();
		D3D11_INPUT_ELEMENT_DESC* vertexDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (int i = 0; i < myVertexFormat.Size(); ++i)
		{
			vertexDesc[i] = *myVertexFormat[i];
		}

		InitInputLayout(vertexDesc, size, aEffect, aGPUContext, aAssetContainer);
		delete[] vertexDesc;

		InitVertexBuffer(aVertexStride, D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();

		SetupVertexBuffer(aVertexCount, aVertexData, aGPUContext);
		SetupIndexBuffer(aIndexCount, aIndexData, aGPUContext);

		myTechniqueName = "Render";
	}

	void GPUData::SetTopology(int aTopology)
	{
		myPrimitiveTopology = aTopology;
	}

	void GPUData::AddInputElement(D3D11_INPUT_ELEMENT_DESC* aElement)
	{
		myVertexFormat.Add(aElement);
	}

	void GPUData::InitInputLayout(D3D11_INPUT_ELEMENT_DESC* aVertexDescArray, int aArraySize, EffectID aEffect, GPUContext& aGPUContext, AssetContainer& aAssetContainer)
	{
		Effect* effect = aAssetContainer.GetEffect(aEffect);

		D3DX11_PASS_DESC passDesc;
		effect->GetTechnique("Render")->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT hr = aGPUContext.GetDevice()->CreateInputLayout(aVertexDescArray
			, aArraySize, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myInputLayout);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "GPUData::Init", MB_ICONWARNING);
		}
	}

	void GPUData::InitVertexBuffer(int aVertexSize, int aBufferUsage, int aCPUUsage)
	{
		myVertexBuffer = new VertexBuffer();
		myVertexBuffer->myStride = aVertexSize;

		myVertexBuffer->myBufferDesc = new D3D11_BUFFER_DESC();
		ZeroMemory(myVertexBuffer->myBufferDesc, sizeof(*myVertexBuffer->myBufferDesc));
		myVertexBuffer->myBufferDesc->Usage = D3D11_USAGE(aBufferUsage);
		myVertexBuffer->myBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		myVertexBuffer->myBufferDesc->CPUAccessFlags = aCPUUsage;
		myVertexBuffer->myBufferDesc->MiscFlags = 0;
		myVertexBuffer->myBufferDesc->StructureByteStride = 0;
	}

	void GPUData::InitIndexBuffer()
	{
		myIndexBuffer = new IndexBuffer();
		myIndexBuffer->myIndexBufferFormat = DXGI_FORMAT_R32_UINT;

		myIndexBuffer->myBufferDesc = new D3D11_BUFFER_DESC();
		ZeroMemory(myIndexBuffer->myBufferDesc, sizeof(*myIndexBuffer->myBufferDesc));
		myIndexBuffer->myBufferDesc->Usage = D3D11_USAGE_IMMUTABLE;
		myIndexBuffer->myBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
		myIndexBuffer->myBufferDesc->CPUAccessFlags = 0;
		myIndexBuffer->myBufferDesc->MiscFlags = 0;
		myIndexBuffer->myBufferDesc->StructureByteStride = 0;
	}

	void GPUData::SetupVertexBuffer(int aVertexCount, char* aVertexData, GPUContext& aGPUContext)
	{
		if (myVertexBuffer->myVertexBuffer != nullptr)
		{
			SAFE_RELEASE(myVertexBuffer->myVertexBuffer);
		}

		myVertexBuffer->myBufferDesc->ByteWidth = myVertexBuffer->myStride * aVertexCount;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = aVertexData;

		DL_ASSERT_EXP(SUCCEEDED(aGPUContext.GetDevice()->CreateBuffer(myVertexBuffer->myBufferDesc, &initData
			, &myVertexBuffer->myVertexBuffer)) == TRUE, "GPUData::SetupVertexBuffer: Failed to SetupVertexBuffer");
	}

	void GPUData::SetupIndexBuffer(int aIndexCount, char* aIndexData, GPUContext& aGPUContext)
	{
		if (myIndexBuffer->myIndexBuffer != nullptr)
		{
			SAFE_RELEASE(myIndexBuffer->myIndexBuffer);
		}

		myIndexBuffer->myBufferDesc->ByteWidth = sizeof(UINT) * aIndexCount;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = aIndexData;

		DL_ASSERT_EXP(SUCCEEDED(aGPUContext.GetDevice()->CreateBuffer(myIndexBuffer->myBufferDesc, &initData,
			&myIndexBuffer->myIndexBuffer)) == TRUE, "GPUData::SetupIndexBuffer: Failed to SetupIndexBuffer");
	}

}