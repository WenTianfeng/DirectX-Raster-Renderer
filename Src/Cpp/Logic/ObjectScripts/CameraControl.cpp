#include "CameraControl.h"

void CameraControl::Update(float dt)
{
	this->owner->GetComponent<Transform>()->SetPosition(UI_Transform::transform_Position[0], UI_Transform::transform_Position[1], UI_Transform::transform_Position[2]);
	this->owner->GetComponent<Transform>()->SetRotation(UI_Transform::transform_Rotation[0], UI_Transform::transform_Rotation[1], UI_Transform::transform_Rotation[2]);
}
