#include "S_u_variables.fx"

Matrix Orientation;
float4 SizeAndHotSpot;
float4 PositionAndScale;

static const float OUTLINE_MIN_VALUE0 = 0.3;
static const float OUTLINE_MIN_VALUE1 = 0.4;
static const float OUTLINE_MAX_VALUE0 = 0.6;
static const float OUTLINE_MAX_VALUE1 = 0.7;
static const float4 OUTLINE_COLOR = {0.f, 0.f, 0.f, 1.f};
static const bool USE_OUTLINE = true;

static const float SMOOTHING = 1.0/32.0;

Pixel_Quad VertexShader_Sprite(Vertex_Quad aInput)
{
	float4 finalPosition = aInput.Pos;

	/*

	finalPosition.xy *= size;
	finalPosition.xy += size;
	finalPosition.xy -= hotspot;
	
	*/

	finalPosition.xy *= PositionAndScale.zw;
	finalPosition.xy += PositionAndScale.xy;

	finalPosition = mul(finalPosition, Projection);

	finalPosition.x -= 1;
	finalPosition.y += 1;

	Pixel_Quad output = (Pixel_Quad)0;
	output.Pos = finalPosition;
	output.Tex = aInput.Tex;
		
	return output;
}

float4 PixelShader_Sprite(Pixel_Quad aInput) : SV_Target
{
	float4 texColor = {1.f, 1.f, 1.f, 1.f};
	float distAlphaMask = AlbedoTexture.SampleLevel(linearSampling, aInput.Tex, 0).a;

	if(USE_OUTLINE &&
		(distAlphaMask >= OUTLINE_MIN_VALUE0 && distAlphaMask <= OUTLINE_MAX_VALUE1))
	{
		float oFactor = 1.0;
		if(distAlphaMask <= OUTLINE_MIN_VALUE1)
		{
			oFactor = smoothstep(OUTLINE_MIN_VALUE0, OUTLINE_MIN_VALUE1, distAlphaMask);
		}
		else
		{
			oFactor = smoothstep(OUTLINE_MAX_VALUE1, OUTLINE_MAX_VALUE0, distAlphaMask);
		}

		texColor = lerp(texColor, OUTLINE_COLOR, oFactor);
	}

	float alpha = smoothstep(0.55 - SMOOTHING, 0.55 + SMOOTHING, distAlphaMask);
	
	texColor.a = alpha;
	//return float4(1.f, 1.f, 1.f, alpha);

	return texColor;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VertexShader_Sprite()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PixelShader_Sprite()));
	}
}