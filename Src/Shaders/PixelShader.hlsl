#include"ShaderHeader.hlsli"

float4 main(VertexOutput input) : SV_TARGET
{
    float4 color;
    color = mainTex.Sample(samplerState, input.uv);
    return color;
}