#include "MaterialManager.h"

MaterialManager::MaterialManager(ID3D11Device* device, std::string* shaderFiles, UINT shaderFileCount):
	m_dxDevice(device)
{
	//设置组件名称
	this->m_componentName = "Material";

	//根据着色器文件数量遍历
	for (UINT i = 0; i < shaderFileCount; i++)
	{
		//实例化材质
		Material material(this->m_dxDevice);
		material.Instantiate(*shaderFiles);

		//将材质加入到材质列表
		this->materials.push_back(material);

		//指针后移
		shaderFiles++;
	}

}

