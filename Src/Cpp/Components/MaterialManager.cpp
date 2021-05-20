#include "MaterialManager.h"

MaterialManager::MaterialManager(ID3D11Device* device):
	m_dxDevice(device)
{
	//设置组件名称
	this->m_componentName = "Material";

}

void MaterialManager::Initialize()
{

	//根据着色器文件数量遍历
	for (UINT i = 0; i < owner->GetComponent<MeshRenderer>()->GetMeshCount(); i++)
	{
		//实例化材质
		Material material(this->m_dxDevice);
		material.Instantiate(ShaderFiles+"PBRTest.hlsl");

		//将材质加入到材质列表
		this->materials.push_back(material);
	}
}

