#include"LightHelper.hlsli"



float4 DisneyPBR(float3 diffuse,float3 specular,float oneMinusReflectivity,float smoothness,float3 normal,float3 viewDir,
    DirectIlluminationData directLightData,IndirectIlluminationData indirectLightData)
{
    float4 col = float4(1, 1, 1, 1);
    return col;
}

//���������ƺ���
float3 FresnelSchlick(float cosTheta, float3 f0)
{
    return f0 + (1.0f - f0) * pow(1.0f - cosTheta, 5.0f);
}

//���߷ֲ�����
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

float3 UnrealPBR(float3 albedo, float roughness, float metallic, float3 worldNormal, float3 viewVec,
    DirectIlluminationData directLightData)
{
    //����˥�������Ĺ�����ɫ
    float4 lightColor = directLightData.color;
    //����ռ��������
    float3 lightVec = directLightData.direction;
    
    //�������
    float3 halfVec = normalize(viewVec + lightVec);
    
    float NdotL = max(0, dot(lightVec, worldNormal));
    float HdotV = max(0, dot(halfVec, viewVec));
    float NdotV = max(0, dot(viewVec, worldNormal));
    float NdotH = max(0, dot(halfVec, worldNormal));
        
        //����������
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedo, metallic);
        //��������
    float3 F = FresnelSchlick(HdotV, F0);
        //���߷ֲ���
    float NDF = DistributionGGX(NdotH, roughness);
        //����Ӱ��
    float G = GeometrySmith(NdotV, NdotL, roughness);
                
    float3 numerator = NDF * G * F;
    float denominator = 4.0f * NdotV * NdotL;
    float3 specular = numerator / max(denominator, 0.001f);
                
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
  
    kD *= 1.0 - metallic; // ���ڽ������治����⣬û����������ɫ��ͨ������kD��ʵ���������
                
    float PI = 3.14159265359;
                
    float3 color = (kD * albedo / PI + specular) * lightColor.rgb * NdotL;
    
    return color;
}