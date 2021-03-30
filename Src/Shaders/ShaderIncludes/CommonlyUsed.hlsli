Texture2D mainTex : register(t0);
SamplerState samplerState : register(s0);

//常用空间变换矩阵
cbuffer CB_VS_TransformMatrix : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix worldInverseTranspose;
};

//顶点着色器输入
struct VertexInput
{
    float3 vertex : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
};

//顶点着色器输出
struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 world_normal : NORMAL;
};