#include "SceneManager.h"
#include"..\Components\Skybox.h"
#include"..\Components\Light.h"
#include"..\Components\Attributes.h"
#include"..\Logic\ObjectScripts\CameraControl.h"

#include"..\Tools\FileManager.h"


SceneManager::SceneManager():
	m_mainCamera(nullptr),
	m_dxDevice(nullptr),
	m_dxDeviceContext(nullptr),
	m_objectIDCounter(0)
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_dxDevice = device;
	this->m_dxDeviceContext = deviceContext;

	//初始化场景
	if (!InitializeScene())
	{
		return false;
	}

	return true;
}

void SceneManager::Update(float dt)
{
	//对所有场景对象进行更新
	for (auto object : this->m_objects)
	{
		object->Update(dt);
	}
}

void SceneManager::OnWindowResize(int clientWidth, int clientHeight)
{
	//重设主相机的横纵比
	m_mainCamera->GetComponent<Camera>()->SetAspect((float)clientWidth / (float)clientHeight);

}

Object* SceneManager::GetMainCamera()
{
	return this->m_mainCamera;
}

std::vector<Object*> SceneManager::GetLights()
{
	return this->m_lights;
}

std::vector<Object*> SceneManager::GetObjects()
{
	return this->m_objects;
}

void SceneManager::AddObject(std::string objectName, std::string objectMeshFilePath)
{

	if (objectName == "Light")
	{
		Object* light = new Object(this, NewObjectID());
		light->AddComponent<Attributes>(ObtainNewObjectNameWithPostfix(objectName, light->GetUID()), "Light");
		light->AddComponent<Transform>(DirectX::XMFLOAT3(80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
		light->AddComponent<Light>();
		this->m_objects.push_back(light);
		this->m_lights.push_back(light);
	}
	else if (objectName == "Camera")
	{
		Object* camera = new Object(this, NewObjectID());
		camera->AddComponent<Attributes>(ObtainNewObjectNameWithPostfix(objectName, camera->GetUID()), "Camera");
		camera->AddComponent<Transform>();
		camera->AddComponent<Camera>();
		this->m_objects.push_back(camera);//将对象加入列表中
	}
	else
	{
		Object* model = new Object(this, NewObjectID());
		model->AddComponent<Attributes>(ObtainNewObjectNameWithPostfix(objectName, model->GetUID()), "Object");
		model->AddComponent<Transform>();//添加Transform组件
		model->AddComponent<MeshRenderer>(m_dxDevice, m_dxDeviceContext, objectMeshFilePath);//添加MeshRender组件
		model->AddComponent<MaterialManager>(m_dxDevice);//添加MaterialManager组件

		this->m_objects.push_back(model);
	}

}

bool SceneManager::DeleteObject(UINT objectID)
{
	int targetIndex = -1;

	for (UINT i = 0; i < m_objects.size(); i++)
	{
		if (m_objects[i]->GetUID() == objectID)
		{
			targetIndex = i;

			break;
		}
	}

	if (targetIndex != -1)
	{
		m_objects[targetIndex]->Destroy();
		m_objects.erase(m_objects.begin() + targetIndex);

		return true;
	}
	else
	{
		return false;
	}
}

bool SceneManager::DeleteObject(Object* object)
{
	int targetIndex = -1;

	for (UINT i = 0; i < m_objects.size(); i++)
	{
		if (m_objects[i]->GetUID() == object->GetUID())
		{
			targetIndex = i;

			break;
		}
	}

	if (targetIndex != -1)
	{
		object->Destroy();
		m_objects.erase(m_objects.begin() + targetIndex);
		return true;
	}
	else
	{
		return false;
	}

}

UINT SceneManager::NewObjectID()
{
	m_objectIDCounter++;

	return m_objectIDCounter;
}

std::string SceneManager::ObtainNewObjectNameWithPostfix(std::string objectName,UINT objectID)
{
	std::string fullName = objectName + " (" + std::to_string(objectID) + ")";

	return fullName;
}

bool SceneManager::InitializeScene()
{
//=================场景生成=================

	#pragma region 生成相机

	//编辑模式相机
	Object* editModeCamera = new Object(this, NewObjectID());
	editModeCamera->AddComponent<Attributes>("EditMode Camera", "Camera", false);
	editModeCamera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 40.0f, -100.0f), DirectX::XMFLOAT3(DirectX::XM_PI / 12, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	editModeCamera->AddComponent<Camera>();
	editModeCamera->AddComponent<CameraControl>();
	m_mainCamera = editModeCamera;
	this->m_objects.push_back(editModeCamera);//将对象加入列表中

	//渲染模式相机
	Object* renderingModeCamera = new Object(this, NewObjectID());
	renderingModeCamera->AddComponent<Attributes>("RenderingMode Camera","Camera");
	renderingModeCamera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 100.0f, -200.0f), DirectX::XMFLOAT3(DirectX::XM_PI / 6, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	renderingModeCamera->AddComponent<Camera>();
	this->m_objects.push_back(renderingModeCamera);//将对象加入列表中

#	pragma endregion

	#pragma region 生成光源

	Object* directionalLight = new Object(this, NewObjectID());
	directionalLight->AddComponent<Attributes>("Directional Light","Light");
	directionalLight->AddComponent<Transform>(DirectX::XMFLOAT3(80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directionalLight->AddComponent<Light>();
	this->m_objects.push_back(directionalLight);
	m_lights.push_back(directionalLight);

	Object* pointLight = new Object(this, NewObjectID());
	pointLight->AddComponent<Attributes>("Point Light", "Light");
	pointLight->AddComponent<Transform>(DirectX::XMFLOAT3(-80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, -DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	pointLight->AddComponent<Light>(Light::LightType::Point, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0), 1.0f, 2000.0f, 60.0f);
	
	this->m_objects.push_back(pointLight);
	m_lights.push_back(pointLight);


	#pragma endregion

	#pragma region 生成天空盒

	Object* skybox = new Object(this,NewObjectID());
	skybox->AddComponent<Attributes>("DefaultSkybox", "Skybox", false);
	skybox->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(500, 500, 500));

	//天空盒纹理文件路径
	std::vector<std::wstring> skyboxTextureList = {
		L"Assets\\Custom\\Textures\\SkyboxTextures\\NightSkybox\\UrbanNightSky_Left.png", L"Assets\\Custom\\Textures\\SkyboxTextures\\NightSkybox\\UrbanNightSky_Right.png",
			L"Assets\\Custom\\Textures\\SkyboxTextures\\NightSkybox\\UrbanNightSky_Top.png", L"Assets\\Custom\\Textures\\SkyboxTextures\\NightSkybox\\UrbanNightSky_Bottom.png",
			L"Assets\\Custom\\Textures\\SkyboxTextures\\NightSkybox\\UrbanNightSky_Front.png", L"Assets\\Custom\\Textures\\SkyboxTextures\\NightSkybox\\UrbanNightSky_Back.png" };

	std::string skyboxShaderFilePath = "Assets\\Shaders\\DefaultSkybox.hlsl";

	skybox->AddComponent<Skybox>(m_dxDevice, m_dxDeviceContext, skyboxShaderFilePath, skyboxTextureList);

	this->m_objects.push_back(skybox);

	#pragma endregion


	return true;
}

