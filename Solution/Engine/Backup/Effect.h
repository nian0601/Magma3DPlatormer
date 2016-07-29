#pragma once

#include <CUString.h>

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectShaderResourceVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectScalarVariable;

struct ID3D11ShaderResourceView;

namespace Magma
{
	class GPUContext;
	class Texture;

	class Effect
	{
	public:
		Effect();
		~Effect();

		void Init(const CU::String<64>& aFilePath, GPUContext& aGPUContext);

		ID3DX11Effect* GetEffect() const;
		ID3DX11EffectTechnique* GetTechnique(const CU::String<30>& aTechniqueName) const;


		void SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix);
		void SetViewMatrix(const CU::Matrix44<float>& aViewMatrix);
		void SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix);
		void SetViewProjectionMatrix(const CU::Matrix44<float>& aMatrix);

		void SetAlbedoMetalness(ID3D11ShaderResourceView* aResource);
		void SetNormalRoughness(ID3D11ShaderResourceView* aResource);
		void SetDepth(ID3D11ShaderResourceView* aResource);
		void SetCubemap(ID3D11ShaderResourceView* aResource);

		void SetCameraPosition(const CU::Vector3<float>& aVector);

		void SetMetalness(float aValue);
		void SetRoughness(float aValue);

	private:
		void LoadMatrix(ID3DX11EffectMatrixVariable*& aMatrix, const CU::String<30>& aVariableName, bool aForceFind = true);
		void LoadShaderResource(ID3DX11EffectShaderResourceVariable*& aResource, const CU::String<30>& aVariableName, bool aForceFind = true);
		void LoadVector(ID3DX11EffectVectorVariable*& aVector, const CU::String<30>& aVariableName, bool aForceFind = true);
		void LoadScalar(ID3DX11EffectScalarVariable*& aScalar, const CU::String<30>& aVariableName, bool aForceFind = true);
		void CheckVariable(const CU::String<30>& aName, const void* aVariable);

		ID3DX11Effect* myEffect;

		ID3DX11EffectMatrixVariable* myProjectionMatrix;
		ID3DX11EffectMatrixVariable* myViewMatrix;
		ID3DX11EffectMatrixVariable* myWorldMatrix;
		ID3DX11EffectMatrixVariable* myViewProjectionMatrix;

		ID3DX11EffectShaderResourceVariable* myAlbedoMetalness;
		ID3DX11EffectShaderResourceVariable* myNormalRoughness;
		ID3DX11EffectShaderResourceVariable* myDepth;
		ID3DX11EffectShaderResourceVariable* myCubemap;

		ID3DX11EffectVectorVariable* myCameraPosition;

		ID3DX11EffectScalarVariable* myMetalness;
		ID3DX11EffectScalarVariable* myRoughness;

		CU::String<64> myFileName;
		CU::String<64> myFilePath;
	};
}