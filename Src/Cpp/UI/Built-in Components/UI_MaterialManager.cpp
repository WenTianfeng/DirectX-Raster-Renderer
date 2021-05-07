#include"UI_MaterialManager.h"

std::vector<UI_Material> UI_MaterialManager::ui_Materials = {};

void UI_Material::Render()
{
	//��ʾ���г��������ڱ���
	for (UINT i = 0; i < imGuiAvailableVariables.size(); i++)
	{

		//�жϱ�������
		switch (imGuiAvailableVariables[i].type)
		{
			//������Ϊfloatֵ
			case ImGuiAvailableVariableType::VT_FLOAT:
			{
				//���ݱ�����С�ж�������
				switch (imGuiAvailableVariables[i].size)
				{
					case sizeof(float) :
					{
						ImGui::DragFloat(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
					case sizeof(float) * 2:
					{
						ImGui::DragFloat2(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
					case sizeof(float) * 3:
					{
						ImGui::DragFloat3(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
					case sizeof(float) * 4:
					{
						ImGui::DragFloat4(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
				}
			}
			break;

			//������Ϊintֵ
			case ImGuiAvailableVariableType::VT_INT:
			{
				//���ݱ�����С�ж�������
				switch (imGuiAvailableVariables[i].size)
				{
					case sizeof(int) :
					{
						ImGui::DragInt(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
					case sizeof(int) * 2:
					{
						ImGui::DragInt2(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
					case sizeof(int) * 3:
					{
						ImGui::DragInt3(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
					case sizeof(int) * 4:
					{
						ImGui::DragInt4(imGuiAvailableVariables[i].name.c_str(), imGuiAvailableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
				}
			}
			break;

			case ImGuiAvailableVariableType::VT_BOOL:
			{
				ImGui::Checkbox("Active", &imGuiAvailableVariables[i].boolVal);
			}
			break;
		}
	}
}

void UI_MaterialManager::Render()
{
	if (!ImGui::CollapsingHeader("Materials"))
	{
		for (UINT i = 0; i < ui_Materials.size(); i++)
		{
			std::string materialName = "material " + std::to_string(i+1);

			if (ImGui::TreeNode(materialName.c_str())) {

				ui_Materials[i].Render();

				ImGui::TreePop();
			}

		}
	}


}
