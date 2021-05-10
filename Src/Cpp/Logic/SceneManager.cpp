#include "SceneManager.h"
#include"..\Components\Skybox.h"
#include"..\Components\Light.h"
#include"..\Components\Attributes.h"

//全局静态变量初始化
Object* SceneManager::mainCamera = nullptr;
std::vector<Object*> SceneManager::lights = {};
std::vector<Object*> SceneManager::objects = {};

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//初始化场景
	if (!InitializeScene(device,deviceContext))
	{
		return false;
	}

	return true;
}

void SceneManager::Update(float dt)
{
	//对所有场景对象进行更新
	for (auto object : this->objects)
	{
		object->Update(dt);
	}
}

void SceneManager::OnWindowResize(int clientWidth, int clientHeight)
{
	//重设主相机的横纵比
	mainCamera->GetComponent<Camera>()->SetAspect((float)clientWidth / (float)clientHeight);

}

bool SceneManager::InitializeScene(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

	#pragma region 判定shader文件路径

	std::string shaderFolderPath;

	//调式模式
	#ifdef _DEBUG
			shaderFolderPath = "Src\\Shaders\\";
	#endif
	
	//发布模式
	#ifdef NDEBUG
			shaderFolderPath = "Assets\\Shaders\\";
	#endif
	
	#pragma endregion

//=================场景生成=================

	#pragma region 生成相机

	Object* camera = new Object(0);
	camera->AddComponent<Attributes>("Main Camera","Camera");
	camera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 80.0f, -120.0f), DirectX::XMFLOAT3(DirectX::XM_PI / 12, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	camera->AddComponent<Camera>();

	mainCamera = camera;

	this->objects.push_back(camera);//将对象加入列表中

#	pragma endregion

	#pragma region 生成光源

	Object* directional_light1 = new Object(1);
	directional_light1->AddComponent<Attributes>("Directional Light1","Light");
	directional_light1->AddComponent<Transform>(DirectX::XMFLOAT3(80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light1->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0), 1.0f, 2000.0f);
	this->objects.push_back(directional_light1);
	lights.push_back(directional_light1);

	Object* directional_light2 = new Object(2);
	directional_light2->AddComponent<Attributes>("Directional Light2", "Light");
	directional_light2->AddComponent<Transform>(DirectX::XMFLOAT3(-80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, -DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light2->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0), 1.0f, 2000.0f);
	this->objects.push_back(directional_light2);
	lights.push_back(directional_light2);

	#pragma endregion

	#pragma region 生成天空盒

	//Object* skybox = new Object(3);
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
	Object* cube = new Object(4);
	cube->AddComponent<Attributes>("Robot", "Object");
	cube->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));//添加Transform组件
	cube->AddComponent<MeshRenderer>(device, deviceContext, "Assets\\Models\\Robot Kyle.fbx");//添加MeshRender组件
	
	std::string shaderFilePaths[] = {
		shaderFolderPath + "PBRTest.hlsl",
		shaderFolderPath + "PBRTest.hlsl"
	};

	cube->AddComponent<MaterialManager>(device, shaderFilePaths, 2);//添加MaterialManager组件

	this->objects.push_back(cube);

	//生成地面平面
	Object* ground = new Object(5);
	ground->AddComponent<Attributes>("Ground");
	ground->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(DirectX::XM_PI / 2, 0, 0), DirectX::XMFLOAT3(10, 10, 10));//添加Transform组件
	ground->AddComponent<MeshRenderer>(device, deviceContext, "Assets\\Models\\Quad.fbx");//添加MeshRender组件
	ground->AddComponent<MaterialManager>(device, shaderFilePaths, 1);//添加MaterialManager组件

	this->objects.push_back(ground);

	#pragma endregion

	return true;
}
