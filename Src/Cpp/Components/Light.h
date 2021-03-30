#pragma once

#include<DirectXMath.h>

#include"Component.h"

enum class LightType
{
	Spot,
	Directional,
	Point
};

class Light : public Component
{


private:
	LightType m_type;
	DirectX::XMFLOAT4 m_color;
	float m_intensity;

};