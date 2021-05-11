#include "UI_CameraControl.h"

bool UI_CameraControl::cameraControl_ViewingMode = false;
float UI_CameraControl::cameraControl_MoveSpeed = 0;

void UI_CameraControl::Render()
{
	if (!ImGui::CollapsingHeader("Camera Control"))
	{
		ImGui::Checkbox("Viewing Mode", &cameraControl_ViewingMode);
		ImGui::DragFloat("Move Speed", &cameraControl_MoveSpeed, 0.2f, 10.0f, 100.0f, "%.2f");
	}
}

void UI_CameraControl::UpdateValues(bool viewingMode, float moveSpeed)
{

	cameraControl_MoveSpeed = moveSpeed;
	cameraControl_ViewingMode = viewingMode;
	
}
