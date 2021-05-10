#include "Light.h"

Light::Light(LightType lightType, DirectX::XMFLOAT4 lightColor, float intensity, float range):
	m_type(lightType),
	m_color(lightColor),
	m_intensity(intensity),
	m_range(range)
{
	this->m_componentName = "Light";
}

void Light::UpdataProperties(LightType lightType, DirectX::XMFLOAT4 lightColor, float intensity, float range)
{
	this->m_type = lightType;
	this->m_color = lightColor;
	this->m_intensity = intensity;
	this->m_range = range;

}

Light::LightType Light::GetType()
{
	return this->m_type;
}

DirectX::XMFLOAT3 Light::GetPosition()
{
	return this->owner->GetComponent<Transform>()->GetPosition();
}

DirectX::XMFLOAT3 Light::GetDirection()
{
	return this->owner->GetComponent<Transform>()->GetForwardAxis();
}

DirectX::XMFLOAT4 Light::GetColor()
{
	return this->m_color;
}

float Light::GetIntensity()
{
	return this->m_intensity;
}

float Light::GetRange()
{
	return this->m_range;
}
