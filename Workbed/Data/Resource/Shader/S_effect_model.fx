#include "S_u_variables.fx"

Pixel_Model VertexShader_Model(Vertex_Model aInput)
{
	Pixel_Model output = (Pixel_Model)0;
	Matrix mat = mul(World, ViewProjection);
	output.Position = mul(aInput.Position, mat);
	
	output.Tex = aInput.Tex;
	
	output.Normal = mul(aInput.Normal, World);
	output.BiNormal = aInput.BiNormal;
	output.Tangent = mul(aInput.Tangent, World);

	output.WorldPosition = mul(aInput.Position, World);
	
	return output;
}

GBuffer PixelShader_Model(Pixel_Model aInput) : SV_Target
{
	float3 norm = NormalTexture.Sample(linearSampling, aInput.Tex) * 2 - 1;
	
	aInput.Normal = normalize(aInput.Normal);
	aInput.BiNormal = normalize(aInput.BiNormal);
	aInput.Tangent  = normalize(aInput.Tangent);
	
	float3x3 tangentSpaceMatrix = float3x3(aInput.Tangent, aInput.BiNormal, aInput.Normal);
	norm = normalize(mul(norm, tangentSpaceMatrix));
	

	float4 AlbedoColor = AlbedoTexture.Sample(linearSampling, aInput.Tex);
	float4 AmbientOcclusion = AOTexture.Sample(linearSampling, aInput.Tex);
	float Metalness = MetalnessTexture.Sample(linearSampling, aInput.Tex).x;
	float Roughness = RoughnessTexture.Sample(linearSampling, aInput.Tex).x;

	GBuffer output;
	output.AlbedoMetalness = float4(AlbedoColor.xyz, Metalness);
	output.NormalRoughness = float4(norm, Roughness);
	output.Depth = aInput.Position.z;
	
	return output;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VertexShader_Model()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PixelShader_Model()));
	}
}