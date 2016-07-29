#include "stdafx.h"

#include "AssetContainer.h"
#include <CommonHelper.h>
#include <d3d11.h>
#include "FbxFactory.h"
#include "FBX/FbxLoader.h"
#include "Matrix44.h"

#include "ModelData.h"
#include "GPUData.h"


namespace Magma
{
	FBXFactory::FBXFactory(AssetContainer& aAssetContainer, GPUContext& aGPUContext)
		: myAssetContainer(aAssetContainer)
		, myGPUContext(aGPUContext)
	{
		myLoader = new FBX::FBXLoader();
	}

	FBXFactory::~FBXFactory()
	{
		for (auto it = myModels.Begin(); it != myModels.End(); it = myModels.Next(it))
		{
			SAFE_DELETE(it.Second());
		}
		myModels.Clear();

		SAFE_DELETE(myLoader);
	}

	ModelData* FBXFactory::LoadModel(const CU::String<64>& aFilePath, EffectID aEffect)
	{
		if (myModels.KeyExists(aFilePath) == true)
		{
			return myModels[aFilePath];
		}

		CU::GrowingArray<CU::String<256>> errors(16);
		FBX::FbxModelData* fbxModelData = myLoader->loadModel(aFilePath.c_str(), errors);

		ModelData* modelData = CreateModel(fbxModelData);
		myModels[aFilePath] = modelData;

		modelData->Init(aEffect, myGPUContext, myAssetContainer);
		return modelData;
	}

	ModelData* FBXFactory::CreateModel(FBX::FbxModelData* someModelData)
	{
		ModelData* modelData = new ModelData();

		if (someModelData->myData)
		{
			modelData->myIsNullObject = false;
			modelData->myOrientation = someModelData->myOrientation;

			modelData->myGPUData = new GPUData();
			GPUData& gpuData = *modelData->myGPUData;
			gpuData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			gpuData.myIndexData = new IndexData();
			gpuData.myVertexData = new VertexData();

			LoadData(gpuData, someModelData->myData);
		}

		for (int i = 0; i < someModelData->myChildren.Size(); ++i)
		{
			auto currentChild = someModelData->myChildren[i];
			modelData->AddChild(CreateModel(currentChild));
		}
		return modelData;
	}

	void FBXFactory::LoadData(GPUData& someGPUData, FBX::ModelData* someFBXData)
	{
		unsigned int* rawIndexData = new unsigned int[someFBXData->myIndexCount];
		memcpy(rawIndexData, someFBXData->myIndicies, someFBXData->myIndexCount*sizeof(unsigned int));

		IndexData* indexData = someGPUData.myIndexData;
		indexData->myFormat = DXGI_FORMAT_R32_UINT;
		indexData->myIndexData = (char*)rawIndexData;
		indexData->mySize = someFBXData->myIndexCount*sizeof(unsigned int);
		indexData->myNumberOfIndices = someFBXData->myIndexCount;

		int sizeOfBuffer = someFBXData->myVertexCount*someFBXData->myVertexStride*sizeof(float);
		char* rawVertexData = new char[sizeOfBuffer];
		memcpy(rawVertexData, someFBXData->myVertexBuffer, sizeOfBuffer);

		VertexData* vertexData = someGPUData.myVertexData;
		vertexData->myVertexData = rawVertexData;
		vertexData->myNumberOfVertices = someFBXData->myVertexCount;
		vertexData->mySize = sizeOfBuffer;
		vertexData->myStride = someFBXData->myVertexStride*sizeof(float);

		for (int i = 0; i < someFBXData->myLayout.Size(); ++i)
		{
			auto currentLayout = someFBXData->myLayout[i];
			D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC();
			desc->SemanticIndex = currentLayout.mySemanticIndex;
			desc->AlignedByteOffset = currentLayout.myOffset;
			desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc->InputSlot = 0;
			desc->InstanceDataStepRate = 0;

			if (currentLayout.myType == FBX::ModelData::VERTEX_POS)
			{
				desc->SemanticName = "POSITION";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (currentLayout.myType == FBX::ModelData::VERTEX_NORMAL)
			{
				desc->SemanticName = "NORMAL";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (currentLayout.myType == FBX::ModelData::VERTEX_UV)
			{
				desc->SemanticName = "TEXCOORD";
				desc->Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (currentLayout.myType == FBX::ModelData::VERTEX_BINORMAL)
			{
				desc->SemanticName = "BINORMAL";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (currentLayout.myType == FBX::ModelData::VERTEX_TANGENT)
			{
				desc->SemanticName = "TANGENT";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (currentLayout.myType == FBX::ModelData::VERTEX_SKINWEIGHTS)
			{
				desc->SemanticName = "WEIGHTS";
				desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			else if (currentLayout.myType == FBX::ModelData::VERTEX_BONEID)
			{
				desc->SemanticName = "BONES";
				desc->Format = DXGI_FORMAT_R32G32B32A32_UINT;
			}
			else if (currentLayout.myType == FBX::ModelData::VERTEX_COLOR)
			{
				desc->SemanticName = "COLOR";
				desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			someGPUData.myVertexFormat.Add(desc);
		}

		for (int i = 0; i < someFBXData->myTextures.Size(); ++i)
		{
			auto& currentTexture = someFBXData->myTextures[i];

			CU::String<80> resourceName;
			if (currentTexture.myType == FBX::ALBEDO)
			{
				resourceName = "AlbedoTexture";
			}
			if (currentTexture.myType == FBX::NORMAL)
			{
				resourceName = "NormalTexture";
			}
			if (currentTexture.myType == FBX::ROUGHNESS)
			{
				resourceName = "RoughnessTexture";
			}
			if (currentTexture.myType == FBX::METALNESS)
			{
				resourceName = "MetalnessTexture";
			}
			if (currentTexture.myType == FBX::AMBIENT)
			{
				resourceName = "AOTexture";
			}
			if (currentTexture.myType == FBX::FBXTextureType::EMISSIVE)
			{
				resourceName = "EmissiveTexture";
			}

			int dataIndex = currentTexture.myFileName.RFind("Data\\");
			CU::String<256> fromData = currentTexture.myFileName.SubStr(dataIndex, currentTexture.myFileName.Size());

			someGPUData.myShaderResourceNames.Add(resourceName.c_str());
			someGPUData.myTextures.Add(myAssetContainer.RequestTexture(fromData.c_str()));
		}
	}

}