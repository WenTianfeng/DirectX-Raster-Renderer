#pragma once

#include<DirectXMath.h>

#include"Object.h"
#include"Transform.h"

class Camera :public Component {

public:
	Camera() = default;

	Camera(float fovY, float aspect, float nearClip, float farClip);

	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMMATRIX GetProjectionMatrix();

private:
	float m_fovY;
	float m_aspect;
	float m_nearClip;
	float m_farClip;

};