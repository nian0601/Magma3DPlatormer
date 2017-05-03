#pragma once

#include "Backbuffer.h"
#include <CUMap.h>
#include <CUString.h>
#include "QuadRenderer.h"
#include <Matrix.h>
#include "ModelData.h"
#include <Vector.h>
#include <GrowingArray.h>
#include "TextData.h"

struct ID3DX11EffectVariable;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;

namespace Magma
{
	class AssetContainer;
	class Camera;
	class Effect;
	class GPUContext;
	class Texture;

	class Font;

	enum eDepthState
	{
		ENABLED,
		DISABLED,
		READ_NO_WRITE,
		NO_READ_NO_WRITE,
		_DEPTH_COUNT
	};

	enum eRasterizer
	{
		CULL_BACK,
		WIRE_FRAME,
		NO_CULLING,
		_RAZTER_COUNT
	};

	enum eBlendState
	{
		NO_BLEND,
		ALPHA_BLEND,
		_BLEND_COUNT
	};

	class Renderer
	{
	public:
		Renderer(AssetContainer& aAssetContainer, GPUContext& aGPUContext);
		~Renderer();

		void AddModelCommand(ModelID myModelID, EffectID aEffectID
			, const CU::Matrix44<float>& aOrientation, const CU::Vector3<float>& aScale);
		void AddSpriteCommand(Texture* aTexture, const CU::Matrix44<float>& aOrientation
			, const CU::Vector4<float>& aSizeAndHotSpot, const CU::Vector4<float>& aPositionAndScale);
		void AddTextCommand(const CU::String& aString, const CU::Vector2<float>& aPosition);
		void AddTextCommand(Font* aFont, const CU::String& aString, const CU::Vector2<float>& aPosition);
		void RenderModels(const Camera& aCamera);
		void RenderSprites(const Camera& aCamera);
		void RenderText(const Camera& aCamera);

		void SetEffect(EffectID aEffect);
		void SetTexture(const CU::String& aName, Texture* aTexture);
		void SetMatrix(const CU::String& aName, const CU::Matrix44<float>& aMatrix);
		void SetVector(const CU::String& aName, const CU::Vector3<float>& aVector);
		void SetVector(const CU::String& aName, const CU::Vector4<float>& aVector);
		void SetRawData(const CU::String& aName, int aDataSize, const void* someData);


		void SetTexture(EffectVariableID aVariableID, Texture* aTexture);
		void SetMatrix(EffectVariableID aVariableID, const CU::Matrix44<float>& aMatrix);
		void SetVector(EffectVariableID aVariableID, const CU::Vector3<float>& aVector);
		void SetVector(EffectVariableID aVariableID, const CU::Vector4<float>& aVector);
		void SetRawData(EffectVariableID aVariableID, int aDataSize, const void* someData);

		void SetClearColor(const CU::Vector4<float>& aColor);

		void SetRasterizerState(eRasterizer aState);
		void SetDepthStencilState(eDepthState aState);
		void SetBlendState(eBlendState aState);

		void AddRenderTarget(Texture* aTexture);
		void ClearRenderTarget(Texture* aTexture);
		void SetDepthStencil(Texture* aTexture);
		void ClearDepthStencil(Texture* aTexture);
		void UseOriginalDepthStencil();
		void UseOriginalRenderTarget();
		void ApplyRenderTargetAndDepthStencil();

		void RenderFullScreen(const CU::String& aTechnique);
		void RenderModel(ModelID aModelID);

		void RenderGPUData(const GPUData& someData);
	private:
		void operator=(Renderer&) = delete;

		ID3DX11EffectVariable* GetEffectVariable(const CU::String& aName);
		ID3DX11EffectVariable* GetEffectVariable(EffectVariableID aEffectVariable);
		void RenderModelData(const ModelData& someData);

		void CreateRasterizerStates();
		void CreateDepthStencilStates();
		void CreateBlendStates();

		void ClearShaderVariables(EffectID aEffectID);

		EffectID myCurrentEffect;
		float myClearColor[4];

		int myRenderTargetCount;
		ID3D11RenderTargetView* myRenderTargets[4];
		ID3D11DepthStencilView* myDepthStencil;
		Backbuffer myBackbuffer;
		GPUContext& myGPUContext;

		CU::Map<EffectID, CU::Map<CU::String, ID3DX11EffectVariable*>> myEffectVariables;
		CU::Map<EffectID, CU::Map<EffectVariableID, ID3DX11EffectVariable*>> myEffectVariablesIds;
		CU::Map<CU::String, ID3DX11EffectVariable*>* myCurrentEffectVariables;
		CU::Map<EffectVariableID, ID3DX11EffectVariable*>* myCurrentEffectVariablesIds;

		ID3D11RasterizerState* myRasterizerStates[static_cast<int>(eRasterizer::_RAZTER_COUNT)];
		ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];
		ID3D11BlendState* myBlendStates[static_cast<int>(eBlendState::_BLEND_COUNT)];

		AssetContainer& myAssetContainer;
		QuadRenderer myQuadRenderer;

		struct ModelCommand
		{
			ModelCommand()
			{}
			ModelCommand(ModelID aModelID, EffectID aEffectID
				, const CU::Matrix44<float>& aOrientation, const CU::Vector3<float>& aScale)
				: myModelID(aModelID)
				, myEffectID(aEffectID)
				, myOrientation(aOrientation)
				, myScale(aScale)
			{}
			CU::Matrix44<float> myOrientation;
			CU::Vector3<float> myScale;
			Magma::ModelID myModelID;
			Magma::EffectID myEffectID;
		};
		CU::GrowingArray<ModelCommand> myModelCommands;
	};
}