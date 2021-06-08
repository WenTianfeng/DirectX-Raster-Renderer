#include"UI_MaterialManager.h"
#include"..\..\Tools\FileManager.h"


bool UI_Material::depthEnable = true;
bool UI_Material::blendEnable = true;

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

void UI_Material::Initialize()
{
	depthEnable = ownerMaterial->depthStencilDesc.DepthEnable;
	blendEnable = ownerMaterial->blendDesc.BlendEnable;

	displayableVariables.clear();

	//�������ʵ�������ɫ����Դ
	for (std::map<std::string, ShaderParameter*>::value_type pair_name_shaderParameter : ownerMaterial->shaderParametersMap)
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
	#pragma region ��ʾ���пɲ�������ɫ����Դ����
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
	#pragma endregion

	ImGui::Separator();
	//============��ʾ���пɲ�������ɫ����Դ����============
	#pragma region ��ʾ���пɲ�������ɫ����Դ����

	ImGui::Text("Shader Parameters");

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
						ImGui::DragFloat(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.3f");
					}
					break;
					case sizeof(float) * 2:
					{
						ImGui::DragFloat2(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.3f");
					}
					break;
					case sizeof(float) * 3:
					{
						ImGui::DragFloat3(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.3f");
					}
					break;
					case sizeof(float) * 4:
					{
						ImGui::DragFloat4(displayableVariables[i].variableName.c_str(), displayableVariables[i].floatVec, 0.1f, 0.0f, 0.0f, "%.3f");
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
							if (files[n] == "None")
							{
								displayableVariables[i].textureFile = files[n];//����Ԥ���ļ���ΪNone
							}
							else
							{
								displayableVariables[i].textureFile = filePath+files[n];//����Ԥ���ļ���Ϊѡ����ļ�
							}
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
	
	ImGui::Separator();

	//===============��ʾ��Ⱦ���߲���״̬����===============
	#pragma region ��ʾ��Ⱦ���߲���״̬����

	ImGui::Text("Rendering State Parameters");
	
	//--------------------��դ��״̬����--------------------
	//�޳�ģʽ
	if (ImGui::BeginCombo("Cull Mode", Material::CullModes[ownerMaterial->rasterizerDesc.CullMode-1].c_str(), 0))
	{
		static int cullModeSelected = -1;

		for (int n = 0; n < 3; n++)
		{
			if (ImGui::Selectable((Material::CullModes[n]).c_str(), cullModeSelected == n))
			{
				cullModeSelected = n;

				ownerMaterial->rasterizerDesc.CullMode = (D3D11_CULL_MODE)(n+1);
				ownerMaterial->ReSetupRasterizerState();
			}
		}
		ImGui::EndCombo();
	}

	
	//���ģʽ
	if (ImGui::BeginCombo("Fill Mode", Material::FillModes[ownerMaterial->rasterizerDesc.FillMode - 2].c_str(), 0))
	{
		static int fillModeSelected = -1;

		for (int n = 0; n < 2; n++)
		{
			if (ImGui::Selectable((Material::FillModes[n]).c_str(), fillModeSelected == n))
			{
				fillModeSelected = n;

				ownerMaterial->rasterizerDesc.FillMode = (D3D11_FILL_MODE)(n + 2);
				ownerMaterial->ReSetupRasterizerState();
			}
		}
		ImGui::EndCombo();
	}

	//--------------------���ģ��״̬����--------------------
	ImGui::Checkbox("Depth Enable", &depthEnable);

	if ((int)depthEnable != ownerMaterial->depthStencilDesc.DepthEnable)
	{
		ownerMaterial->depthStencilDesc.DepthEnable = (int)depthEnable;
		ownerMaterial->ReSetupDepthStencilState();
	}

	if (depthEnable)
	{
		//���д������
		if (ImGui::BeginCombo("Depth Write Mask", Material::DepthWriteMasks[ownerMaterial->depthStencilDesc.DepthWriteMask].c_str(), 0))
		{
			static int depthWriteMaskSelected = -1;

			for (int n = 0; n < 2; n++)
			{
				if (ImGui::Selectable((Material::DepthWriteMasks[n]).c_str(), depthWriteMaskSelected == n))
				{
					depthWriteMaskSelected = n;

					ownerMaterial->depthStencilDesc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)(n);
					ownerMaterial->ReSetupDepthStencilState();
				}
			}
			ImGui::EndCombo();
		}

		//���д��ȽϺ���
		if (ImGui::BeginCombo("Depth Write Func", Material::DepthFuncs[ownerMaterial->depthStencilDesc.DepthFunc - 1].c_str(), 0))
		{
			static int depthWriteFuncSelected = -1;

			for (int n = 0; n < 8; n++)
			{
				if (ImGui::Selectable((Material::DepthFuncs[n]).c_str(), depthWriteFuncSelected == n))
				{
					depthWriteFuncSelected = n;

					ownerMaterial->depthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)(n + 1);
					ownerMaterial->ReSetupDepthStencilState();
				}
			}
			ImGui::EndCombo();
		}
	}

	//--------------------���״̬����--------------------
	ImGui::Checkbox("Blend Enable", &blendEnable);
	if ((int)blendEnable != ownerMaterial->blendDesc.BlendEnable)
	{
		ownerMaterial->blendDesc.BlendEnable = (int)blendEnable;
		ownerMaterial->ReSetupBlendState();
	}

	if (blendEnable)
	{
		//��Ϸ�ʽ
		if (ImGui::BeginCombo("Blend OP", Material::BlendOps[ownerMaterial->blendDesc.BlendOp - 1].c_str(), 0))
		{
			static int blendOPSelected = -1;

			for (int n = 0; n < 5; n++)
			{
				if (ImGui::Selectable((Material::BlendOps[n]).c_str(), blendOPSelected == n))
				{
					blendOPSelected = n;

					ownerMaterial->blendDesc.BlendOp = (D3D11_BLEND_OP)(n + 1);
					ownerMaterial->ReSetupBlendState();
				}
			}
			ImGui::EndCombo();
		}
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
