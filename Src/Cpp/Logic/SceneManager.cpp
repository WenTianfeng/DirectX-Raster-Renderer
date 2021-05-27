#include "SceneManager.h"
#include"..\Components\Skybox.h"
#include"..\Components\Light.h"
#include"..\Components\Attributes.h"
#include"..\Logic\ObjectScripts\CameraControl.h"

#include"..\Tools\FileManager.h"


SceneManager::SceneManager():
	m_mainCamera(nullptr)
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


void SceneManager::AddNewObject(std::string objectName, std::string objectMeshFilePath)
{
	if (objectName == "Light")
	{

	}
	else if (objectName == "Camera")
	{
		//编辑模式相机
		Object* newCamera = new Object(this, GetNewObjectID());
		newCamera->AddComponent<Attributes>("EditMode Camera", "Camera", false);
		newCamera->AddComponent<Transform>();
		newCamera->AddComponent<Camera>();
		newCamera->AddComponent<CameraControl>();
		this->m_objects.push_back(newCamera);//将对象加入列表中
	}
	else
	{
		Object* newObject = new Object(this, GetNewObjectID());
		newObject->AddComponent<Attributes>(ObtainNewObjectNameWithPostfix(objectName), "Object");
		newObject->AddComponent<Transform>();//添加Transform组件
		newObject->AddComponent<MeshRenderer>(m_dxDevice, m_dxDeviceContext, objectMeshFilePath);//添加MeshRender组件
		newObject->AddComponent<MaterialManager>(m_dxDevice);//添加MaterialManager组件

		this->m_objects.push_back(newObject);
	}

}

bool SceneManager::InitializeScene()
{
//=================场景生成=================

	#pragma region 生成相机

	//编辑模式相机
	Object* editModeCamera = new Object(this, GetNewObjectID());
	editModeCamera->AddComponent<Attributes>("EditMode Camera", "Camera", false);
	editModeCamera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 80.0f, -120.0f), DirectX::XMFLOAT3(DirectX::XM_PI / 12, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	editModeCamera->AddComponent<Camera>();
	editModeCamera->AddComponent<CameraControl>();
	m_mainCamera = editModeCamera;
	this->m_objects.push_back(editModeCamera);//将对象加入列表中

	//渲染模式相机
	Object* renderingModeCamera = new Object(this, GetNewObjectID());
	renderingModeCamera->AddComponent<Attributes>("RenderingMode Camera","Camera", true);
	renderingModeCamera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 100.0f, -200.0f), DirectX::XMFLOAT3(DirectX::XM_PI / 6, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	renderingModeCamera->AddComponent<Camera>();
	this->m_objects.push_back(renderingModeCamera);//将对象加入列表中

#	pragma endregion

	#pragma region 生成光源

	Object* directional_light1 = new Object(this, GetNewObjectID());
	directional_light1->AddComponent<Attributes>("Directional Light1","Light");
	directional_light1->AddComponent<Transform>(DirectX::XMFLOAT3(80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light1->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0), 1.0f, 2000.0f);
	this->m_objects.push_back(directional_light1);
	m_lights.push_back(directional_light1);

	Object* directional_light2 = new Object(this, GetNewObjectID());
	directional_light2->AddComponent<Attributes>("Directional Light2", "Light");
	directional_light2->AddComponent<Transform>(DirectX::XMFLOAT3(-80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, -DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light2->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0), 1.0f, 2000.0f);
	
	this->m_objects.push_back(directional_light2);
	m_lights.push_back(directional_light2);

	#pragma endregion

	#pragma region 生成天空盒

	//Object* skybox = new Object(GetNewObjectID());
	//skybox->AddComponent<Attributes>("DefaultSkybox", "Skybox");
	//skybox->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(500, 500, 500));

	////天空盒纹理文件路径
	//std::vector<std::wstring> skyboxTextureList = {
	//	L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Left.png", L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Right.png",
	//		L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Top.png", L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Bottom.png",
	//		L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Front.png", L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Back.png" };

	//skybox->AddComponent<Skybox>(device, deviceContext, skyboxTextureList);

	////默认天空盒着色器
	//std::string skyboxShaderFilePaths[] = {
	//	shaderFolderPath + "DefaultSkybox.hlsl"
	//};

	//skybox->AddComponent<MaterialManager>(device, skyboxShaderFilePaths, 1);

	//this->objects.push_back(skybox);

	#pragma endregion

	#pragma region 生成场景对象

	//机器人
	Object* robot = new Object(this, GetNewObjectID());
	robot->AddComponent<Attributes>("Robot", "Object");
	robot->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));//添加Transform组件
	robot->AddComponent<MeshRenderer>(m_dxDevice, m_dxDeviceContext, CustomizedMeshFiles+"Robot Kyle.fbx");//添加MeshRender组件
	robot->AddComponent<MaterialManager>(m_dxDevice);//添加MaterialManager组件

	this->m_objects.push_back(robot);

	//生成地面平面
	Object* ground = new Object(this, GetNewObjectID());
	ground->AddComponent<Attributes>("Ground");
	ground->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(DirectX::XM_PI / 2, 0, 0), DirectX::XMFLOAT3(20, 10, 20));//添加Transform组件
	ground->AddComponent<MeshRenderer>(m_dxDevice, m_dxDeviceContext, PresetMeshFiles+"Quad.fbx");//添加MeshRender组件
	ground->AddComponent<MaterialManager>(m_dxDevice);//添加MaterialManager组件

	this->m_objects.push_back(ground);

	#pragma endregion

	return true;
}

UINT SceneManager::GetNewObjectID()
{
	return m_objects.size();
}

std::string SceneManager::ObtainNewObjectNameWithPostfix(std::string objectName)
{
	std::string ss = "ss";
	return ss;
}
