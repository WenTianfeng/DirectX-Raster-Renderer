#include"..\ShaderIncludes\CommonIncludes.hlsli"

float4 main(VertexOutput input) : SV_TARGET
{
    float4 albedo;
    
    albedo = float4(0.5f, 0.5f, 0.5f, 1);
    float4 diffuse_color = albedo * max(0, dot(float3(2,1,-1), input.world_normal));
    return diffuse_color;
}