#include "stdafx.h"

#include <CommonHelper.h>
#include <d3dx11effect.h>
#include <D3D10effect.h>
#include <D3DX11async.h>
#include <D3Dcompiler.h>
#include "Effect.h"
#include "GPUContext.h"
#include "Texture.h"

namespace Magma
{
	Effect::Effect()
		: myEffect(nullptr)
		, myWorldMatrix(nullptr)
		, myViewMatrix(nullptr)
		, myProjectionMatrix(nullptr)
		, myViewProjectionMatrix(nullptr)
		, myCubemap(nullptr)
		, myCameraPosition(nullptr)
		, myFileName("NOT INITIALIZED")
	{
	}

	Effect::~Effect()
	{
		SAFE_RELEASE(myEffect);
	}

	void Effect::Init(const CU::String<64>& aFilePath, GPUContext& aGPUContext)
	{
		myFileName = CU::GetFileNameFromFilePath(aFilePath);
		myFilePath = aFilePath;

		unsigned int shaderFlags = 0;
#ifdef _DEBUG
		shaderFlags |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* compilerMsg = nullptr;

		HRESULT result = D3DX11CompileFromFile(myFilePath.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilerMsg, 0);

		if (FAILED(result))
		{
			CU::String<100> errorMessage = "";

			if (compilerMsg != nullptr)
			{
				DL_MESSAGE_BOX((char*)compilerMsg->GetBufferPointer(), "Effect Error", MB_ICONWARNING);

				errorMessage = "[Effect]: Failed to compile";
			}
			else
			{
				errorMessage = "[Effect]: Could not find the effect ";
				errorMessage += myFileName.c_str();
			}

			DL_ASSERT(errorMessage.c_str());
		}

		if (compilerMsg)
		{
			SAFE_RELEASE(compilerMsg);
		}

		result = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, aGPUContext.GetDevice(), &myEffect);
		DL_ASSERT_EXP(SUCCEEDED(result) == TRUE, "Failed to Create shader");

		if (compiledShader)
		{
			SAFE_RELEASE(compiledShader);
		}

		LoadMatrix(myWorldMatrix, "World");
		LoadMatrix(myViewMatrix, "View");
		LoadMatrix(myProjectionMatrix, "Projection");
		LoadMatrix(myViewProjectionMatrix, "ViewProjection");

		LoadShaderResource(myCubemap, "Cubemap", false);
		LoadShaderResource(myAlbedoMetalness, "AlbedoMetalnessTexture", false);
		LoadShaderResource(myNormalRoughness, "NormalRoughnessTexture", false);
		LoadShaderResource(myDepth, "DepthTexture", false);

		LoadVector(myCameraPosition, "CameraPosition");

