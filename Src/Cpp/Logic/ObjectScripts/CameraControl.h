#pragma once

#include"..\..\Components\Transform.h"
#include"..\..\Object\Object.h"
#include"..\..\UI\Built-in Components\UI_Transform.h"

class CameraControl : public Component
{
public:
	CameraControl() = default;
	void Update(float dt);



};