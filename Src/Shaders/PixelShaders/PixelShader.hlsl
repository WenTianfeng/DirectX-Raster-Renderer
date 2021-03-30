#include"..\ShaderIncludes\CommonlyUsed.hlsli"

float4 main(VertexOutput input) : SV_TARGET
{
    float4 albedo;
    
    albedo = float4(0, 1, 0, 1);
    float4 diffuse_color = albedo * max(0, dot(float3(1,1,-1), input.world_normal));
    return diffuse_color;
}