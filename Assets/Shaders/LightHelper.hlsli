#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2
#define NUM_LIGHTS 1

//��Դ��Ϣ�ṹ��
struct Light
{
    float4 Color;   //��Դ��ɫ
    float3 Position;    //��Դ����ռ�λ��
    float Range;    //��Դ��Χ
    float3 Direction;   //��Դ����
    float Intensity;    //��Դǿ��
    uint Type;  //��Դ����
    float SpotAngle; //�۹�Ʒ���Ƕ�
    float2 Padding; //��ʽ����16�ֽ�
};

//��Դ�б�ṹ����
StructuredBuffer<Light> Lights : register(t0);

//����˥������֮��Ĺ�Դ��Ϣ
struct DirectIlluminationData
{
    float4 color;
    float3 direction;
};

struct IndirectIlluminationData
{
    float3 diffuse;
    float3 specular;
};

//����ֱ���
DirectIlluminationData ProcessDirectionalLight(Light light,float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
    
    lightData.color = light.Color*light.Intensity/20;
    lightData.direction = normalize(-light.Direction);
    
    return lightData;
}

//������Դ
DirectIlluminationData ProcessPointlLight(Light light, float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
    
    lightData.direction = normalize(light.Position - pixelWorldPos);
    float lightDistance = distance(light.Position, pixelWorldPos);//��Դ��ƬԪ����
    
    float distanceAtt = 1 / (1 + lightDistance * lightDistance);
    float rangeAtt = smoothstep(light.Range, 0, lightDistance);
    
    lightData.color = light.Color * 10000 * distanceAtt * rangeAtt * light.Intensity / 20;
    
    return lightData;
}

//����۹��
DirectIlluminationData ProcessSpotLight(Light light, float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
    
    lightData.direction = normalize(light.Position - pixelWorldPos);
    float lightDistance = distance(light.Position, pixelWorldPos); //��Դ��ƬԪ����
    
    float coneAtt = pow(max(dot(-lightData.direction, normalize(light.Direction)), 0.0f), 180-light.SpotAngle);
    float distanceAtt = 1 / (1 + lightDistance * lightDistance);
    float rangeAtt = smoothstep(light.Range, 0, lightDistance);
        
    lightData.color = light.Color * 10000 * distanceAtt * rangeAtt * coneAtt * light.Intensity / 20;
    
    
    return lightData;
}


