#include "S_u_variables.fx"

struct PointLightData
{
	float4 Position;
	float4 Color;
	float Range;
};

PointLightData PointLight[1];

Pixel_LightMesh VertexShader_PointLight(Vertex_LightMesh aInput)
{
	Pixel_LightMesh output = (Pixel_LightMesh)0;

	float range = PointLight[0].Range;
	float4 scale = float4(range, range, range, 1.0f);
	aInput.Position *= scale;
	
	aInput.Position.w = 1.0f;

	Matrix mat = mul(World, ViewProjection);
	output.Position = mul(aInput.Position, mat);

	float x = output.Position.x;
	float y = output.Position.y;
	float w = output.Position.w;

	output.Tex = float4((float2(x + w, w - y)) / 2, output.Position.zw);

	return output;
}

float4 PixelShader_PointLight(Pixel_LightMesh aInput) : SV_Target
{
	//return PointLight[0].Color;
	aInput.Tex /= aInput.Tex.w;

	PBLData data = CalculatePBLData_GBuffer(aInput.Tex.xy);
	//return float4(data.WorldPosition.xyz, 1.f);
	float3 viewPos = CameraPosition;
	float3 toEye = normalize(viewPos - data.WorldPosition.xyz);

	float3 toLight = PointLight[0].Position - data.WorldPosition;
	float3 lightDir = normalize(toLight);
	float3 halfVec = normalize(lightDir + toEye.xyz);
	float NdotL = saturate(dot(data.Normal.xyz, lightDir));
	float HdotN = saturate(dot(halfVec, data.Normal.xyz));
	float NdotV = saturate(dot(data.Normal.xyz, toEye));

	float3 F = saturate(Fresnel(data.Substance.xyz, lightDir, halfVec));
	float D = saturate(D_GGX(HdotN, (data.Roughness + 1.f) / 2.f));
	float V = saturate(SchlickForGGX((data.Roughness + 1.f) / 2.f, NdotV, NdotL));
	float lambert = NdotL;

	float attenuation = Attenuation(toLight, PointLight[0].Range);
	float3 lightColor = PointLight[0].Color.xyz * PointLight[0].Color.w * attenuation;
	float3 directSpecc = F * D * V  * lightColor;
	return float4(directSpecc, 1.f);
}


BlendState BlendCorrect
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};


technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VertexShader_PointLight()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PixelShader_PointLight()));
		//SetBlendState(BlendCorrect, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
	}
}
