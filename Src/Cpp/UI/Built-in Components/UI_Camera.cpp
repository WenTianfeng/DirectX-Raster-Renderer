#include "UI_Camera.h"

float UI_Camera::camera_Aspect = 0;
float UI_Camera::camera_FovY = 0;
float UI_Camera::camera_NearClip = 0;
float UI_Camera::camera_FarClip = 0;

void UI_Camera::Render()
{
	if (!ImGui::CollapsingHeader("CAMERA"))
	{
		ImGui::DragFloat("Field of View", &camera_FovY, 0.1f, 60.0f, 179.0f, "%.3f");
		ImGui::DragFloat("Aspect", &camera_Aspect, 0.03f, 1.0f, 2.0f, "%.3f");
		ImGui::DragFloat("Near Clip", &camera_NearClip, 0.02f, 0.01f, 0.5f, "%.3f");
		ImGui::DragFloat("Far Clip", &camera_FarClip, 1.0f, 1000.0f, 2000.0f, "%.3f");
	}
}

void UI_Camera::UpdateValues(float fovY, float aspect, float nearClip, float farClip)
{
	camera_FovY = fovY;
	camera_Aspect = aspect;
	camera_NearClip = nearClip;
	camera_FarClip = farClip;
}
