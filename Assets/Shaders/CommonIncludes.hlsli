
//----------------------------------------着色器资源----------------------------------------
//常用纹理
TextureCube SkyboxMap : register(t1);
Texture2D MainTex : register(t2);
Texture2D NormalMap : register(t3);
Texture2D BumpMap : register(t4);
Texture2D MetallicSmoothnessMap : register(t5);
Texture2D EmissionMap : register(t6);
Texture2D AmbientOcclusionMap : register(t7);

//常用采样器
SamplerState SkyboxSampler : register(s0);
SamplerState GeneralSampler : register(s1);

//常用空间变换矩阵
cbuffer CB_PresetVariables : register(b0)
{
    matrix ObjectToWorld;
    matrix WorldToView;
    matrix Projection;
    matrix ObjectToWorld_InverseTranspose;
    float3 ViewPos;
    float Padding;

};

//----------------------------------------常用结构体----------------------------------------
//顶点着色器输入
struct VertexInput
{
    float3 vertex : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float4 tangent : TANGENT; 
};

//顶点着色器输出
struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 world_pos : TEXCOORD1;
    float3 world_normal : NORMAL;
    float3 world_tangent : TANGENT;
    float3 world_binormal : BINORMAL;
};

//天空盒顶点着色器输出
struct VertexOutput_Skybox
{
    float4 pos : SV_POSITION;
    float3 pos_model : POSITION;
};

//----------------------------------------常用函数----------------------------------------

//对法线纹理采样结果进行处理
float3 UnpackNormal(float3 rawNormal)
{
    return rawNormal * 2.0f - 1.0f;
}