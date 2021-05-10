#include"UI_Transform.h"

float UI_Transform::transform_Position[3] = { 0.0f,0.0f,0.0f };
float UI_Transform::transform_Rotation[3] = { 0.0f,0.0f,0.0f };
float UI_Transform::transform_Scale[3] = { 1.0f,1.0f,1.0f };



void UI_Transform::Render()
{
	if (!ImGui::CollapsingHeader("TRANSFORM"))
	{
		ImGui::DragFloat3("Position", transform_Position, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Rotation", transform_Rotation, 0.02f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("Scale", transform_Scale, 0.1f, 0.0f, 0.0f, "%.2f");
	}

}

void UI_Transform::UpdateValues(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	transform_Position[0] = position.x;
	transform_Position[1] = position.y;
	transform_Position[2] = position.z;

	transform_Rotation[0] = rotation.x;
	transform_Rotation[1] = rotation.y;
	transform_Rotation[2] = rotation.z;

	transform_Scale[0] = scale.x;
	transform_Scale[1] = scale.y;
	transform_Scale[2] = scale.z;
}


