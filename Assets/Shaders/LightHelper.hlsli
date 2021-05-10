#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2
#define NUM_LIGHTS 2

//��Դ��Ϣ�ṹ��
struct Light
{
    float4 Color;   //��Դ��ɫ
    float3 Position;    //��Դ����ռ�λ��
    float Range;    //��Դ��Χ
    float3 Direction;   //��Դ����
    float Intensity;    //��Դǿ��
    uint Type;  //��Դ����
    float3 Padding; //��ʽ����16�ֽ�
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
    
    lightData.color = light.Color;
    lightData.direction = light.Direction;
    
    return lightData;
}

//������Դ
DirectIlluminationData ProcessPointlLight(Light light, float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
        
    lightData.color = light.Color;
    lightData.direction = light.Direction;
    
    return lightData;
}

//����۹��
DirectIlluminationData ProcessSpotLight(Light light, float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
    
        
    lightData.color = light.Color;
    lightData.direction = light.Direction;
    
    return lightData;
}


