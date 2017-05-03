#pragma once
#include "Macros.h"

struct ID3D11Buffer;
struct D3D11_BUFFER_DESC;

namespace Magma
{
	struct VertexBuffer
	{
		VertexBuffer()
			: myVertexBuffer(nullptr)
		{}

		unsigned int myStride;
		ID3D11Buffer* myVertexBuffer;
		D3D11_BUFFER_DESC* myBufferDesc;
	};

	struct VertexData
	{
		VertexData()
			: myVertexData(nullptr)
		{}

		~VertexData()
		{
			delete myVertexData;
		}

		int myNumberOfVertices;
		int mySize;
		int myStride;
		char* myVertexData;
	};



	struct IndexBuffer
	{
		IndexBuffer()
			: myIndexBuffer(nullptr)
			, myIndexBufferFormat(0)
		{}

		unsigned int myIndexBufferFormat;
		ID3D11Buffer* myIndexBuffer;
		D3D11_BUFFER_DESC* myBufferDesc;
	};

	struct IndexData
	{
		IndexData()
			: myIndexData(nullptr)
		{}
		~IndexData()
		{
			SAFE_DELETE(myIndexData);
		}

		unsigned int myFormat;
		char* myIndexData;
		int myNumberOfIndices;
		int mySize;
	};


	struct VertexPosColor
	{
		CU::Vector4<float> myPos;
		CU::Vector4<float> myCol;
	};

	struct VertexPosUV
	{
		CU::Vector3<float> myPos;
		CU::Vector2<float> myUV;
	};
}