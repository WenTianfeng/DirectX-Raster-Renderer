#include"LightHelper.hlsli"



float4 DisneyPBR(float3 diffuse,float3 specular,float oneMinusReflectivity,float smoothness,float3 normal,float3 viewDir,
    DirectIlluminationData directLightData,IndirectIlluminationData indirectLightData)
{
    float4 col = float4(1, 1, 1, 1);
    return col;
}

//菲涅尔近似函数
float3 FresnelSchlick(float cosTheta, float3 f0)
{
    return f0 + (1.0f - f0) * pow(1.0f - cosTheta, 5.0f);
}

//法线分布函数
float DistributionGGX(float NdotH, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = NdotH2 * (a2 - 1.0f) + 1.0f;
    
    float PI = 3.14159265359;
    denom = PI * denom * denom;
    
    return num / denom;
    
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    
    float num = NdotV;
    float denom = NdotV * (1.0f - k) + k;
    
    return num / denom;
}

float GeometrySmith(float NdotV,float NdotL,float roughness)
{
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
    
}
