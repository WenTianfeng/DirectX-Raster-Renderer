#pragma once

#include <DirectXMath.h>
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）

struct Vertex3D
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT3 normal;
};


/// <summary>
/// 顶点信息（位置、纹理坐标）
/// </summary>
struct Vertex_PosTex {

	DirectX::XMFLOAT2 pos;//位置坐标
	DirectX::XMFLOAT2 texcoord;//纹理坐标
};