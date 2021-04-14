Texture2D MainTex : register(t1);
Texture2D NormalMap : register(t2);
Texture2D BumpMap : register(t3);
Texture2D MetallicMap : register(t4);
Texture2D EmissionMap : register(t5);

SamplerState samplerState : register(s0);

//���ÿռ�任����
cbuffer CB_VS_TransformMatrix : register(b0)
{
    matrix ObjectToWorld;
    matrix WorldToView;
    matrix Projection;
    matrix ObjectToWorld_InverseTranspose;
};

//������ɫ������
struct VertexInput
{
    float3 vertex : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
};

//������ɫ�����
struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 world_pos : TEXCOORD1;
    float3 world_normal : NORMAL;
};