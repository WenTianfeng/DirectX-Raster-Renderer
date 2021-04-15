#include"..\ShaderIncludes\CommonIncludes.hlsli"

float4 main(VertexOutput_Skybox input) : SV_TARGET
{
    return SkyboxMap.Sample(SkyboxSampler, input.pos_model);
}