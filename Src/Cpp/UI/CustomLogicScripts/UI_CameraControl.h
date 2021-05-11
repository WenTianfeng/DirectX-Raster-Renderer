#pragma once

#include"..\Built-in Components\UI_Component.h"

class UI_CameraControl : public UI_Component
{
public:

	static float cameraControl_MoveSpeed;
	static bool cameraControl_ViewingMode;

public:
	static void Render();
	static void UpdateValues(bool viewingMode, float moveSpeed);
};