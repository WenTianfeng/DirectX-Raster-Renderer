#pragma once

#include"..\..\Components\Transform.h"
#include"..\..\Object\Object.h"

#include"..\..\Keyboard\Keyboard.h"
#include"..\..\Mouse\Mouse.h"


class CameraControl : public Component
{
private:
	float m_moveSpeed;
	bool m_viewingMode;

public:
	CameraControl();
	void Update(float dt);
	void UpdateProperties(bool viewingMode, float moveSpeed);

	bool GetViewingMode();
	float GetMoveSpeed();
	void SetExamineMode(bool);

};