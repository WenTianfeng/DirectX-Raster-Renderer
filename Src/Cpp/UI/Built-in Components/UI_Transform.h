#pragma once

#include<DirectXMath.h>

#include"UI_Component.h"

class UI_Transform:	public UI_Component
{
public:
	
	static float transform_Position[3];
	static float transform_Rotation[3];
	static float transform_Scale[3];

public:
	static void Render();
	static void UpdateValues(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
};