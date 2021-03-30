#pragma once

#include<DirectXMath.h>

#include"Transform.h"
#include"..\Object\Object.h"

class Camera : public Component {

public:
	Camera(float fovY, float aspect, float nearClip, float farClip);

	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMMATRIX GetProjectionMatrix();

private:
	float m_fovY;
	float m_aspect;
	float m_nearClip;
	float m_farClip;

};