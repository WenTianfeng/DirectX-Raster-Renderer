#pragma once

#include<DirectXMath.h>

#include"Transform.h"
#include"..\Object\Object.h"

class Camera : public Component {

public:
	Camera();
	Camera(float fovY, float aspect, float nearClip, float farClip);

	void UpdateProperties(float fovY, float aspect, float nearClip, float farClip);

	void SetAspect(float aspect);

	float GetFovY();
	float GetAspect();
	float GetNearClip();
	float GetFarClip();

	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMMATRIX GetProjectionMatrix();

private:
	float m_fovY;
	float m_aspect;
	float m_nearClip;
	float m_farClip;

};