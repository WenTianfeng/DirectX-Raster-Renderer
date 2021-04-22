#include"Camera.h"

Camera::Camera(float fovY, float aspect, float nearClip, float farClip) :
	m_fovY(fovY),
	m_aspect(aspect),
	m_nearClip(nearClip),
	m_farClip(farClip)
{
	this->m_componentName = "Camera";
}


DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return this->owner->GetComponent<Transform>()->GetWorldToLocalMatrixXM();
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearClip, m_farClip);
}