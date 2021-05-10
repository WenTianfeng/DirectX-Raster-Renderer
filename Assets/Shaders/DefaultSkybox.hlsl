#include"CommonIncludes.hlsli"

VertexOutput_Skybox VS_Main(VertexInput input)
{
    VertexOutput_Skybox output;
    matrix mvp = mul(mul(ObjectToWorld, WorldToView), Projection);

    float4 posHomo = mul(float4(input.vertex, 1.0f), mvp);
    output.pos = posHomo.xyww;
    output.pos_model = input.vertex;

    return output;
}

float4 PS_Main(VertexOutput_Skybox input) : SV_TARGET
{
    return SkyboxMap.Sample(SkyboxSampler, input.pos_model);
}