		LoadScalar(myMetalness, "GlobalMetalness");
		LoadScalar(myRoughness, "GlobalRoughness");
	}

	ID3DX11Effect* Effect::GetEffect() const
	{
		return myEffect;
	}

	ID3DX11EffectTechnique* Effect::GetTechnique(const CU::String<30>& aTechniqueName) const
	{
		return myEffect->GetTechniqueByName(aTechniqueName.c_str());
	}

	void Effect::SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix)
	{
		CheckVariable("WorldMatrix", myWorldMatrix);
		myWorldMatrix->SetMatrix(&aWorldMatrix.myMatrix[0]);
	}

	void Effect::SetViewMatrix(const CU::Matrix44<float>& aViewMatrix)
	{
		CheckVariable("ViewMatrix", myViewMatrix);
		myViewMatrix->SetMatrix(&aViewMatrix.myMatrix[0]);
	}

	void Effect::SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix)
	{
		CheckVariable("ProjectionMatrix", myProjectionMatrix);
		myProjectionMatrix->SetMatrix(&aProjectionMatrix.myMatrix[0]);
	}

	void Effect::SetViewProjectionMatrix(const CU::Matrix44<float>& aMatrix)
	{
		CheckVariable("ViewProjectionMatrix", myViewProjectionMatrix);
		myViewProjectionMatrix->SetMatrix(&aMatrix.myMatrix[0]);
	}


	void Effect::SetAlbedoMetalness(ID3D11ShaderResourceView* aResource)
	{
		CheckVariable("AlbedoMetalnessTexture", myAlbedoMetalness);
		myAlbedoMetalness->SetResource(aResource);
	}

	void Effect::SetNormalRoughness(ID3D11ShaderResourceView* aResource)
	{
		CheckVariable("NormalRoughnessTexture", myNormalRoughness);
		myNormalRoughness->SetResource(aResource);
	}

	void Effect::SetDepth(ID3D11ShaderResourceView* aResource)
	{
		CheckVariable("DepthTexture", myDepth);
		myDepth->SetResource(aResource);
	}

	void Effect::SetCubemap(ID3D11ShaderResourceView* aResource)
	{
		CheckVariable("Cubemap", myCubemap);
		myCubemap->SetResource(aResource);
	}

	void Effect::SetCameraPosition(const CU::Vector3<float>& aVector)
	{
		CheckVariable("CameraPosition", myCameraPosition);
		myCameraPosition->SetFloatVector(&aVector.x);
	}

	void Effect::SetMetalness(float aValue)
	{
		CheckVariable("Metalness", myMetalness);
		myMetalness->SetFloat(aValue);
	}

	void Effect::SetRoughness(float aValue)
	{
		CheckVariable("Roughness", myRoughness);
		myRoughness->SetFloat(aValue);
	}

	void Effect::LoadMatrix(ID3DX11EffectMatrixVariable*& aMatrix, const CU::String<30>& aVariableName, bool aForceFind)
	{
		aMatrix = myEffect->GetVariableByName(aVariableName.c_str())->AsMatrix();
		if (aMatrix->IsValid() == false)
		{
			aMatrix = nullptr;
			if (aForceFind == true)
			{
				DL_ASSERT(CU::Concatenate<256>("Shader: Failed to find variable: %s, in file: %s", aVariableName.c_str(), myFileName.c_str()));
			}
		}
	}

	void Effect::LoadShaderResource(ID3DX11EffectShaderResourceVariable*& aResource, const CU::String<30>& aVariableName, bool aForceFind)
	{
		aResource = myEffect->GetVariableByName(aVariableName.c_str())->AsShaderResource();
		if (aResource->IsValid() == false)
		{
			aResource = nullptr;
			if (aForceFind == true)
			{
				DL_ASSERT(CU::Concatenate<256>("Shader: Failed to find variable: %s, in file: %s", aVariableName.c_str(), myFileName.c_str()));
			}
		}
	}

	void Effect::LoadVector(ID3DX11EffectVectorVariable*& aVector, const CU::String<30>& aVariableName, bool aForceFind /*= true*/)
	{
		aVector = myEffect->GetVariableByName(aVariableName.c_str())->AsVector();
		if (aVector->IsValid() == false)
		{
			aVector = nullptr;
			if (aForceFind == true)
			{
				DL_ASSERT(CU::Concatenate<256>("Shader: Failed to find variable: %s, in file: %s", aVariableName.c_str(), myFileName.c_str()));
			}
		}
	}

	void Effect::LoadScalar(ID3DX11EffectScalarVariable*& aScalar, const CU::String<30>& aVariableName, bool aForceFind /*= true*/)
	{
		aScalar = myEffect->GetVariableByName(aVariableName.c_str())->AsScalar();
		if (aScalar->IsValid() == false)
		{
			aScalar = nullptr;
			if (aForceFind == true)
			{
				DL_ASSERT(CU::Concatenate<256>("Shader: Failed to find variable: %s, in file: %s", aVariableName.c_str(), myFileName.c_str()));
			}
		}
	}

	void Effect::CheckVariable(const CU::String<30>& aName, const void* aVariable)
	{
		DL_ASSERT_EXP(aVariable != nullptr, CU::Concatenate<256>("Variable: %s not found in shader: %s", aName.c_str(), myFileName.c_str()).c_str());
	}
}