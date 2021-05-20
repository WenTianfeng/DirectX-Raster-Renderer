#include "MaterialManager.h"

MaterialManager::MaterialManager(ID3D11Device* device):
	m_dxDevice(device)
{
	//�����������
	this->m_componentName = "Material";

}

void MaterialManager::Initialize()
{

	//������ɫ���ļ���������
	for (UINT i = 0; i < owner->GetComponent<MeshRenderer>()->GetMeshCount(); i++)
	{
		//ʵ��������
		Material material(this->m_dxDevice);
		material.Instantiate(ShaderFiles+"PBRTest.hlsl");

		//�����ʼ��뵽�����б�
		this->materials.push_back(material);
	}
}

