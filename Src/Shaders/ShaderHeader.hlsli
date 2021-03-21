Texture2D mainTex : register(t0);
SamplerState samplerState : register(s0);

cbuffer CB_Offset : register(b0)
{
    float xoffset;
    float yoffset;
};

struct VertexInput
{
    float2 vertex : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};