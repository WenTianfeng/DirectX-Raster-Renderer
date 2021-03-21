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
	//生成相机
	Object* camera = new Object();
	camera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 40, -40), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	camera->AddComponent<Camera>(DirectX::XM_PI / 2, 1.25f, 0.5f, 2000.0f);

	this->m_objects.push_back(camera);//将对象加入列表中
	//生成立方体
	Object* cube = new Object();
	cube->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));//添加Transform组件
	cube->AddComponent<MeshRenderer>();//添加MeshRender组件
	cube->AddComponent<MaterialManager>();//添加MaterialManager组件

	this->m_objects.push_back(cube);

	return true;
}
