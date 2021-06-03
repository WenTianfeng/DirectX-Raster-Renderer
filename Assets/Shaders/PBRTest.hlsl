#include"PBRHelper.hlsli"
#include"CommonIncludes.hlsli"

cbuffer CB_VS_UVScaleOffset : register(b1)
{
    float2 UV_Scale = float2(1, 1);
    float2 UV_Offset = float2(0, 0);
    float BumpScale = 0;
};

//顶点着色器
VertexOutput VS_Main(VertexInput input)
{
    VertexOutput output;
    
    matrix mvp = mul(mul(ObjectToWorld, WorldToView), Projection);
    
    output.pos = mul(float4(input.vertex, 1.0), mvp);
    output.uv = input.texcoord * UV_Scale+UV_Offset;
    output.world_normal = mul(input.normal, (float3x3) ObjectToWorld_InverseTranspose);
    output.world_pos = mul(float4(input.vertex, 1.0), ObjectToWorld).xyz;
    output.world_tangent = mul(input.tangent, ObjectToWorld).xyz;
    output.world_binormal = cross(output.world_normal, output.world_tangent) * (-input.tangent.w);
    
    return output;
}

//像素着色器
float4 PS_Main(VertexOutput input) : SV_TARGET
{
    float3 albedo = MainTex.Sample(GeneralSampler, input.uv).rgb;
    float metallic = MetallicSmoothnessMap.Sample(GeneralSampler, input.uv).r;
    float smoothness = MetallicSmoothnessMap.Sample(GeneralSampler, input.uv).a;
    float roughness = 1.0f - smoothness;
    float3 color = float3(0, 0, 0);
    
    //由三个基向量组成的从切线空间到世界空间的变换矩阵
    float3x3 TBN = float3x3(
        normalize(input.world_tangent),
        normalize(input.world_binormal),
        normalize(input.world_normal));
    
    //法线纹理采样结果（RGB 0~1）
    float3 rawNormal = NormalMap.Sample(GeneralSampler, input.uv).xyz;
    //映射至 -1~1
    float3 tangentSpaceNormal = UnpackNormal(rawNormal);
    
    //引入法线向量z分量的缩放
    tangentSpaceNormal.xy *= BumpScale.x;
    tangentSpaceNormal.z = sqrt(1 - saturate(dot(tangentSpaceNormal.xy, tangentSpaceNormal.xy)));
    
    //世界空间法线向量
    float3 worldNormal = normalize(mul(tangentSpaceNormal, TBN));
    
    //世界空间视线向量
    float3 viewVec = normalize(ViewPos - input.world_pos);
    

    //遍历所有光源，对每个光源进行逐像素光照计算
    for (uint i = 0; i < NUM_LIGHTS; i++)
    {
        
        DirectIlluminationData directLightData;    
        
        //根据光源类型分别处理
        switch (Lights[i].Type)
        {
            case DIRECTIONAL_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                directLightData = ProcessDirectionalLight(Lights[i], input.world_pos);
            }
            break;
         
            case POINT_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                directLightData = ProcessPointlLight(Lights[i], input.world_pos);
                }
            break;
         
            case SPOT_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                directLightData = ProcessSpotLight(Lights[i], input.world_pos);
            }
            break;
         
        }
                
        color += UnrealPBR(albedo, roughness, metallic, worldNormal, viewVec, directLightData);
    }
    
    // HDR tonemapping
    color = color / (color + float3(1.0f,1.0f,1.0f));
    // gamma correct
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    return float4(color, 1.0f);
}
