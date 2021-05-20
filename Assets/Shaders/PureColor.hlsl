#include"PBRHelper.hlsli"
#include"CommonIncludes.hlsli"

//顶点着色器
VertexOutput VS_Main(VertexInput input)
{
    VertexOutput output;
    
    matrix mvp = mul(mul(ObjectToWorld, WorldToView), Projection);
    
    output.pos = mul(float4(input.vertex, 1.0), mvp);
    output.uv = input.texcoord;
    output.world_normal = mul(input.normal, (float3x3) ObjectToWorld_InverseTranspose);
    output.world_pos = mul(float4(input.vertex, 1.0), ObjectToWorld).xyz;
    output.world_tangent = mul(input.tangent, ObjectToWorld).xyz;
    output.world_binormal = cross(output.world_normal, output.world_tangent) * (-input.tangent.w);
    
    return output;
}

//像素着色器
float4 PS_Main(VertexOutput input) : SV_TARGET
{
    float3 color = float3(1, 0, 0);
    return float4(color, 1.0f);
}
