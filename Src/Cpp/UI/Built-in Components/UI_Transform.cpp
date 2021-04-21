#include"UI_Transform.h"

float UI_Transform::transform_Position[3] = { 0.0f,20.0f,-80.0f };
float UI_Transform::transform_Rotation[3] = { 0.0f,0.0f,0.0f };
float UI_Transform::transform_Scale[3] = { 1.0f,1.0f,1.0f };



void UI_Transform::Render()
{
	if (!ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Position", transform_Position, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Rotation", transform_Rotation, 0.02f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Scale", transform_Scale, 0.1f, 0.0f, 0.0f, "%.2f");
	}

}
