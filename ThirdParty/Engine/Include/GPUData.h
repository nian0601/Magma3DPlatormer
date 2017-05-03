#pragma once

#include <CUString.h>
#include <GrowingArray.h>
#include "Types.h"

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;


namespace Magma
{
	struct VertexBuffer;
	struct VertexData;
	struct IndexBuffer;
	struct IndexData;

	class AssetContainer;
	class GPUContext;
	class Texture;

	class GPUData
	{
		friend class FBXFactory;
		friend class Renderer;
	public:
		GPUData();
		~GPUData();

		void Init(EffectID aEffect, GPUContext& aGPUContext, AssetContainer& aAssetContainer);
		void Init(EffectID aEffect, int aIndexCount, char* aIndexData
			, int aVertexCount, int aVertexStride, char* aVertexData, GPUContext& aGPUContext, AssetContainer& aAssetContainer);
		void InitWithoutBufferSetup(EffectID aEffect, int aVertexStride, GPUContext& aGPUContext, AssetContainer& aAssetContainer);
		void SetTopology(int aTopology);
		void AddInputElement(D3D11_INPUT_ELEMENT_DESC* aElement);

		const VertexBuffer& GetVertexBuffer() const;
		const IndexBuffer& GetIndexBuffer() const;
		ID3D11InputLayout* GetInputLayout() const;
		int GetTopology() const;

		const CU::String& GetTechniqueName() const;

		void UpdateBuffers(int aIndexCount, char* aIndexData, int aVertexCount, int aVertexStride, char* aVertexData, GPUContext& aGPUContext);
	private:
		void InitInputLayout(D3D11_INPUT_ELEMENT_DESC* aVertexDescArray, int aArraySize, EffectID aEffect, GPUContext& aGPUContext, AssetContainer& aAssetContainer);
		void InitVertexBuffer(int aVertexSize, int aBufferUsage, int aCPUUsage);
		void InitIndexBuffer();

		void SetupVertexBuffer(int aVertexCount, char* aVertexData, GPUContext& aGPUContext);
		void SetupIndexBuffer(int aIndexCount, char* aIndexData, GPUContext& aGPUContext);

		VertexBuffer* myVertexBuffer;
		VertexData* myVertexData;
		IndexBuffer* myIndexBuffer;
		IndexData* myIndexData;
		ID3D11InputLayout* myInputLayout;
		int myPrimitiveTopology;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC*> myVertexFormat;

		CU::GrowingArray<CU::String> myShaderResourceNames;
		CU::GrowingArray<Texture*> myTextures;
		CU::String myTechniqueName;
	};

	inline const VertexBuffer& GPUData::GetVertexBuffer() const
	{
		return *myVertexBuffer;
	}

	inline const IndexBuffer& GPUData::GetIndexBuffer() const
	{
		return *myIndexBuffer;
	}

	inline ID3D11InputLayout* GPUData::GetInputLayout() const
	{
		return myInputLayout;
	}

	inline int GPUData::GetTopology() const
	{
		return myPrimitiveTopology;
	}

	inline const CU::String& GPUData::GetTechniqueName() const
	{
		return myTechniqueName;
	}
}