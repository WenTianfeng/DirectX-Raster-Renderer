#include"PBRHelper.hlsli"
#include"CommonIncludes.hlsli"

cbuffer CB_VS_UVScaleOffset : register(b1)
{
    float2 UV_Scale;
    float2 UV_Offset;
    float4 BumpScale;
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
    output.world_tangent = mul(input.tangent, ObjectToWorld).xyz;
    output.world_binormal = cross(output.world_normal, output.world_tangent) * (-input.tangent.w);
    
    return output;
}

//������ɫ��
float4 PS_Main(VertexOutput input) : SV_TARGET
{
    float3 albedo = MainTex.Sample(GeneralSampler, input.uv).rgb;
    float metallic = MetallicSmoothnessMap.Sample(GeneralSampler, input.uv).r;
    float smoothness = MetallicSmoothnessMap.Sample(GeneralSampler, input.uv).a;
    float roughness = 1.0f - smoothness;
    float3 color = float3(0, 0, 0);
    
    //��������������ɵĴ����߿ռ䵽����ռ�ı任����
    float3x3 TBN = float3x3(
        normalize(input.world_tangent),
        normalize(input.world_binormal),
        normalize(input.world_normal));
    
    //����������������RGB 0~1��
    float3 rawNormal = NormalMap.Sample(GeneralSampler, input.uv).xyz;
    //ӳ���� -1~1
    float3 tangentSpaceNormal = UnpackNormal(rawNormal);
    
    //���뷨������z����������
    tangentSpaceNormal.xy *= BumpScale.x;
    tangentSpaceNormal.z = sqrt(1 - saturate(dot(tangentSpaceNormal.xy, tangentSpaceNormal.xy)));
    
    //����ռ䷨������
    float3 worldNormal = normalize(mul(tangentSpaceNormal, TBN));
    
    //����ռ���������
    float3 viewVec = normalize(ViewPos - input.world_pos);
    

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
                
                //����ռ��������
                float3 lightVec = directionalLightData.direction;
                
                //�������
                float3 halfVec = normalize(viewVec + lightVec);

                //�������
                float NdotL = max(0, dot(lightVec, worldNormal));
                float HdotV = max(0, dot(halfVec, viewVec));
                float NdotV = max(0, dot(viewVec, worldNormal));
                float NdotH = max(0, dot(halfVec, worldNormal));
                
                //����������
                float3 F0 = float3(0.04f, 0.04f, 0.04f);
                F0 = lerp(F0, albedo, metallic);
                //��������
                float3 F = FresnelSchlick(HdotV, F0);
                //���߷ֲ���
                float NDF = DistributionGGX(NdotH, roughness);
                //����Ӱ��
                float G = GeometrySmith(NdotV, NdotL, roughness);
                
                float3 numerator = NDF * G * F;
                float denominator = 4.0f * NdotV * NdotL;
                float3 specular = numerator / max(denominator, 0.001f);
                
                float3 kS = F;
                float3 kD = float3(1.0f,1.0f,1.0f) - kS;
  
                kD *= 1.0 - metallic; // ���ڽ������治����⣬û����������ɫ��ͨ������kD��ʵ���������
                
                float PI = 3.14159265359;
                
                color += (kD * albedo / PI + specular) * directionalLightData.color.rgb * NdotL;
                
            }
                break;
         
            case POINT_LIGHT:
            {
                //���ݶ�Ӧ�������ʹ���õ������ֱ�ӹ�����Ϣ
                DirectIlluminationData pointLightData = ProcessPointlLight(Lights[i], input.world_pos);
             
                //����ռ��������
                float3 lightVec = pointLightData.direction;
                
                //�������
                float3 halfVec = normalize(viewVec + lightVec);
                
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
    
    // HDR tonemapping
    color = color / (color + float3(1.0f,1.0f,1.0f));
    // gamma correct
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));


    
    return float4(color, 1.0f);
}
