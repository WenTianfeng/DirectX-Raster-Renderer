Texture2D mainTex : register(t0);
SamplerState samplerState : register(s0);

//���ÿռ�任����
cbuffer CB_VS_TransformMatrix : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix worldInverseTranspose;
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
    float2 uv : TEXCOORD;
    float3 world_normal : NORMAL;
};