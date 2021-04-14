Texture2D MainTex : register(t1);
Texture2D NormalMap : register(t2);
Texture2D BumpMap : register(t3);
Texture2D MetallicMap : register(t4);
Texture2D EmissionMap : register(t5);

SamplerState samplerState : register(s0);

//常用空间变换矩阵
cbuffer CB_VS_TransformMatrix : register(b0)
{
    matrix ObjectToWorld;
    matrix WorldToView;
    matrix Projection;
    matrix ObjectToWorld_InverseTranspose;
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
    float2 uv : TEXCOORD0;
    float3 world_pos : TEXCOORD1;
    float3 world_normal : NORMAL;
};