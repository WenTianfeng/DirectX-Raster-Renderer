
TextureCube SkyboxMap : register(t1);
Texture2D MainTex : register(t2);
Texture2D NormalMap : register(t3);
Texture2D BumpMap : register(t4);
Texture2D MetallicMap : register(t5);
Texture2D EmissionMap : register(t6);

SamplerState SkyboxSampler : register(s0);
SamplerState MainTexSampler : register(s1);

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

//天空盒顶点着色器输出
struct VertexOutput_Skybox
{
    float4 pos : SV_POSITION;
    float3 pos_model : POSITION;
};