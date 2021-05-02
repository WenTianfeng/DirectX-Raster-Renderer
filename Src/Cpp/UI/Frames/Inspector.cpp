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
			//��ȡ��Hierarchy��ѡ�е�Object����
			Object* object = SceneManager::objects[preSelected];

//============���Objectӵ�ж�Ӧ���������ʾ��ӦUI===========

			#pragma region Attributes UI

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

			#pragma endregion

			#pragma region Transform UI

			//��ʾTransform UI
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
				if (!changed)
				{
					//�������е�UI_Material
					for (UINT i = 0; i < UI_MaterialManager::ui_Materials.size(); i++)
					{
						//����UI_Material �����б���
						for (UINT j = 0; j < UI_MaterialManager::ui_Materials[i].imGuiAvailableVariables.size(); j++)
						{
							ImGuiAvailableVariable imGuiAvailableVariable = UI_MaterialManager::ui_Materials[i].imGuiAvailableVariables[j];
							ConstantBufferVariable cbVariable = imGuiAvailableVariable.ownerBuffer->GetConstantBufferVariableByName(imGuiAvailableVariable.name);


							if (imGuiAvailableVariable.type == ImGuiAvailableVariableType::VT_FLOAT)
							{
								imGuiAvailableVariable.ownerBuffer->SetFloatVec(imGuiAvailableVariable.size / sizeof(float), cbVariable.startOffset, cbVariable.size, imGuiAvailableVariable.floatVec);
							}
							else if (imGuiAvailableVariable.type == ImGuiAvailableVariableType::VT_INT)
							{
								imGuiAvailableVariable.ownerBuffer->SetIntVec(imGuiAvailableVariable.size / sizeof(float), cbVariable.startOffset, cbVariable.size, imGuiAvailableVariable.intVec);
							}
						}
					}
				}
				else
				{
					//���UI_MaterialManagerd��UI_Material�б�
					UI_MaterialManager::ui_Materials.clear();

					//����MaterialManager�е����в���
					for (UINT i = 0; i < object->GetComponent<MaterialManager>()->materials.size(); i++)
					{
						//ʵ�����µĲ���UI
						UI_Material ui_Material;

						//�������ʵ�������ɫ����Դ
						for (std::map<std::string, ShaderParameter*>::value_type pair_name_shaderParameter : object->GetComponent<MaterialManager>()->materials[i].shaderParametersMap)
						{
							//������ɫ����Դ�������ͷֱ���
							switch (pair_name_shaderParameter.second->GetShaderParameterType())
							{
								//����Դ����Ϊ ConstantBuffer
								case ShaderParameter::ShaderParameterType::ConstantBuffer:
								{
									//������������ı���map������ȡ�ı���ת��ΪImGui���ܵ���ʽ�����뵽����UI�����б���
									for (std::map<std::string, ConstantBufferVariable>::value_type pair_name_constantBufferVariable : pair_name_shaderParameter.second->constantBuffer->constantVariablesMap)
									{
										ImGuiAvailableVariable generalVariable(pair_name_constantBufferVariable.second, pair_name_shaderParameter.second->constantBuffer);
										ui_Material.imGuiAvailableVariables.push_back(generalVariable);
									}
								}
									break;

								case ShaderParameter::ShaderParameterType::Sampler:
									break;
								case ShaderParameter::ShaderParameterType::Texture:
									break;
								case ShaderParameter::ShaderParameterType::StructuredBuffer:
									break;
								case ShaderParameter::ShaderParameterType::Invalid:
									break;
							}

						}

						//�� UI_Material ���뵽 UI_MaterialManager �� UI_Material �б�
						UI_MaterialManager::ui_Materials.push_back(ui_Material);

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


			}

			#pragma endregion 

			#pragma region Camera UI

			//��ʾCamera UI
			if (object->HasComponent<Camera>())
			{


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

			#pragma endregion

		}
	}

	ImGui::End();

}

