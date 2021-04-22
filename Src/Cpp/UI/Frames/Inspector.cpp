#include"Inspector.h"
#include"..\..\Logic\SceneManager.h"
#include"..\..\Components\Skybox.h"
#include"..\..\Components\Light.h"
#include"..\..\Logic\ObjectScripts\CameraControl.h"
#include"..\..\Components\Attributes.h"

int Inspector::preSelected = -1;

void Inspector::Initialize()
{

}

void Inspector::Render()
{
	bool changed;

	if (preSelected == Hierarchy::selected)
	{
		changed = false;
	}
	else
	{
		changed = true;
		preSelected = Hierarchy::selected;
	}


	if (ImGui::Begin("INSPECTOR"))
	{
		if (preSelected != -1)
		{
			Object* object = SceneManager::objects[preSelected];

			if (object->HasComponent<Attributes>())
			{
				if (!changed)
				{
					object->SetActivation(UI_Attributes::attributes_Active);
					object->GetComponent<Attributes>()->UpdateProperties(UI_Attributes::attributes_ObjectName, UI_Attributes::attributes_Tag);
				}
				else
				{
					UI_Attributes::UpdateValues(
						object->GetComponent<Attributes>()->ObjectName(),
						object->IsActive(),
						object->GetComponent<Attributes>()->ObjectTag()
					);
				}

				UI_Attributes::Render();
			}


			if (object->HasComponent<Transform>())
			{
				if (!changed)
				{
					object->GetComponent<Transform>()->UpdateProperties(
						DirectX::XMFLOAT3(UI_Transform::transform_Position[0], UI_Transform::transform_Position[1], UI_Transform::transform_Position[2]),
						DirectX::XMFLOAT3(UI_Transform::transform_Rotation[0], UI_Transform::transform_Rotation[1], UI_Transform::transform_Rotation[2]),
						DirectX::XMFLOAT3(UI_Transform::transform_Scale[0], UI_Transform::transform_Scale[1], UI_Transform::transform_Scale[2])
					);
				}
				else
				{
					UI_Transform::UpdateValues(
						object->GetComponent<Transform>()->GetPosition(),
						object->GetComponent<Transform>()->GetRotation(),
						object->GetComponent<Transform>()->GetScale()
					);
				}

				UI_Transform::Render();
			}

			if (object->HasComponent<MeshRenderer>())
			{


			}

			if (object->HasComponent<Skybox>())
			{


			}


			if (object->HasComponent<MaterialManager>())
			{
				for (UINT i = 0; i < object->GetComponent<MaterialManager>()->materials.size(); i++)
				{


				}

			}


			if (object->HasComponent<Light>())
			{


			}



			if (object->HasComponent<Camera>())
			{


			}

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Indent(50.0f);
			if (ImGui::Button("Modify Component"))
			{
				ImGui::OpenPopup("my_toggle_popup");
				ImGui::Unindent();
			}

			if (ImGui::BeginPopup("my_toggle_popup"))
			{
				ImGui::MenuItem("Built-in Components", NULL, false, false);
				ImGui::Separator();
				if (ImGui::BeginMenu("Sub-menu"))
				{
					ImGui::MenuItem("Click me");
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
		}
	}

	ImGui::End();

}

void Inspector::RefreshUIList()
{


}
