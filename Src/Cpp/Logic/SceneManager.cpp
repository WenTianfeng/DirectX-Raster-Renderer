#include "SceneManager.h"
#include"..\Components\Skybox.h"
#include"..\Components\Light.h"
#include"..\Components\Attributes.h"

//全局静态变量初始化
Object* SceneManager::mainCamera = nullptr;
std::vector<Object*> SceneManager::lights = {};
std::vector<Object*> SceneManager::objects = {};

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

bool SceneManager::InitializeScene(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
#pragma region 判定shader文件路径

	std::wstring shaderFolderPath;
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG
		shaderFolderPath = L"Src\\Shaders\\";
#else //RELEASE（待修改）
		shaderFolderPath = L"..\\..\\..\\Debug\\";
#endif // DEBUG

	}
#pragma endregion

#pragma region 生成相机

	Object* camera = new Object(0);
	camera->AddComponent<Attributes>("Main Camera","Camera");
	camera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 30.0f, -80.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	camera->AddComponent<Camera>(DirectX::XM_PI / 3, 2.0f, 0.5f, 2000.0f);

	mainCamera = camera;

	this->objects.push_back(camera);//将对象加入列表中

#pragma endregion

#pragma region 生成光源

	Object* directional_light1 = new Object(1);
	directional_light1->AddComponent<Attributes>("Directional Light1","Light");
	directional_light1->AddComponent<Transform>(DirectX::XMFLOAT3(80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light1->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 0, 0, 0), 1.0f, 2000.0f);
	this->objects.push_back(directional_light1);
	lights.push_back(directional_light1);

	Object* directional_light2 = new Object(2);
	directional_light2->AddComponent<Attributes>("Directional Light2", "Light");
	directional_light2->AddComponent<Transform>(DirectX::XMFLOAT3(-80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, -DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light2->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(0, 0, 1.0f, 0), 1.0f, 2000.0f);
	this->objects.push_back(directional_light2);
	lights.push_back(directional_light2);

#pragma endregion

#pragma region 生成天空盒

	//Object* skybox = new Object(3);
	//skybox->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(500, 500, 500));

	//std::vector<std::wstring> skyboxTextureList = {
	//	L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Left.png", L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Right.png",
	//		L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Up.png", L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Down.png",
	//		L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Front.png", L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Back.png" };

	//skybox->AddComponent<Skybox>(device, deviceContext, skyboxTextureList);

	//std::wstring skyboxVertexShaderFilePaths[] = {
	//	shaderFolderPath + L"VertexShader_Skybox.cso"
	//};

	//std::wstring skyboxPixelShaderFilePaths[] = {
	//	shaderFolderPath + L"PixelShader_Skybox.cso"
	//};

	//skybox->AddComponent<MaterialManager>(device, skyboxVertexShaderFilePaths, skyboxPixelShaderFilePaths, 1);

	//this->m_objects.push_back(skybox);

#pragma endregion

#pragma region 生成场景对象

	//生成立方体
	Object* cube = new Object(4);
	cube->AddComponent<Attributes>("BoxBall", "Object");
	cube->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(DirectX::XM_PI / 2, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));//添加Transform组件
	cube->AddComponent<MeshRenderer>(device, deviceContext, "Assets\\Models\\BoxBall.FBX");//添加MeshRender组件
	
	std::wstring shaderFilePaths[] = {
		shaderFolderPath + L"PBRTest.hlsl",
		shaderFolderPath + L"PBRTest.hlsl"
	};

	cube->AddComponent<MaterialManager>(device, shaderFilePaths, 2);//添加MaterialManager组件

	this->objects.push_back(cube);

	//生成四边形
	//Object* quad = new Object(5);
	//quad->AddComponent<Attributes>("Quad");
	//quad->AddComponent<Transform>(DirectX::XMFLOAT3(0, 40, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(3, 3, 3));//添加Transform组件
	//quad->AddComponent<MeshRenderer>(device, deviceContext, "Assets\\Models\\Quad.FBX");//添加MeshRender组件

	//std::wstring quadVertexShaderFilePaths[] = {
	//	shaderFolderPath + L"PBRTest.hlsl",
	//};

	//std::wstring quadPixelShaderFilePaths[] = {
	//	shaderFolderPath + L"PBRTest.hlsl",
	//};

	//quad->AddComponent<MaterialManager>(device, quadVertexShaderFilePaths, quadPixelShaderFilePaths, 1);//添加MaterialManager组件

	//this->objects.push_back(quad);

#pragma endregion

	return true;
}
