#include "MaterialManager.h"

MaterialManager::MaterialManager(ID3D11Device* device, std::string* shaderFiles, UINT shaderFileCount):
	m_dxDevice(device)
{
	//�����������
	this->m_componentName = "Material";

	//������ɫ���ļ���������
	for (UINT i = 0; i < shaderFileCount; i++)
	{
		//ʵ��������
		Material material(this->m_dxDevice);
		material.Instantiate(*shaderFiles);

		//�����ʼ��뵽�����б�
		this->materials.push_back(material);

		//ָ�����
		shaderFiles++;
	}

}

