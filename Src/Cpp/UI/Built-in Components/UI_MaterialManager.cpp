#include"UI_MaterialManager.h"
#include"..\..\Tools\FileManager.h"

std::vector<UI_Material> UI_MaterialManager::ui_Materials = {};//初始化 UI_MaterialManager 的材质UI列表静态成员

//-------------------------------------------UI_Material相关--------------------------------------------
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

	//遍历材质的所有着色器资源
	for (std::map<std::string, ShaderParameter*>::value_type pair_name_shaderParameter : material.shaderParametersMap)
	{
		//根据着色器资源参数类型分别处理
		switch (pair_name_shaderParameter.second->GetShaderParameterType())
		{
			//=========当资源类型为 ConstantBuffer =========
			case ShaderParameter::ShaderParameterType::ConstantBuffer:
			{
				//如果常量缓冲属于预置变量，则不将其UI化
				if (pair_name_shaderParameter.first == "CB_PresetVariables")
				{
					break;
				}

				//遍历常量缓冲的变量map，将获取的变量转换为ImGui接受的形式，加入到材质UI变量列表中
				for (std::map<std::string, ConstantBufferVariable>::value_type pair_name_constantBufferVariable : pair_name_shaderParameter.second->constantBuffer->constantVariablesMap)
				{
					DisplayableVariable displayableVariable(pair_name_constantBufferVariable.second, pair_name_shaderParameter.second->constantBuffer);
					displayableVariables.push_back(displayableVariable);	
				}
			}
			break;

			//=========当资源类型为 Texture =========
			case ShaderParameter::ShaderParameterType::Texture:
			{
				DisplayableVariable displayableVariable(pair_name_shaderParameter.first, pair_name_shaderParameter.second->texture);
				displayableVariables.push_back(displayableVariable);
			}
			break;

			//=========当资源类型为 Sampler =========
			case ShaderParameter::ShaderParameterType::Sampler:
				break;

				//=========当资源类型为 StructuredBuffer =========
			case ShaderParameter::ShaderParameterType::StructuredBuffer:
				break;

			case ShaderParameter::ShaderParameterType::Invalid:
				break;
		}

	}

}

void UI_Material::Render()
{
	//===============显示可选择的材质着色器============
	//显示着色器文件选择框
	if (ImGui::BeginCombo("Shader", shaderFilePath.c_str(), 0))
	{
		std::vector<std::string> files;
		std::string filePath = ShaderFiles;
		std::string format = ".hlsl";
		FileManager::GetFileNamesByFormat(filePath, files, format);

		static int shaderFIlesSelected = -1;

		//绘制着色器文件选择框
		for (UINT n = 0; n < files.size(); n++)
		{
			if (ImGui::Selectable(files[n].c_str(), shaderFIlesSelected == n))
			{
				shaderFIlesSelected = n;
				shaderFilePath = ShaderFiles+files[n];//设置预览文件名为选择的文件
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	//============显示所有可操作的着色器资源变量============
	#pragma region 显示所有可操作的着色器资源变量

	for (UINT i = 0; i < displayableVariables.size(); i++)
	{
		//判断变量类型
		switch (displayableVariables[i].variableType)
		{
			//当变量为float值
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
			{
				//根据变量大小判定分量数
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

			//当变量为int值
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
			{
				//根据变量大小判定分量数
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

				//显示纹理文件选择框
				if (ImGui::BeginCombo(displayableVariables[i].variableName.c_str(), displayableVariables[i].textureFile.c_str(), 0))
				{
					//检测资源文件目录下所有纹理文件
					std::vector<std::string> files;//获取的文件路径列表
					files.push_back("None");

					std::string filePath = CustomizedTextureFiles;//搜索路径

					std::string format = ".png";//搜索文件扩展名
					FileManager::GetFileNamesByFormat(filePath, files, format);//搜索文件

					format = ".jpg";//搜索文件扩展名
					FileManager::GetFileNamesByFormat(filePath, files, format);//搜索文件



					static int textureFileSelected = -1;

					//绘制纹理文件选择框
					for (UINT n = 0; n < files.size(); n++)
					{
						if (ImGui::Selectable((files[n]).c_str(), textureFileSelected == n))
						{
							textureFileSelected = n;
							displayableVariables[i].textureFile = filePath+files[n];//设置预览文件名为选择的文件
						}
					}

					ImGui::EndCombo();
				}

				//显示纹理缩略图
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


//-------------------------------------------UI_MaterialManager相关--------------------------------------------
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
