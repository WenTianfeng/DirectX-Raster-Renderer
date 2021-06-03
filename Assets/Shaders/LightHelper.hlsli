#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2
#define NUM_LIGHTS 1

//光源信息结构体
struct Light
{
    float4 Color;   //光源颜色
    float3 Position;    //光源世界空间位置
    float Range;    //光源范围
    float3 Direction;   //光源方向
    float Intensity;    //光源强度
    uint Type;  //光源类型
    float SpotAngle; //聚光灯发射角度
    float2 Padding; //显式补齐16字节
};

//光源列表结构缓冲
StructuredBuffer<Light> Lights : register(t0);

//经过衰减计算之后的光源信息
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

//处理直射光
DirectIlluminationData ProcessDirectionalLight(Light light,float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
    
    lightData.color = light.Color*light.Intensity/20;
    lightData.direction = normalize(-light.Direction);
    
    return lightData;
}

//处理点光源
DirectIlluminationData ProcessPointlLight(Light light, float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
    
    lightData.direction = normalize(light.Position - pixelWorldPos);
    float lightDistance = distance(light.Position, pixelWorldPos);//光源和片元距离
    
    float distanceAtt = 1 / (1 + lightDistance * lightDistance);
    float rangeAtt = smoothstep(light.Range, 0, lightDistance);
    
    lightData.color = light.Color * 10000 * distanceAtt * rangeAtt * light.Intensity / 20;
    
    return lightData;
}

//处理聚光灯
DirectIlluminationData ProcessSpotLight(Light light, float3 pixelWorldPos)
{
    DirectIlluminationData lightData;
    
    lightData.direction = normalize(light.Position - pixelWorldPos);
    float lightDistance = distance(light.Position, pixelWorldPos); //光源和片元距离
    
    float coneAtt = pow(max(dot(-lightData.direction, normalize(light.Direction)), 0.0f), 180-light.SpotAngle);
    float distanceAtt = 1 / (1 + lightDistance * lightDistance);
    float rangeAtt = smoothstep(light.Range, 0, lightDistance);
        
    lightData.color = light.Color * 10000 * distanceAtt * rangeAtt * coneAtt * light.Intensity / 20;
    
    
    return lightData;
}


