#pragma once

struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace Magma
{
	struct Backbuffer
	{
		ID3D11Texture2D* myBackbufferTexture;
		ID3D11RenderTargetView* myBackbufferTarget;
		ID3D11ShaderResourceView* myBackbufferView;

		ID3D11Texture2D* myDepthStencilTexture;
		ID3D11DepthStencilView* myDepthStencilView;
	};
}