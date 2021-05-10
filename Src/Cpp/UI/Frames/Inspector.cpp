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
			//获取从Hierarchy中选中的Object对象
			Object* object = SceneManager::objects[preSelected];

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
						object->GetComponent<Light>()->GetRange()
					);
				}
				//------------------------------------------------------------------没有选中新对象，则根据输入修改参数--------------------------------------------------------------------
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

			#pragma region Modify Component Button UI

			//显示修改组件按钮
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

				//绘制纹理文件选择框
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


