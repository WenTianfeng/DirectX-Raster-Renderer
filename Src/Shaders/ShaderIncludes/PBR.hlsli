#include"LightHelper.hlsli"

float4 DisneyPBR(float3 diffuse,float3 specular,float oneMinusReflectivity,float smoothness,float3 normal,float3 viewDir,
    DirectIlluminationData directLightData,IndirectIlluminationData indirectLightData)
{
    float4 col = float4(1, 1, 1, 1);
    return col;
}