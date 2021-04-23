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
        //�����������շ�Χ��Point��Spot��Դ
        //if (Lights[i].Type != DIRECTIONAL_LIGHT && length(Lights[i].Position - input.world_pos) > Lights[i].Range)
        //{
        //    continue;
        //}

        switch (Lights[i].Type)
        {
            case DIRECTIONAL_LIGHT:
            {
                //���ݶ�Ӧ�������ʹ���õ������ֱ�ӹ�����Ϣ
                    DirectIlluminationData directionalLightData = ProcessDirectionalLight(Lights[i], input.world_pos);
             
                //����������ģ�ͼ������ɫ���ӵ�������ɫ
                    color += albedo * directionalLightData.color * max(0, dot(-directionalLightData.direction, input.world_normal));

                }
                break;
         
            case POINT_LIGHT:
            {
                //���ݶ�Ӧ�������ʹ���õ������ֱ�ӹ�����Ϣ
                    DirectIlluminationData pointLightData = ProcessPointlLight(Lights[i], input.world_pos);
             
                //����������ģ�ͼ������ɫ���ӵ�������ɫ
                    color += albedo * pointLightData.color * max(0, dot(pointLightData.direction, input.world_normal));
                }
                break;
         
            case SPOT_LIGHT:
            {
                //���ݶ�Ӧ�������ʹ���õ������ֱ�ӹ�����Ϣ
                    DirectIlluminationData spotLightData = ProcessSpotLight(Lights[i], input.world_pos);
             
                //����������ģ�ͼ������ɫ���ӵ�������ɫ
                    color += albedo * spotLightData.color * max(0, dot(spotLightData.direction, input.world_normal));
                }
                break;
         
        }
    
    }
    
    return color;
}
