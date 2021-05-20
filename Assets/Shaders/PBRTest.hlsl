#include"PBRHelper.hlsli"
#include"CommonIncludes.hlsli"

cbuffer CB_VS_UVScaleOffset : register(b1)
{
    float2 UV_Scale;
    float2 UV_Offset;
    float4 BumpScale;
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
    

    //遍历所有光源
    for (uint i = 0; i < NUM_LIGHTS; i++)
    {
        //跳过超过光照范围的Point和Spot光源
        //if (Lights[i].Type != DIRECTIONAL_LIGHT && length(Lights[i].Position - input.world_pos) > Lights[i].Range)
        //{
        //    continue;
        //}
        
        //根据光源类型分别处理
        switch (Lights[i].Type)
        {
            case DIRECTIONAL_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                DirectIlluminationData directionalLightData = ProcessDirectionalLight(Lights[i], input.world_pos);
                
                //世界空间光照向量
                float3 lightVec = directionalLightData.direction;
                
                //半角向量
                float3 halfVec = normalize(viewVec + lightVec);

                //需求变量
                float NdotL = max(0, dot(lightVec, worldNormal));
                float HdotV = max(0, dot(halfVec, viewVec));
                float NdotV = max(0, dot(viewVec, worldNormal));
                float NdotH = max(0, dot(halfVec, worldNormal));
                
                //基础反射率
                float3 F0 = float3(0.04f, 0.04f, 0.04f);
                F0 = lerp(F0, albedo, metallic);
                //菲涅尔项
                float3 F = FresnelSchlick(HdotV, F0);
                //法线分布项
                float NDF = DistributionGGX(NdotH, roughness);
                //自阴影项
                float G = GeometrySmith(NdotV, NdotL, roughness);
                
                float3 numerator = NDF * G * F;
                float denominator = 4.0f * NdotV * NdotL;
                float3 specular = numerator / max(denominator, 0.001f);
                
                float3 kS = F;
                float3 kD = float3(1.0f,1.0f,1.0f) - kS;
  
                kD *= 1.0 - metallic; // 由于金属表面不折射光，没有漫反射颜色，通过归零kD来实现这个规则
                
                float PI = 3.14159265359;
                
                color += (kD * albedo / PI + specular) * directionalLightData.color.rgb * NdotL;
                
            }
                break;
         
            case POINT_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                DirectIlluminationData pointLightData = ProcessPointlLight(Lights[i], input.world_pos);
             
                //世界空间光照向量
                float3 lightVec = pointLightData.direction;
                
                //半角向量
                float3 halfVec = normalize(viewVec + lightVec);
                
                //将经过光照模型计算的颜色叠加到最终颜色
                //color += pointLightData.color * max(0, dot(pointLightData.direction, input.world_normal));
            }
                break;
         
            case SPOT_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                //DirectIlluminationData spotLightData = ProcessSpotLight(Lights[i], input.world_pos);
             
                //将经过光照模型计算的颜色叠加到最终颜色
                //color += spotLightData.color * max(0, dot(spotLightData.direction, input.world_normal));
            }
                break;
         
        }
    
    }
    
    // HDR tonemapping
    color = color / (color + float3(1.0f,1.0f,1.0f));
    // gamma correct
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));


    
    return float4(color, 1.0f);
}
