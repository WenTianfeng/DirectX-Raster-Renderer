#pragma once

#include<DirectXMath.h>

#include"Transform.h"
#include"..\Logic\Object.h"

class Light : public Component
{
public:
	enum class LightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2
	};

public:
	Light();
	Light(LightType lightType, DirectX::XMFLOAT4 lightColor,float intensity,float range, float spotAngle);

	void UpdateProperties(LightType lightType, DirectX::XMFLOAT4 lightColor, float intensity, float range, float spotAngle);

	LightType GetType();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetDirection();
	DirectX::XMFLOAT4 GetColor();
	float GetIntensity();
	float GetRange();
	float GetSpotAngle();

private:

	LightType m_type;
	DirectX::XMFLOAT4 m_color;
	float m_intensity;
	float m_range;
	float m_spotAngle;
};