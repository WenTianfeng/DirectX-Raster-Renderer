#include"CommonIncludes.hlsli"


VertexOutput VS_Main(VertexInput input)
{
    VertexOutput output;
    
    matrix mvp = mul(mul(ObjectToWorld, WorldToView), Projection);
    
    output.pos = mul(float4(input.vertex, 1.0), mvp);
    output.uv = input.texcoord;
    output.world_normal = mul(input.normal, (float3x3) ObjectToWorld_InverseTranspose);
    output.world_pos = mul(float4(input.vertex, 1.0), ObjectToWorld).xyz;
    output.world_tangent = mul(input.tangent, ObjectToWorld).xyz;
    output.world_binormal = cross(output.world_normal, output.world_tangent) * input.tangent.w;
    
    return output;
}

float4 PS_Main(VertexOutput input) : SV_TARGET
{
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
}