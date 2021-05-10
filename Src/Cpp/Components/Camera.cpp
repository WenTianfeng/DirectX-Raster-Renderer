#include"Camera.h"

Camera::Camera():
	m_fovY(DirectX::XM_PI / 3),
	m_aspect(2.0f),
	m_nearClip(0.1f),
	m_farClip(2000.0f)
{
}

Camera::Camera(float fovY, float aspect, float nearClip, float farClip) :
	m_fovY(fovY),
	m_aspect(aspect),
	m_nearClip(nearClip),
	m_farClip(farClip)
{
	this->m_componentName = "Camera";
}

void Camera::UpdateProperties(float fovY, float aspect, float nearClip, float farClip)
{
	this->m_aspect = aspect;
	this->m_fovY = fovY;
	this->m_nearClip = nearClip;
	this->m_farClip = farClip;
}

void Camera::SetAspect(float aspect)
{
	this->m_aspect = aspect;
}

float Camera::GetFovY()
{
	return this->m_fovY;
}

float Camera::GetAspect()
{
	return this->m_aspect;
}

float Camera::GetNearClip()
{
	return this->m_nearClip;
}

float Camera::GetFarClip()
{
	return this->m_farClip;
}


DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return this->owner->GetComponent<Transform>()->GetWorldToLocalMatrixXM();
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearClip, m_farClip);
}