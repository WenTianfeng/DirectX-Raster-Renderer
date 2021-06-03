#include "Light.h"

Light::Light():
	m_type(LightType::Directional),
	m_color(DirectX::XMFLOAT4(1,1,1,1)),
	m_intensity(1.0f),
	m_range(1000.0f),
	m_spotAngle(60.0f)
{
	this->m_componentName = "Light";
}

Light::Light(LightType lightType, DirectX::XMFLOAT4 lightColor, float intensity, float range, float spotAngle):
	m_type(lightType),
	m_color(lightColor),
	m_intensity(intensity),
	m_range(range),
	m_spotAngle(spotAngle)
{
	this->m_componentName = "Light";
}

void Light::UpdateProperties(LightType lightType, DirectX::XMFLOAT4 lightColor, float intensity, float range, float spotAngle)
{
	this->m_type = lightType;
	this->m_color = lightColor;
	this->m_intensity = intensity;
	this->m_range = range;
	this->m_spotAngle = spotAngle;

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

float Light::GetSpotAngle()
{
	return this->m_spotAngle;
}
