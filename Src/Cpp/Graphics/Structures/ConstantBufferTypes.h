#pragma once

#include<DirectXMath.h>

struct CB_PS_Light
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 pos;
	float range;

};

struct CB_VS_TransformMatrix
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX worldInverseTranspose;
};