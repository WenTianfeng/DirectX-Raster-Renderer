#include"PBR.hlsli"
#include"CommonIncludes.hlsli"

cbuffer CB_VS_UVScaleOffset : register(b1)
{
    float2 UV_Scale;
    float2 UV_Offset;
};

cbuffer CB_PS_PBR : register(b2)
{
    float4 Tint;
};


//������ɫ��
VertexOutput VS_Main(VertexInput input)
{
    VertexOutput output;
    
    matrix mvp = mul(mul(ObjectToWorld, WorldToView), Projection);
    
    output.pos = mul(float4(input.vertex, 1.0), mvp);
    output.uv = input.texcoord * UV_Scale+UV_Offset;
    output.world_normal = mul(input.normal, (float3x3) ObjectToWorld_InverseTranspose);
    output.world_pos = mul(float4(input.vertex, 1.0), ObjectToWorld).xyz;

    return output;
}

//������ɫ��
float4 PS_Main(VertexOutput input) : SV_TARGET
{
    float4 ambient = float4(0.1, 0.1, 0.1, 1.0);
    float4 albedo = MainTex.Sample(MainTexSampler, input.uv) * Tint + ambient;
    float4 color = float4(0, 0, 0, 1);

    //�������й�Դ
    for (uint i = 0; i < NUM_LIGHTS; i++)
    {
        //�����������շ�Χ��Point��Spot��Դ
        //if (Lights[i].Type != DIRECTIONAL_LIGHT && length(Lights[i].Position - input.world_pos) > Lights[i].Range)
        //{
        //    continue;
        //}
        
        //���ݹ�Դ���ͷֱ���
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
                //DirectIlluminationData pointLightData = ProcessPointlLight(Lights[i], input.world_pos);
             
                //����������ģ�ͼ������ɫ���ӵ�������ɫ
                //color += pointLightData.color * max(0, dot(pointLightData.direction, input.world_normal));
            }
                break;
         
            case SPOT_LIGHT:
            {
                //���ݶ�Ӧ�������ʹ���õ������ֱ�ӹ�����Ϣ
                //DirectIlluminationData spotLightData = ProcessSpotLight(Lights[i], input.world_pos);
             
                //����������ģ�ͼ������ɫ���ӵ�������ɫ
                //color += spotLightData.color * max(0, dot(spotLightData.direction, input.world_normal));
            }
                break;
         
        }
    
    }
    
    return color;
}
