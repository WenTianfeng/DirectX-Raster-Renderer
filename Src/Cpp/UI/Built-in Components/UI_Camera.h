#pragma once

#include<DirectXMath.h>
#include"UI_Component.h"

class UI_Camera : public UI_Component
{
public:
	static float camera_FovY;
	static float camera_Aspect;
	static float camera_NearClip;
	static float camera_FarClip;

public:
	static void Render();
	static void UpdateValues(float fovY, float aspect, float nearClip, float farClip);
};