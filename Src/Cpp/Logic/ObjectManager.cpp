#include "ObjectManager.h"

bool ObjectManager::Initialize()
{
	return false;
}

void ObjectManager::RenderFrame()
{
	for (auto object : this->m_objects)
	{
		object->Render();
	}
}

bool ObjectManager::InitializeScene()
{
	//�������
	Object* camera = new Object();
	camera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 40, -40), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	camera->AddComponent<Camera>(DirectX::XM_PI / 2, 1.25f, 0.5f, 2000.0f);

	this->m_objects.push_back(camera);//����������б���
	//����������
	Object* cube = new Object();
	cube->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));//���Transform���
	cube->AddComponent<MeshRenderer>();//���MeshRender���
	cube->AddComponent<MaterialManager>();//���MaterialManager���

	this->m_objects.push_back(cube);

	return true;
}
