#include "MaterialManager.h"

MaterialManager::MaterialManager(ID3D11Device* device, std::wstring* vertexShaderFiles, std::wstring* pixelShaderFiles, UINT shaderCount):
	m_dxDevice(device)
{
	this->m_componentName = "Material";

	for (UINT i = 0; i < shaderCount; i++)
	{
		Material material(this->m_dxDevice, *vertexShaderFiles, *pixelShaderFiles);

		this->materials.push_back(material);

		vertexShaderFiles++;
		pixelShaderFiles++;
	}

}
