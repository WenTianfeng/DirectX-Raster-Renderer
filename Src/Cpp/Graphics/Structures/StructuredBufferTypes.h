#pragma once

#include <wrl/client.h>
#include<DirectXMath.h>

struct SB_PS_Light
{
    DirectX::XMFLOAT4 Color;   //光源颜色
    DirectX::XMFLOAT3 Position;    //光源世界空间位置
    float Range;    //光源范围
    DirectX::XMFLOAT3 Direction;   //光源方向
    float Intensity;    //光源强度
    UINT Type;  //光源类型
    float SpotAngle; //聚光灯发射角度
    DirectX::XMFLOAT2 Padding; //显式补齐16字节

};