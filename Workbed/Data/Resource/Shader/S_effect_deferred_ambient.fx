#include "S_u_variables.fx"

float4 PixelShader_Deferred_Ambient(Pixel_FullscreenQuad aInput) : SV_Target
{
	PBLData data = CalculatePBLData_GBuffer(aInput.Tex);
	float3 ToEye = normalize(CameraPosition - data.WorldPosition.xyz);
	float3 RefFresnel = ReflectionFresnel(data.Substance, data.Normal, ToEye, 1 - data.RoughnessOffsetted);


	//Calculate the AmbientDiffuse, this is simply the Cubemap * AmbientOcclusion * MetalnessAlbedo
	//we also multiply with (1 - RefFresnel) to keep us energy conserving (Dont want to create more light than we receive)
	float3 ambientDiffuse = Cubemap.SampleLevel(linearSampling, data.Normal, 7).xyz * data.AmbientOcclusion * data.MetalnessAlbedo * (1 - RefFresnel);


	//Next we need to calculate the AmbientSpecc
	float3 ReflectionVector = reflect(ToEye, data.Normal);
	float LysMipLevel = GetLysMipLevel(data.Roughness, 10);

	float3 ambientSpecc = Cubemap.SampleLevel(linearSampling, ReflectionVector, LysMipLevel).xyz * data.AmbientOcclusion * RefFresnel;

	float3 ambientColor = saturate(ambientDiffuse + ambientSpecc);



	//To test things we also do calculations for a faked directional light
	/*
	float3 LightDir = normalize(float3(-0.5f, 1.f, 1.f));
	float3 LightColor = float3(0.f, 1.f, 0.f);

	float3 LightHalfVec = normalize(LightDir + -ToEye);

	float NdotL = saturate(dot(data.Normal, LightDir));
	float HdotN = saturate(dot(LightHalfVec, data.Normal));

	float3 F = saturate(Fresnel(data.Substance, LightDir, LightHalfVec));
	float D = saturate(D_GGX(HdotN, (data.Roughness + 1) / 2));
	float V = saturate(SchlickForGGX((data.Roughness + 1) / 2, dot(data.Normal, ToEye), NdotL));

	float3 LightSpecc = (((D * V * F) / 3.14159 + (1 - F) * NdotL * data.MetalnessAlbedo * data.AmbientOcclusion)) * LightColor;
	*/
	return saturate(float4(ambientColor, 1.f));
}