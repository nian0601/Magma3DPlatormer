#include "S_u_vertex_layouts.fx"

Matrix World;
Matrix View;
Matrix NotInvertedView;
Matrix Projection;
Matrix InvertedProjection;
Matrix ViewProjection;
float3 CameraPosition;
float3 Scale;


//Model-Textures
Texture2D AlbedoTexture;
Texture2D NormalTexture;
Texture2D RoughnessTexture;
Texture2D MetalnessTexture;
Texture2D AOTexture;
Texture2D EmissiveTexture;

//GBuffer-Textures
Texture2D AlbedoMetalnessTexture;
Texture2D NormalRoughnessTexture;
Texture2D DepthTexture;


TextureCube Cubemap;

float GlobalMetalness;
float GlobalRoughness;


SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState pointSampling
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct PBLData
{
	float3 Albedo;
	float3 Normal;
	float3 Metalness;
	float Roughness;
	float3 AmbientOcclusion;

	float3 MetalnessAlbedo;
	float3 Substance;
	float RoughnessOffsetted;

	float4 WorldPosition;
};

PBLData CalculatePBLData(float2 aTexCoord, float3 aVertexNormal, float3 aVertexBinormal, float3 aVertexTangent)
{
	PBLData output;

	output.Albedo = AlbedoTexture.Sample(linearSampling, aTexCoord);
	
	output.Metalness = MetalnessTexture.Sample(linearSampling, aTexCoord);
	output.Roughness = RoughnessTexture.Sample(linearSampling, aTexCoord);
	output.Metalness = float3(GlobalMetalness, GlobalMetalness, GlobalMetalness);
	output.Roughness = GlobalRoughness;
	output.AmbientOcclusion = AOTexture.Sample(linearSampling, aTexCoord);

	output.Substance = (0.04 - 0.04 * output.Metalness) + output.Albedo * output.Metalness;
	output.MetalnessAlbedo = output.Albedo - output.Albedo * output.Metalness;

	output.RoughnessOffsetted = pow(8192, output.Roughness);

	output.Normal = NormalTexture.Sample(linearSampling, aTexCoord) * 2 - 1;
	float3x3 tangentSpaceMatrix = float3x3(normalize(aVertexTangent), normalize(aVertexBinormal), normalize(aVertexNormal));
	output.Normal = normalize(mul(output.Normal, tangentSpaceMatrix));

	return output;
}

PBLData CalculatePBLData_GBuffer(float2 aTexCoord)
{
	float4 AlbedoMetalness = AlbedoMetalnessTexture.Sample(pointSampling, aTexCoord);
	float4 NormalRoughness = NormalRoughnessTexture.Sample(pointSampling, aTexCoord);
	float Depth = DepthTexture.Sample(pointSampling, aTexCoord).x;

	float3 Albedo = AlbedoMetalness.xyz;
	float3 Metalness = float3(AlbedoMetalness.w, AlbedoMetalness.w, AlbedoMetalness.w);

	float3 Normal = NormalRoughness.xyz;
	Normal *= 2.f;
	Normal -= 1.f;
	Normal = normalize(Normal);

	float Roughness = NormalRoughness.w;

	float x = aTexCoord.x * 2.f - 1.f;
	float y = (1.f - aTexCoord.y) * 2.f - 1.f;
	float z = Depth;

	float4 WorldPosition = float4(x, y, z, 1.f);
	WorldPosition = mul(WorldPosition, InvertedProjection);
	WorldPosition /= WorldPosition.w;
	WorldPosition = mul(WorldPosition, NotInvertedView);

	PBLData output;
	output.Albedo = Albedo;
	output.Normal = Normal;
	output.Metalness = Metalness;
	output.Roughness = Roughness;
	output.AmbientOcclusion = float3(1.f, 1.f, 1.f);

	output.MetalnessAlbedo = output.Albedo - output.Albedo * output.Metalness;
	output.Substance = (0.04f - 0.04f * output.Metalness) + output.Albedo * output.Metalness;
	output.RoughnessOffsetted = pow(8192, output.Roughness);

	output.WorldPosition = WorldPosition;

	return output;
}

