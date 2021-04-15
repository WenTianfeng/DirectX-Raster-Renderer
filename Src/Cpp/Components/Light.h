#pragma once

#include<DirectXMath.h>

#include"Transform.h"
#include"..\Object\Object.h"

class Light : public Component
{
public:
	enum class LightType
	{
		Directional = 0,
		Spot = 1,
		Point = 2
	};

public:
	Light(LightType lightType, DirectX::XMFLOAT4 lightColor,float intensity,float range);

	LightType GetType();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetDirection();
	DirectX::XMFLOAT4 GetColor();
	float GetIntensity();
	float GetRange();


private:

	LightType m_type;
	DirectX::XMFLOAT4 m_color;
	float m_intensity;
	float m_range;
};