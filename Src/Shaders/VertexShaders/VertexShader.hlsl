#include"..\ShaderIncludes\CommonlyUsed.hlsli"

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    matrix mvp = mul(mul(world, view), projection);
    
    output.pos = mul(float4(input.vertex,1.0), mvp);
    output.uv = input.texcoord;
    output.world_normal = mul(input.normal, (float3x3) worldInverseTranspose);

    return output;
}