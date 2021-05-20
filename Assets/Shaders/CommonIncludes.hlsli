
//----------------------------------------��ɫ����Դ----------------------------------------
//��������
TextureCube SkyboxMap : register(t1);
Texture2D MainTex : register(t2);
Texture2D NormalMap : register(t3);
Texture2D BumpMap : register(t4);
Texture2D MetallicSmoothnessMap : register(t5);
Texture2D EmissionMap : register(t6);
Texture2D AmbientOcclusionMap : register(t7);

//���ò�����
SamplerState SkyboxSampler : register(s0);
SamplerState GeneralSampler : register(s1);

//���ÿռ�任����
cbuffer CB_PresetVariables : register(b0)
{
    matrix ObjectToWorld;
    matrix WorldToView;
    matrix Projection;
    matrix ObjectToWorld_InverseTranspose;
    float3 ViewPos;
    float Padding;

};

//----------------------------------------���ýṹ��----------------------------------------
//������ɫ������
struct VertexInput
{
    float3 vertex : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float4 tangent : TANGENT; 
};

//������ɫ�����
struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 world_pos : TEXCOORD1;
    float3 world_normal : NORMAL;
    float3 world_tangent : TANGENT;
    float3 world_binormal : BINORMAL;
};

//��պж�����ɫ�����
struct VertexOutput_Skybox
{
    float4 pos : SV_POSITION;
    float3 pos_model : POSITION;
};

//----------------------------------------���ú���----------------------------------------

//�Է����������������д���
float3 UnpackNormal(float3 rawNormal)
{
    return rawNormal * 2.0f - 1.0f;
}