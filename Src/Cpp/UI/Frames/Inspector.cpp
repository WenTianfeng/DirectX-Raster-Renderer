#include"Inspector.h"

int Inspector::preSelected = -1;

void Inspector::Initialize(SceneManager* sceneManager)
{
	this->m_sceneManager = sceneManager;
}

void Inspector::Render(int windowWidth, int windowHeight)
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

	//设置Inspector窗口样式
	ImGuiWindowFlags inspectorWindowFlags = ImGuiWindowFlags_NoMove;//禁止移动

	if (ImGui::Begin("INSPECTOR",NULL, inspectorWindowFlags))
	{
		if (preSelected != -1)
		{
			//获取从Hierarchy中选中的Object对象
			Object* object = m_sceneManager->GetObjects()[preSelected];

			//============如果Object拥有对应组件，则显示对应UI===========

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

			//显示Transform UI
			if (object->HasComponent<Transform>())
			{
				if (changed)
				{
					//将弧度转换为角度
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
				//------------------------------------------------------------------如果选中了新的对象--------------------------------------------------------------------
				if (changed)
				{
					//清空UI_MaterialManagerd的UI_Material列表
					UI_MaterialManager::ui_Materials.clear();

					//遍历MaterialManager中的所有材质
					for (UINT i = 0; i < object->GetComponent<MaterialManager>()->materials.size(); i++)
					{
						//实例化新的材质UI
						UI_Material ui_Material(&object->GetComponent<MaterialManager>()->materials[i]);

						//根据材质内容对其UI进行初始化
						ui_Material.Initialize(object->GetComponent<MaterialManager>()->materials[i]);

						//将 UI_Material 加入到 UI_MaterialManager 的 UI_Material 列表
						UI_MaterialManager::ui_Materials.push_back(ui_Material);
					}
				}
				//------------------------------------------------------------------没有选中新对象，则根据输入修改参数--------------------------------------------------------------------
				else
				{
					//遍历所有的UI_Material
					for (UINT i = 0; i < UI_MaterialManager::ui_Materials.size(); i++)
					{
						//===================遍历UI_Material 的所有 displayableVariable，更新其对应的着色器资源参数=================
						for (UINT j = 0; j < UI_MaterialManager::ui_Materials[i].displayableVariables.size(); j++)
						{
							UI_MaterialManager::ui_Materials[i].displayableVariables[j].UpdateOwnerParameterValues();
						}

						//===================如果在材质UI修改了其着色器路径，那么使用新路径重新实例化材质（材质对象不变），并重初始化材质UI==================
						if (UI_MaterialManager::ui_Materials[i].shaderFilePath != UI_MaterialManager::ui_Materials[i].ownerMaterial->GetShaderFilePath())
						{
							UI_MaterialManager::ui_Materials[i].ownerMaterial->Instantiate(UI_MaterialManager::ui_Materials[i].shaderFilePath);
							UI_MaterialManager::ui_Materials[i].Initialize(*UI_MaterialManager::ui_Materials[i].ownerMaterial);
						}
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
				//------------------------------------------------------------------如果选中了新的对象--------------------------------------------------------------------
				if (changed)
				{
					UI_Light::UpdateValues(
						object->GetComponent<Light>()->GetType(),
						object->GetComponent<Light>()->GetColor(),
						object->GetComponent<Light>()->GetIntensity(),
						object->GetComponent<Light>()->GetRange(),
						object->GetComponent<Light>()->GetSpotAngle()
					);
				}
				//------------------------------------------------------------------没有选中新对象，则根据输入修改参数--------------------------------------------------------------------
				else
				{
					object->GetComponent<Light>()->UpdateProperties(
						UI_Light::light_Type,

						DirectX::XMFLOAT4(
							UI_Light::light_Color[0],
							UI_Light::light_Color[1],
							UI_Light::light_Color[2],
							UI_Light::light_Color[3]),

						UI_Light::light_Intensity,
						UI_Light::light_Range,
						UI_Light::light_SpotAngle
					);
				}

				UI_Light::Render();

			}

			#pragma endregion 

			#pragma region Camera UI

			//显示Camera UI
			if (object->HasComponent<Camera>())
			{
				//------------------------------------------------------------------如果选中了新的对象--------------------------------------------------------------------
				if (changed)
				{
					UI_Camera::UpdateValues(
						DirectX::XMConvertToDegrees(object->GetComponent<Camera>()->GetFovY()), 
						object->GetComponent<Camera>()->GetAspect(), 
						object->GetComponent<Camera>()->GetNearClip(), 
						object->GetComponent<Camera>()->GetFarClip());
				}
				//------------------------------------------------------------------没有选中新对象，则根据输入修改参数--------------------------------------------------------------------
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

			#pragma region Object Operation UI

			ImGui::Spacing();
			ImGui::Separator();

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("my_toggle_popup");
			}

			ImGui::SameLine();
			if (ImGui::Button("Delete Object"))
			{
				this->m_sceneManager->DeleteObject(object);
				preSelected = -1;
				Hierarchy::selected = -1;
			}

			if (ImGui::BeginPopup("my_toggle_popup"))
			{
				ImGui::MenuItem("Built-in Components", NULL, false, false);

				//绘制组件选择框
				for (UINT n = 0; n < builtInComponentNames.size(); n++)
				{
					if (ImGui::MenuItem(builtInComponentNames[n].c_str()))
					{
						//组件添加
						if (builtInComponentNames[n] == "MeshRenderer")
						{
							//如果对象没有此类组件
							if (!object->HasComponent<MeshRenderer>())
							{

							}
							else
							{
								//否则打印错误信息
							}
						}
						else if (builtInComponentNames[n] == "MaterialManager")
						{

						}
					}
				}

				ImGui::EndPopup();
			}

			#pragma endregion
		}
	}

	ImGui::End();

	//Inspector自适应窗口变化
	ImGui::SetWindowPos("INSPECTOR", ImVec2(windowWidth-300.0f, 20));
	ImGui::SetWindowSize("INSPECTOR", ImVec2(300, windowHeight - 20.0f));

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


