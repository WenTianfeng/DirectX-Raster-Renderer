#include"PBR.hlsli"
#include"CommonIncludes.hlsli"

VertexOutput VS_Main(VertexInput input)
{
    VertexOutput output;
    
    matrix mvp = mul(mul(ObjectToWorld, WorldToView), Projection);
    
    output.pos = mul(float4(input.vertex, 1.0), mvp);
    output.uv = input.texcoord;
    output.world_normal = mul(input.normal, (float3x3) ObjectToWorld_InverseTranspose);
    output.world_pos = mul(float4(input.vertex, 1.0), ObjectToWorld).xyz;

    return output;
}

float4 PS_Main(VertexOutput input) : SV_TARGET
{
    float4 albedo = float4(0.5f, 0.5f, 0.5f, 1);
    float4 color = float4(0, 0, 0, 1);
    

    for (uint i = 0; i < NUM_LIGHTS; i++)
    {
        //跳过超过光照范围的Point和Spot光源
        //if (Lights[i].Type != DIRECTIONAL_LIGHT && length(Lights[i].Position - input.world_pos) > Lights[i].Range)
        //{
        //    continue;
        //}

        switch (Lights[i].Type)
        {
            case DIRECTIONAL_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                    DirectIlluminationData directionalLightData = ProcessDirectionalLight(Lights[i], input.world_pos);
             
                //将经过光照模型计算的颜色叠加到最终颜色
                    color += albedo * directionalLightData.color * max(0, dot(-directionalLightData.direction, input.world_normal));

                }
                break;
         
            case POINT_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                    DirectIlluminationData pointLightData = ProcessPointlLight(Lights[i], input.world_pos);
             
                //将经过光照模型计算的颜色叠加到最终颜色
                    color += albedo * pointLightData.color * max(0, dot(pointLightData.direction, input.world_normal));
                }
                break;
         
            case SPOT_LIGHT:
            {
                //根据对应光照类型处理得到所需的直接光照信息
                    DirectIlluminationData spotLightData = ProcessSpotLight(Lights[i], input.world_pos);
             
                //将经过光照模型计算的颜色叠加到最终颜色
                    color += albedo * spotLightData.color * max(0, dot(spotLightData.direction, input.world_normal));
                }
                break;
         
        }
    
    }
    
    return color;
}
