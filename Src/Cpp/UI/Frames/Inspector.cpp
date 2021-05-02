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
			//获取从Hierarchy中选中的Object对象
			Object* object = SceneManager::objects[preSelected];

//============如果Object拥有对应组件，则显示对应UI===========

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

			//显示Transform UI
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

				//Transfrom UI渲染
				UI_Transform::Render();
			}

			#pragma endregion

			#pragma region MeshRenderer UI

			//显示MeshRenderer UI
			if (object->HasComponent<MeshRenderer>())
			{


			}

			#pragma endregion 

			#pragma region Skybox UI

			//显示天空盒UI
			if (object->HasComponent<Skybox>())
			{


			}

			#pragma endregion 

			#pragma region MaterialManager UI

			if (object->HasComponent<MaterialManager>())
			{
				if (!changed)
				{
					//遍历所有的UI_Material
					for (UINT i = 0; i < UI_MaterialManager::ui_Materials.size(); i++)
					{
						//遍历UI_Material 的所有变量
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
					//清空UI_MaterialManagerd的UI_Material列表
					UI_MaterialManager::ui_Materials.clear();

					//遍历MaterialManager中的所有材质
					for (UINT i = 0; i < object->GetComponent<MaterialManager>()->materials.size(); i++)
					{
						//实例化新的材质UI
						UI_Material ui_Material;

						//遍历材质的所有着色器资源
						for (std::map<std::string, ShaderParameter*>::value_type pair_name_shaderParameter : object->GetComponent<MaterialManager>()->materials[i].shaderParametersMap)
						{
							//根据着色器资源参数类型分别处理
							switch (pair_name_shaderParameter.second->GetShaderParameterType())
							{
								//当资源类型为 ConstantBuffer
								case ShaderParameter::ShaderParameterType::ConstantBuffer:
								{
									//遍历常量缓冲的变量map，将获取的变量转换为ImGui接受的形式，加入到材质UI变量列表中
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

						//将 UI_Material 加入到 UI_MaterialManager 的 UI_Material 列表
						UI_MaterialManager::ui_Materials.push_back(ui_Material);

					}
				}

				//MaterialManager UI渲染
				UI_MaterialManager::Render();

			}

			#pragma endregion 
			
			#pragma region Light UI

			//显示Light信息UI
			if (object->HasComponent<Light>())
			{


			}

			#pragma endregion 

			#pragma region Camera UI

			//显示Camera UI
			if (object->HasComponent<Camera>())
			{


			}

			#pragma endregion 

			#pragma region Modify Component Button UI

			//显示修改组件按钮
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

