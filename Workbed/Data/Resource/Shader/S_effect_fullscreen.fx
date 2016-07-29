#include "S_effect_deferred_ambient.fx"

Pixel_FullscreenQuad VertexShader_FullscreenQuad(Vertex_FullscreenQuad aInput)
{
	Pixel_FullscreenQuad output = (Pixel_FullscreenQuad)0;

	output.Tex = aInput.Tex;
	output.Pos = aInput.Pos;

	return output;
}

float4 PixelShader_FullscreenQuad(Pixel_FullscreenQuad aInput) : SV_Target
{
	return float4(0.75f, 0.33f, 0.25f, 1.f);
}


technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VertexShader_FullscreenQuad()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PixelShader_FullscreenQuad()));
	}
}

technique11 Deferred_Ambient
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VertexShader_FullscreenQuad()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PixelShader_Deferred_Ambient()));
	}
}