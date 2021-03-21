#include"ShaderHeader.hlsli"

VertexOutput main(VertexInput input)
{
    input.vertex.x += xoffset;
    input.vertex.y += yoffset;
    VertexOutput output;
    output.pos = float4(input.vertex,0.0,1.0);
    output.uv = input.texcoord;

    return output;
}