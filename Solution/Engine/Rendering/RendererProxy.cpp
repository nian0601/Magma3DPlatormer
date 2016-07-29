#include "stdafx.h"

#include "Renderer.h"
#include "RendererProxy.h"

namespace Magma
{
	RendererProxy::RendererProxy(Renderer& aRenderer)
		: myRenderer(aRenderer)
	{
	}


	RendererProxy::~RendererProxy()
	{
	}

	void RendererProxy::RenderModel(ModelID aModelID, EffectID aEffectID
		, const CU::Matrix44<float>& aOrientation, const CU::Vector3<float>& aScale)
	{
		myRenderer.AddRenderCommand(aModelID, aEffectID, aOrientation, aScale);
	}
}