PBLData CalculatePBLData_NoTextures(float2 aTexCoord, float3 aVertexNormal, float3 aVertexBinormal, float3 aVertexTangent)
{
	PBLData output;

	output.Albedo = float3(1.f, 0.f, 0.f);
	
	output.Metalness = float3(GlobalMetalness, GlobalMetalness, GlobalMetalness);
	output.Roughness = GlobalRoughness;
	output.AmbientOcclusion = float3(1.f, 1.f, 1.f);

	output.Substance = (0.04 - 0.04 * output.Metalness) + output.Albedo * output.Metalness;
	output.MetalnessAlbedo = output.Albedo - output.Albedo * output.Metalness;

	output.RoughnessOffsetted = pow(8192, output.Roughness);

	output.Normal = aVertexNormal;

	return output;
}


//
//	Functions used to calculate Ambient light
//--------------------------------------------------------

//Calculates how reflections will behave in different materials
float3 ReflectionFresnel(float3 aSubstance, float3 aLightDir, float3 aHalfVector, float aRoughness)
{
	return (aSubstance + (1.f - aSubstance) * pow((1.f - saturate(dot(aLightDir, aHalfVector))), 5) / (4 - 3 * aRoughness));
}


//Calculations copied from Lys which calculates which MIP level to use for the reflections based on roughness
static const float k0 = 0.00098f;
static const float k1 = 0.9921f;
static const float fakeLysMaxSpecularPower = (2.f / (0.0014f * 0.0014f)) - 2.f;
static const float fMaxT = (exp2(-10.f / sqrt((2.f / (0.0014f * 0.0014f)) - 2.f)) - 0.00098f) / 0.9921f;

float RoughToSPow(float aRoughness)
{
	return (2.f / (aRoughness * aRoughness)) - 2.f;
}

float GetLysMipLevel(float aRoughness, int aMipCount)
{
	float fSmulMaxT = (exp2(-10.0 / sqrt(RoughToSPow(aRoughness))) - k0) / k1;

	return float(aMipCount - 1 - 0) * (1.0 - clamp(fSmulMaxT / fMaxT, 0.0, 1.0));
}



//
//	Functions used to calculate Direct light
//--------------------------------------------------------
float3 Fresnel(float3 aSubstance, float3 aLightDir, float3 aHalfVector)
{
	return (aSubstance + (1.f - aSubstance) * pow((1.f - saturate(dot(aLightDir, aHalfVector))), 5));
}

float SchlickForGGX(float aRoughness, float NdotV, float NdotL)
{
	float k = aRoughness * aRoughness * 0.5f;
	float G1V = NdotV * (1.f - k) + k;
	float G1L = NdotL * (1.f - k) + k;

	return 0.25f / (G1V * G1L);
}

float D_GGX(float HdotN, float aRoughness)
{
	float m = aRoughness * aRoughness;
	float m2 = m * m;
	float denominator = HdotN * HdotN * (m2 - 1.f) + 1.f;

	return m2 / (3.14159 * denominator * denominator);
}


//
//	Attenuations
//--------------------------------------------------------
float Attenuation(float3 aLightVec, float aRange)
{
	float distance = length(aLightVec);
	float attenuation = 1.f / (1.f + 0.1f * distance + 0.01f * distance * distance);
	float fallOff = 1.f - (distance / (aRange + 0.00001f));
	float totalAttenuation = attenuation * fallOff;
	return totalAttenuation;
/*
	float distance = length(aLightVec);
	return 1 - (distance / aRange);

	float attenuation = (1.f + 0.1f * distance + 0.01f * distance * distance); //1.f;
	float fallOff = 1.f - (distance / (aRange + 0.00001f));
	return saturate(attenuation * fallOff);
	*/
}

float AngularAttenuation(float3 aLightVec, float3 aLightDirection, float aLightCone)
{
	float angularAttenuation = dot(-aLightVec, aLightDirection);
	angularAttenuation -= 1.f - aLightCone;
	angularAttenuation *= 1.f / aLightCone;

	return angularAttenuation;
}