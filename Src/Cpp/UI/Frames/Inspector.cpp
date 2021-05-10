#include"Inspector.h"



int Inspector::preSelected = -1;

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
			//��ȡ��Hierarchy��ѡ�е�Object����
			Object* object = SceneManager::objects[preSelected];

//============���Objectӵ�ж�Ӧ���������ʾ��ӦUI===========

			#pragma region Attributes UI

			if (object->HasComponent<Attributes>())
			{
				if (changed)
				{
					UI_Attributes::UpdateValues(
						object->GetComponent<Attributes>()->ObjectName(),
						object->IsActive(),
						object->GetComponent<Attributes>()->ObjectTag()
					);
				}
				else
				{
					object->SetActivation(UI_Attributes::attributes_Active);
					object->GetComponent<Attributes>()->UpdateProperties(UI_Attributes::attributes_ObjectName, UI_Attributes::attributes_Tag);
				}

				UI_Attributes::Render();
			}

			#pragma endregion

			#pragma region Transform UI

			//��ʾTransform UI
			if (object->HasComponent<Transform>())
			{
				if (changed)
				{
					//������ת��Ϊ�Ƕ�
					DirectX::XMFLOAT3 rotationInDegree = DirectX::XMFLOAT3(
						DirectX::XMConvertToDegrees(object->GetComponent<Transform>()->GetRotation().x),
						DirectX::XMConvertToDegrees(object->GetComponent<Transform>()->GetRotation().y),
						DirectX::XMConvertToDegrees(object->GetComponent<Transform>()->GetRotation().z));

					UI_Transform::UpdateValues(
						object->GetComponent<Transform>()->GetPosition(),
						rotationInDegree,
						object->GetComponent<Transform>()->GetScale()
					);
				}
				else
				{
					object->GetComponent<Transform>()->UpdateProperties(
						DirectX::XMFLOAT3(UI_Transform::transform_Position[0], UI_Transform::transform_Position[1], UI_Transform::transform_Position[2]),

						DirectX::XMFLOAT3(
							DirectX::XMConvertToRadians(UI_Transform::transform_Rotation[0]),
							DirectX::XMConvertToRadians(UI_Transform::transform_Rotation[1]),
							DirectX::XMConvertToRadians(UI_Transform::transform_Rotation[2])),

						DirectX::XMFLOAT3(UI_Transform::transform_Scale[0], UI_Transform::transform_Scale[1], UI_Transform::transform_Scale[2])
					);
				}

				//Transfrom UI��Ⱦ
				UI_Transform::Render();
			}

			#pragma endregion

			#pragma region MeshRenderer UI

			//��ʾMeshRenderer UI
			if (object->HasComponent<MeshRenderer>())
			{


			}

			#pragma endregion 

			#pragma region Skybox UI

			//��ʾ��պ�UI
			if (object->HasComponent<Skybox>())
			{


			}

			#pragma endregion 

			#pragma region MaterialManager UI

			if (object->HasComponent<MaterialManager>())
			{
				//------------------------------------------------------------------���ѡ�����µĶ���--------------------------------------------------------------------
				if (changed)
				{
					//���UI_MaterialManagerd��UI_Material�б�
					UI_MaterialManager::ui_Materials.clear();

					//����MaterialManager�е����в���
					for (UINT i = 0; i < object->GetComponent<MaterialManager>()->materials.size(); i++)
					{
						//ʵ�����µĲ���UI
						UI_Material ui_Material(&object->GetComponent<MaterialManager>()->materials[i]);

						//���ݲ������ݶ���UI���г�ʼ��
						ui_Material.Initialize(object->GetComponent<MaterialManager>()->materials[i]);

						//�� UI_Material ���뵽 UI_MaterialManager �� UI_Material �б�
						UI_MaterialManager::ui_Materials.push_back(ui_Material);
					}
				}
				//------------------------------------------------------------------û��ѡ���¶�������������޸Ĳ���--------------------------------------------------------------------
				else
				{
					//�������е�UI_Material
					for (UINT i = 0; i < UI_MaterialManager::ui_Materials.size(); i++)
					{
						//===================����UI_Material ������ displayableVariable���������Ӧ����ɫ����Դ����=================
						for (UINT j = 0; j < UI_MaterialManager::ui_Materials[i].displayableVariables.size(); j++)
						{
							UI_MaterialManager::ui_Materials[i].displayableVariables[j].UpdateOwnerParameterValues();
						}

						//===================����ڲ���UI�޸�������ɫ��·������ôʹ����·������ʵ�������ʣ����ʶ��󲻱䣩�����س�ʼ������UI==================
						if (UI_MaterialManager::ui_Materials[i].shaderFilePath != UI_MaterialManager::ui_Materials[i].ownerMaterial->GetShaderFilePath())
						{
							UI_MaterialManager::ui_Materials[i].ownerMaterial->Instantiate(UI_MaterialManager::ui_Materials[i].shaderFilePath);
							UI_MaterialManager::ui_Materials[i].Initialize(*UI_MaterialManager::ui_Materials[i].ownerMaterial);
						}
					}
				}

				//MaterialManager UI��Ⱦ
				UI_MaterialManager::Render();

			}

			#pragma endregion 
			
			#pragma region Light UI

			//��ʾLight��ϢUI
			if (object->HasComponent<Light>())
			{
				//------------------------------------------------------------------���ѡ�����µĶ���--------------------------------------------------------------------
				if (changed)
				{
					UI_Light::UpdateValues(
						object->GetComponent<Light>()->GetType(),
						object->GetComponent<Light>()->GetColor(),
						object->GetComponent<Light>()->GetIntensity(),
						object->GetComponent<Light>()->GetRange()
					);
				}
				//------------------------------------------------------------------û��ѡ���¶�������������޸Ĳ���--------------------------------------------------------------------
				else
				{
					object->GetComponent<Light>()->UpdataProperties(
						UI_Light::light_Type,

						DirectX::XMFLOAT4(
							UI_Light::light_Color[0],
							UI_Light::light_Color[1],
							UI_Light::light_Color[2],
							UI_Light::light_Color[3]),

						UI_Light::light_Intensity,
						UI_Light::light_Range
					);
				}

				UI_Light::Render();

			}

			#pragma endregion 

			#pragma region Camera UI

			//��ʾCamera UI
			if (object->HasComponent<Camera>())
			{
				//------------------------------------------------------------------���ѡ�����µĶ���--------------------------------------------------------------------
				if (changed)
				{
					UI_Camera::UpdateValues(
						DirectX::XMConvertToDegrees(object->GetComponent<Camera>()->GetFovY()), 
						object->GetComponent<Camera>()->GetAspect(), 
						object->GetComponent<Camera>()->GetNearClip(), 
						object->GetComponent<Camera>()->GetFarClip());
				}
				//------------------------------------------------------------------û��ѡ���¶�������������޸Ĳ���--------------------------------------------------------------------
				else
				{
					object->GetComponent<Camera>()->UpdateProperties(
						DirectX::XMConvertToRadians(UI_Camera::camera_FovY),
						UI_Camera::camera_Aspect,
						UI_Camera::camera_NearClip,
						UI_Camera::camera_FarClip);
				}

				UI_Camera::Render();

			}

			#pragma endregion 

			#pragma region Modify Component Button UI

			//��ʾ�޸������ť
			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Indent(50.0f);
			if (ImGui::Button("Modify Component"))
			{
				ImGui::OpenPopup("my_toggle_popup");
			}
			ImGui::Unindent(50.0f);
			if (ImGui::BeginPopup("my_toggle_popup"))
			{
				ImGui::MenuItem("Built-in Components", NULL, false, false);

				static int componentNameSelected = -1;

				//���������ļ�ѡ���
				for (UINT n = 0; n < builtInComponentNames.size(); n++)
				{
					if (ImGui::Selectable(builtInComponentNames[n].c_str(), componentNameSelected == n))
					{
						componentNameSelected = n;
					}
				}

				ImGui::EndPopup();
			}

			#pragma endregion
		}
	}

	ImGui::End();

}

void Inspector::AddComponentToObject(Object* object, std::string componentName)
{
	if (componentName == "Transform")
	{
		if (!object->HasComponent<Transform>())
		{
			object->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
		}
	}

	else if (componentName == "MeshRenderer")
	{
		if (!object->HasComponent<MeshRenderer>())
		{

		}
	}

	else if (componentName == "MaterialManager")
	{
		if (!object->HasComponent<MaterialManager>())
		{

		}
	}


}


