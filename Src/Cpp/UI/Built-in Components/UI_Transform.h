#pragma once

#include"UI_Component.h"

class UI_Transform:	public UI_Component
{
public:
	
	static float transform_Position[3];
	static float transform_Rotation[3];
	static float transform_Scale[3];

public:
	void Render();

};