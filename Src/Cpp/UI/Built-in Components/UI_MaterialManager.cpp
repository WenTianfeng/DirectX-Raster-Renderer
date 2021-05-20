#include"UI_MaterialManager.h"
#include"..\..\Tools\FileManager.h"

std::vector<UI_Material> UI_MaterialManager::ui_Materials = {};//��ʼ�� UI_MaterialManager �Ĳ���UI�б�̬��Ա

//-------------------------------------------UI_Material���--------------------------------------------
UI_Material::UI_Material():
	ownerMaterial(nullptr)
{
}

UI_Material::UI_Material(Material* material):
	ownerMaterial(material),
	shaderFilePath(material->GetShaderFilePath())
{

}

void UI_Material::Initialize(const Material& material)
{
	displayableVariables.clear();

	//�������ʵ�������ɫ����Դ
	for (std::map<std::string, ShaderParameter*>::value_type pair_name_shaderParameter : material.shaderParametersMap)
	{
		//������ɫ����Դ�������ͷֱ���
		switch (pair_name_shaderParameter.second->GetShaderParameterType())
		{
			//=========����Դ����Ϊ ConstantBuffer =========
			case ShaderParameter::ShaderParameterType::ConstantBuffer:
			{
				//���������������Ԥ�ñ������򲻽���UI��
				if (pair_name_shaderParameter.first == "CB_PresetVariables")
				{
					break;
				}

				//������������ı���map������ȡ�ı���ת��ΪImGui���ܵ���ʽ�����뵽����UI�����б���
				for (std::map<std::string, ConstantBufferVariable>::value_type pair_name_constantBufferVariable : pair_name_shaderParameter.second->constantBuffer->constantVariablesMap)
				{
					DisplayableVariable displayableVariable(pair_name_constantBufferVariable.second, pair_name_shaderParameter.second->constantBuffer);
					displayableVariables.push_back(displayableVariable);	
				}
			}
			break;

			//=========����Դ����Ϊ Texture =========
			case ShaderParameter::ShaderParameterType::Texture:
			{
				DisplayableVariable displayableVariable(pair_name_shaderParameter.first, pair_name_shaderParameter.second->texture);
				displayableVariables.push_back(displayableVariable);
			}
			break;

			//=========����Դ����Ϊ Sampler =========
			case ShaderParameter::ShaderParameterType::Sampler:
				break;

				//=========����Դ����Ϊ StructuredBuffer =========
			case ShaderParameter::ShaderParameterType::StructuredBuffer:
				break;

			case ShaderParameter::ShaderParameterType::Invalid:
				break;
		}

	}

}

void UI_Material::Render()
{
	//===============��ʾ��ѡ��Ĳ�����ɫ��============
	//��ʾ��ɫ���ļ�ѡ���
	if (ImGui::BeginCombo("Shader", shaderFilePath.c_str(), 0))
	{
		std::vector<std::string> files;
		std::string filePath = ShaderFiles;
		std::string format = ".hlsl";
		FileManager::GetFileNamesByFormat(filePath, files, format);

		static int shaderFIlesSelected = -1;

		//������ɫ���ļ�ѡ���
		for (UINT n = 0; n < files.size(); n++)
		{
			if (ImGui::Selectable(files[n].c_str(), shaderFIlesSelected == n))
			{
				shaderFIlesSelected = n;
				shaderFilePath = ShaderFiles+files[n];//����Ԥ���ļ���Ϊѡ����ļ�
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	//============��ʾ���пɲ�������ɫ����Դ����============
	#pragma region ��ʾ���пɲ�������ɫ����Դ����

	for (UINT i = 0; i < displayableVariables.size(); i++)
	{
		//�жϱ�������
		switch (displayableVariables[i].variableType)
		{
			//������Ϊfloatֵ
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
			{
				//���ݱ�����С�ж�������
				switch (displayableVariables[i].variableSize)
				{
					case sizeof(float) :
					{
						ImGui::DragFloat(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
					case sizeof(float) * 2:
					{
						ImGui::DragFloat2(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
					case sizeof(float) * 3:
					{
						ImGui::DragFloat3(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
					case sizeof(float) * 4:
					{
						ImGui::DragFloat4(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.2f");
					}
					break;
				}
			}
			break;

			//������Ϊintֵ
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
			{
				//���ݱ�����С�ж�������
				switch (displayableVariables[i].variableSize)
				{
					case sizeof(int) :
					{
						ImGui::DragInt(displayableVariables[i].variableName.c_str(), displayableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
					case sizeof(int) * 2:
					{
						ImGui::DragInt2(displayableVariables[i].variableName.c_str(), displayableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
					case sizeof(int) * 3:
					{
						ImGui::DragInt3(displayableVariables[i].variableName.c_str(), displayableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
					case sizeof(int) * 4:
					{
						ImGui::DragInt4(displayableVariables[i].variableName.c_str(), displayableVariables[i].intVec, 0.1f, 0, 0, "%d");
					}
					break;
				}
			}
			break;

			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_BOOL:
			{
				ImGui::Checkbox("Active", &displayableVariables[i].boolVal);
			}
			break;

			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE:
			{

				//��ʾ�����ļ�ѡ���
				if (ImGui::BeginCombo(displayableVariables[i].variableName.c_str(), displayableVariables[i].textureFile.c_str(), 0))
				{
					//�����Դ�ļ�Ŀ¼�����������ļ�
					std::vector<std::string> files;//��ȡ���ļ�·���б�
					files.push_back("None");

					std::string filePath = CustomizedTextureFiles;//����·��

					std::string format = ".png";//�����ļ���չ��
					FileManager::GetFileNamesByFormat(filePath, files, format);//�����ļ�

					format = ".jpg";//�����ļ���չ��
					FileManager::GetFileNamesByFormat(filePath, files, format);//�����ļ�



					static int textureFileSelected = -1;

					//���������ļ�ѡ���
					for (UINT n = 0; n < files.size(); n++)
					{
						if (ImGui::Selectable((files[n]).c_str(), textureFileSelected == n))
						{
							textureFileSelected = n;
							displayableVariables[i].textureFile = filePath+files[n];//����Ԥ���ļ���Ϊѡ����ļ�
						}
					}

					ImGui::EndCombo();
				}

				//��ʾ��������ͼ
				if (displayableVariables[i].textureFile != "None")
				{
					float texWidth = 80.0f;
					float texHeight = 80.0f;
					ImVec2 uvMin = ImVec2(0.0f, 0.0f);                 
					ImVec2 uvMax = ImVec2(1.0f, 1.0f);
					ImTextureID textureID = displayableVariables[i].ownerTexture->GetShaderResourceView();

					ImGui::Image(textureID, ImVec2(texWidth, texHeight), uvMin, uvMax);
				}
			}
			break;
		}

		ImGui::Spacing();

	}

	#pragma endregion

}


//-------------------------------------------UI_MaterialManager���--------------------------------------------
void UI_MaterialManager::Render()
{
	if (!ImGui::CollapsingHeader("MATERIALS"))
	{
		for (UINT i = 0; i < ui_Materials.size(); i++)
		{
			std::string materialName = "Material " + std::to_string(i+1);

			if (ImGui::TreeNode(materialName.c_str())) {

				ui_Materials[i].Render();
				ImGui::Separator();
				ImGui::TreePop();
			}

		}
	}


}
