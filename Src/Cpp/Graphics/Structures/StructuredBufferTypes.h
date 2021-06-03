#pragma once

#include <wrl/client.h>
#include<DirectXMath.h>

struct SB_PS_Light
{
    DirectX::XMFLOAT4 Color;   //��Դ��ɫ
    DirectX::XMFLOAT3 Position;    //��Դ����ռ�λ��
    float Range;    //��Դ��Χ
    DirectX::XMFLOAT3 Direction;   //��Դ����
    float Intensity;    //��Դǿ��
    UINT Type;  //��Դ����
    float SpotAngle; //�۹�Ʒ���Ƕ�
    DirectX::XMFLOAT2 Padding; //��ʽ����16�ֽ�

};