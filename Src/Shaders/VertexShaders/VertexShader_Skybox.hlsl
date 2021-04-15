#include"..\ShaderIncludes\CommonIncludes.hlsli"

 VertexOutput_Skybox main(VertexInput input)
{
    VertexOutput_Skybox output;
    matrix mvp = mul(mul(ObjectToWorld, WorldToView), Projection);

    float4 posHomo = mul(float4(input.vertex, 1.0f), mvp);
    output.pos = posHomo.xyww;
    output.pos_model = input.vertex;

    return output;
}